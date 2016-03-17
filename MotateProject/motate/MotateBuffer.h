/*
 MotateBuffer.hpp - Buffer templates for the Motate system
 http://github.com/synthetos/motate/

 Copyright (c) 2013 - 2016 Robert Giseburt
 Copyright (c)        2016 Alden S. Hart, Jr.

 This file is part of the Motate Library.

 This file ("the software") is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2 as published by the
 Free Software Foundation. You should have received a copy of the GNU General Public
 License, version 2 along with the software. If not, see <http://www.gnu.org/licenses/>.

 As a special exception, you may use this file as part of a software library without
 restriction. Specifically, if other files instantiate templates or use macros or
 inline functions from this file, or you compile this file and link it with  other
 files to produce an executable, this file does not by itself cause the resulting
 executable to be covered by the GNU General Public License. This exception does not
 however invalidate any other reasons why the executable file might be covered by the
 GNU General Public License.

 THE SOFTWARE IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
 WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MOTATEBUFFER_H_ONCE
#define MOTATEBUFFER_H_ONCE

#include <inttypes.h>
//#include <utility> // for std::move

namespace Motate {
    // Implement a simple circular buffer, with a compile-time size
    template <uint16_t _size, typename base_type = char>
    struct Buffer {
        // Internal properties!
        base_type _data[_size];

        const base_type* const _end_pos = _data+_size; // NOTE: _end_pos is one *past* the end of the buffer.
        volatile base_type* _read_pos = _data;
        volatile base_type* _write_pos = _data;
        volatile int16_t _available = _size;
        volatile bool _write_locked = true;

        struct _mutex {
            volatile bool &_mutex_lock;
            _mutex(volatile bool& _lockVar) : _mutex_lock {_lockVar} {
                _mutex_lock = true;
            };

            ~_mutex() {
                _mutex_lock = false;
            }
        };

        constexpr int16_t size() { return _size; };

        // Default constructors will work fine.

        volatile base_type *nextReadPos() {
            volatile base_type *ret = _read_pos+1;
            if (ret == _end_pos) {
                ret = _data;
            }
            return ret;
        };

        volatile base_type *nextWritePos() {
            volatile base_type *ret = _write_pos+1;
            if (ret == _end_pos) {
                ret = _data;
            }
            return ret;
        };

        bool isEmpty() { return _available == _size; }
        bool isFull() { return _available == 0; }
        bool isLocked() { return _write_locked; }
        //	bool isEmpty() { return _read_pos == _write_pos; }
        //	bool isFull() { return nextReadPos() == _write_pos; }

        int16_t peek() {
            if (isEmpty())
                return -1;
            int16_t ret = *_read_pos;
            return ret;
        };

        void pop() {
            if (isEmpty())
                return; // Ignore pop on an empty buffer

            _read_pos = nextReadPos();
            _available++;
            return;
        };

        int16_t read() {
            if (isEmpty() || isLocked())
                return -1;

            int16_t ret = *_read_pos;
            _read_pos = nextReadPos();

            _available++;
            return ret;
        };
        
        int16_t write(const base_type newValue) {
            if (isFull())
                return -1;
            
            _mutex write_lock(_write_locked);
            
            // as quickly as possible we'll change the availability
            _available--;
            
            *_write_pos = newValue;
            _write_pos = nextWritePos();
            
            return 1;
        };
        
        int16_t available() {
            return _available;
            //	    if (_read_pos <= _write_pos) {
            //		return _write_pos - _read_pos;
            //	    } else {
            //		return (_write_pos - _data) + (_end_pos - _read_pos);
            //	    }
        };
    };

    // Implement a simple circular buffer, with a compile-time size, and can only be written to by DMA
    // owner_type is a *pointer* type thet implements const base_type* getRXTransferPosition()
    template <uint16_t _size, typename owner_type, uint16_t _reserve_size_set = 0, typename base_type = char>
    struct RXBuffer {
        static_assert(((_size-1)&_size)==0, "RXBuffer size must be 2^N");

        owner_type _owner;

        // Internal properties!
        base_type _data[_size + _reserve_size_set];
        static const uint16_t _reserve_size = _reserve_size_set; // store _reserve_size where subclasses can get to it

//        const base_type* const _end_pos = _data+_size; // NOTE: _end_pos is one *past* the end of the buffer.
//        base_type* _read_pos = _data;

        uint16_t _read_offset;              // The offset into the buffer we of our next read
        uint16_t _last_known_write_offset;  // The offset into the buffer we of the last known write (cached)

        uint16_t _transfer_requested = 0; // keep track of how much we have requested. Non-zero means a request is active.

        constexpr int16_t size() { return _size; };

        RXBuffer(owner_type owner) : _owner(owner) {
            _owner->setRXTransferDoneCallback([&]() { // use a closure
                _transfer_requested = 0;
                _restartTransfer();
            });
        };

        uint16_t _nextReadOffset() {
            return (_read_offset + 1)&(_size-1);
        };

        bool _canBeRead(uint16_t pos) {
            if (pos == (_last_known_write_offset & (_size-1))) {
                _getWriteOffset();
                if (pos == (_last_known_write_offset & (_size-1))) {
                    _restartTransfer();
                    return false;
                }
            }
            return true;
        };

        uint16_t _getWriteOffset() {
            base_type* pos = _owner->getRXTransferPosition();
            if (pos==nullptr) {
                _last_known_write_offset = 0;
            } else {
                _last_known_write_offset = (pos - _data);
            }
            return _last_known_write_offset;
        }


        bool isLocked() { return false; } // this kind of buffer cannot be locked

        // isEmpth and isFull are expensive, and should be avoided when not strictly necessary!

        bool isEmpty() {
            // If we weren't empty last time we checked, we aren't empty now.
            if (!_isEmptyCached()) {
                return false;
            }

            // Update the cache and check again
            _getWriteOffset();
            return _isEmptyCached();
        }

        bool isFull() {
            // If we were full last time we checked, we are still full
            if (_isFullCached()) {
                return true;
            }

            return _isFullCached();
        }


        // _isEmptyCached and _isFullCached aren't as expensive, since they only use the cached _last_known_write_offset
        bool _isEmptyCached() { return _read_offset == _last_known_write_offset; }
        bool _isFullCached() { return _nextReadOffset() == _last_known_write_offset; }

        void flush() {
            // We can't stop the machinery, but we can "trow away" what we have read so far.
            _read_offset = _getWriteOffset();
        }

        int16_t peek() {
            if (isEmpty())
                return -1;

            int16_t ret = _data[_read_offset];
            return ret;
        };

        void pop() {
            if (isEmpty())
                return; // Ignore pop on an empty buffer

            _read_offset = _nextReadOffset();
            return;
        };

        void _restartTransfer() {
            if ((_transfer_requested == 0) && !isFull()) {
                // We can only request contiguous chunks. Let's see what the next one is.
                _getWriteOffset(); // cache the write position

                int16_t transfer_size = 0;
                char *_write_pos = _data + _last_known_write_offset;

                // Possible cases:
                // [1] _read_pos <= _write_pos && _write_pos < _end_pos
                //     IOW: We read to some position in the middle, and read is between start and write.
                //          So, we can transfer from write to the end of the buffer.
                // [2] _read_pos <= _write_pos && _write_pos == _end_pos
                //     IOW: We read to the end of the buffer.
                //          So, we transfer from the start of the buffer up to the read position.
                // [3] _read_pos > _write_pos
                //     IOW: We read to some position in the middle, and write is before it (so the data is between read->end, then start->write).
                //          So, we transfer from write to the read position.

                // Case [3] (tested first, since tests [1] and [2] simplify.)
                if (_read_offset > _last_known_write_offset) {
                    transfer_size = _read_offset - _last_known_write_offset;

                // Case [2]
                } else if (_last_known_write_offset == _size) {
                    _last_known_write_offset = 0;
                    _write_pos = _data;
                    transfer_size = _read_offset;

                // Case [1]
                } else {
                    transfer_size = _size - _last_known_write_offset;
                }

                // We set _transfer_requested BEFORE startRXTransfer, in case an interrupt fires before we exit startRXTransfer
                //   (which should only happen if it started succesfully).
                // startRXTransfer will return false if it couldn't start the transfer.
                _transfer_requested = transfer_size;
                if (!_owner->startRXTransfer(_write_pos, transfer_size)) {
                    _transfer_requested = 0;
                }
            }
        };

        int16_t read() {
            if (isEmpty()) {
                _restartTransfer();
                return -1;
            }

            int16_t ret = _data[_read_offset];
            _read_offset = _nextReadOffset();

            return ret;
        };

        int16_t _getAvailableCached() {
            if (_read_offset == _last_known_write_offset) {
                return _size;
            } else if (_read_offset < _last_known_write_offset) {
                return _size  - (_last_known_write_offset - _read_offset);
            } else {
                return (_last_known_write_offset) + (_size - _read_offset);
            }
        };


        int16_t available() {
            _getWriteOffset(); // cache the write position
            return _getAvailableCached();
        };
    };
} // namespace Motate

#endif /* end of include guard: MOTATEBUFFER_H_ONCE */