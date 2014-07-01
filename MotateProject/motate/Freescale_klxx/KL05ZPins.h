/*
  KL05ZPins.h - Library for the Motate system
  http://tinkerin.gs/

  Copyright (c) 2013 Robert Giseburt

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

#ifndef KL05ZPINS_H_ONCE
#define KL05ZPINS_H_ONCE

// #include <chip.h>
#include "MKL05Z4.h"
#include <type_traits>
//#include "Freescale_klxx/KL05ZCommon.h"

namespace Motate {
	// Numbering is arbitrary:
	enum PinMode {
		kUnchanged      = 0,
		kOutput         = 1,
		kInput          = 2,

		/* kPeripheral* is to prevent setting the mux to GPIO. */
		/* This is for Motate internal-use only, and should never be in user-code. */
		kPeripheralOutput = 3,
		kPeripheralInput  = 4
	};

	// Numbering is arbitrary, but bit unique for bitwise operations (unlike other architectures):
	// Thses are NOT in order.
	enum PinOptions {
		kNormal         = 0,
		kTotem          = 0, // alias
		kPullUp         = 1<<1,
		// KL05Z appears to not allow configuring pull direction, even though there's a register for it.
		// They're all pull-up, except PTA0/SWD_CLK, which is pull-down.
#if !defined(MOTATE_AVR_COMPATIBILITY) && !defined(MOTATE_AVRX_COMPATIBILITY) && !defined(MOTATE_SAM_COMPATIBILITY)
		// Drive strength control only works on:
		//   PTA12, PTA13, PTB0, PTB1
		kDriveStrengthLow  = 1<<2,
		kDriveStrengthHigh = 1<<3,
#endif // !MOTATE_AVR_COMPATIBILITY && !MOTATE_SAM_COMPATIBILITY

		// The PinMux* modes are necessary for proper fuction of the library,
		// but should NOT be used outside of Motate to maintain cross-platform support.

		// Uses 4 bits
		kPinMuxOffset      = 4,
		kPinMuxOff         = (0b1000 | 0) << kPinMuxOffset,
		kPinMux0           = (0b1000 | 0) << kPinMuxOffset, // alias
		kPinMuxGPIO        = (0b1000 | 1) << kPinMuxOffset,
		kPinMux1           = (0b1000 | 1) << kPinMuxOffset, // alias
		kPinMux2           = (0b1000 | 2) << kPinMuxOffset,
		kPinMux3           = (0b1000 | 3) << kPinMuxOffset,
		kPinMux4           = (0b1000 | 4) << kPinMuxOffset,
		kPinMux5           = (0b1000 | 5) << kPinMuxOffset,
		kPinMux6           = (0b1000 | 6) << kPinMuxOffset,
		kPinMux7           = (0b1000 | 7) << kPinMuxOffset,
		kPinMuxSet         = 0b1000 << kPinMuxOffset,
		kPinMuxMask        = 0b0111 << kPinMuxOffset,

		// For use on PWM pins only!
		kPWMPinInverted    = 1<<8
	};
    
    enum PinInterruptOptions {
        kPinInterruptsOff                = 0,
        
        kPinInterruptOnChange            = 0b1011,
        
        kPinInterruptOnRisingEdge        = 0b1001,
        kPinInterruptOnFallingEdge       = 0b1010,
        
        kPinInterruptOnLowLevel          = 0b1000,
        kPinInterruptOnHighLevel         = 0b1100,

		kPinInterruptTypeMask            = 0b1111,

        /* This turns the IRQ on, but doesn't set the timer to ever trigger it. */
		kPinInterruptOnSoftwareTrigger   = 1<<4,

		/* Set priority levels here as well: */
		kPinInterruptPriorityHighest     = 1<<5,
		kPinInterruptPriorityHigh        = 1<<6,
		kPinInterruptPriorityMedium      = 1<<7,
		kPinInterruptPriorityLow         = 1<<8,
		kPinInterruptPriorityLowest      = 1<<9,
        
        kPinInterruptPriorityMask        = ((1<<10) - (1<<5))
    };
	
	typedef uint32_t uintPort_t;
    
	typedef const int8_t pin_number;

	template <unsigned char portLetter>
	struct Port32 {
		static const uint8_t letter = 0; // NULL stub!

		static const uint32_t pmcId() {
            return 0;
        };
		
		void setModes(const uintPort_t value, const uintPort_t mask = 0xffffffff) {
			// stub
		};
		void setOptions(const uint16_t options, const uintPort_t mask) {
			// stub
		};
		void getModes() {
			// stub
		};
		void getOptions() {
			// stub
		};
		void set(const uintPort_t value) {
			// stub
		};
		void clear(const uintPort_t value) {
			// stub
		};
		void write(const uintPort_t value) {
			// stub
		};
		void write(const uintPort_t value, const uintPort_t mask) {
			// stub
		};
		uintPort_t getInputValues(const uintPort_t mask = 0xffffffff) {
			// stub
			return 0;
		};
		uintPort_t getOutputValues(const uintPort_t mask = 0xffffffff) {
			// stub
			return 0;
		};
	};

	template<int8_t pinNum>
	struct Pin {
		static const int8_t number = -1;
		static const uint8_t portLetter = 0;
		static const uint32_t mask = 0;

		Pin() {};
		Pin(const PinMode type, const PinOptions options = kNormal) {};
		void operator=(const bool value) {};
		operator bool() { return 0; };
		
		void init(const PinMode type, const uint16_t options = kNormal, const bool fromConstructor=false) {};
		void setMode(const PinMode type, const bool fromConstructor=false) {};
		PinMode getMode() { return kUnchanged; };
		void setOptions(const uint16_t options, const bool fromConstructor=false) {};
		uint16_t getOptions() { return kNormal; };
		void set() {};
		void clear() {};
		void write(const bool value) {};
		void toggle() {};
		uint8_t get() { return 0; };
		uint8_t getInputValue() { return 0; };
		uint8_t getOutputValue() { return 0; };
		static uint32_t maskForPort(const uint8_t otherPortLetter) { return 0; };
		bool isNull() { return true; };
        
        /* Placeholder for user code. */\
        static void interrupt() __attribute__ ((weak));\
	};

    template<uint8_t portChar, uint8_t portPin>
	struct ReversePinLookup : Pin<-1> {
		ReversePinLookup() {};
		ReversePinLookup(const PinMode type, const PinOptions options = kNormal) : Pin<-1>(type, options) {};
    };
    
	template<int8_t pinNum>
	struct InputPin : Pin<pinNum> {
		InputPin() : Pin<pinNum>(kInput) {};
		InputPin(const PinOptions options) : Pin<pinNum>(kInput, options) {};
		void init(const PinOptions options = kNormal  ) {Pin<pinNum>::init(kInput, options);};
		uint32_t get() {
			return Pin<pinNum>::getInputValue();
		};
		/*Override these to pick up new methods */
		operator bool() { return (get() != 0); };
	private: /* Make these private to catch them early. These are intentionally not defined. */
		void init(const PinMode type, const PinOptions options = kNormal);
		void operator=(const bool value) { Pin<pinNum>::write(value); };
		void write(const bool);
	};

	template<int8_t pinNum>
	struct OutputPin : Pin<pinNum> {
		OutputPin() : Pin<pinNum>(kOutput) {};
		OutputPin(const PinOptions options) : Pin<pinNum>(kOutput, options) {};
		void init(const PinOptions options = kNormal) {Pin<pinNum>::init(kOutput, options);};
		uint32_t get() {
			return Pin<pinNum>::getOutputValue();
		};
		void operator=(const bool value) { Pin<pinNum>::write(value); };
		/*Override these to pick up new methods */
		operator bool() { return (get() != 0); };
	private: /* Make these private to catch them early. */
		void init(const PinMode type, const PinOptions options = kNormal); /* Intentially not defined. */
	};	
	
	// TODO: Make the Pin<> use the appropriate Port<>, reducing duplication when there's no penalty
	
    #define _MAKE_MOTATE_PIN(pinNum, registerLetter, registerChar, registerPin)\
		template<>\
		struct Pin<pinNum> {\
		private: /* Lock the copy contructor.*/\
			Pin(const Pin<pinNum>&){};\
		public:\
			static const int8_t number = pinNum;\
			static const uint8_t portLetter = (uint8_t) registerChar;\
			static const uint32_t mask = (1u << registerPin);\
            \
			Pin() {};\
			Pin(const PinMode type, const PinOptions options = kNormal) {\
				init(type, options, /*fromConstructor=*/true);\
			};\
			void operator=(const bool value) { write(value); };\
			operator bool() { return (get() != 0); };\
			\
			void init(const PinMode type, const uint16_t options = kNormal | kDriveStrengthHigh, const bool fromConstructor=false) {\
				/* Turn on the clock for this peripheral */\
				/* ASSUMPTION: All ports are in SIM_SCGC5 on this processor */\
				SIM->SCGC5 |= (SIM_SCGC5_PORT ## registerLetter ##_MASK);\
				setMode(type, fromConstructor);\
				setOptions(options, fromConstructor);\
			};\
			/* "Note: GPIO module is clocked by system clock." -- KL05 reference manual */\
			void setMode(const PinMode type, const bool fromConstructor=false) {\
				switch (type) {\
					case kOutput:\
						(*PORT ## registerLetter).PCR[registerPin] = \
							((*PORT ## registerLetter).PCR[registerPin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(kPinMuxGPIO >> kPinMuxOffset) ;\
						/* NO break -- fall through: */\
					case kPeripheralOutput:\
						(*FPT ## registerLetter).PDDR |= mask ;\
						break;\
					\
					case kInput:\
						(*PORT ## registerLetter).PCR[registerPin] = \
							((*PORT ## registerLetter).PCR[registerPin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(kPinMuxGPIO >> kPinMuxOffset) ;\
						/* NO break -- fall through: */\
					case kPeripheralInput:\
						(*FPT ## registerLetter).PDDR &= ~mask ;\
						break;\
					default:\
						break;\
				}\
			};\
			/* Note: getMode() does NOT check for peripheral modes. */\
			PinMode getMode() {\
				return ((*FPT ## registerLetter).PDOR & mask) ? kOutput : kInput;\
			};\
			void setOptions(const uint16_t options, const bool fromConstructor=false) {\
				if (kPullUp & options)\
				{\
					(*PORT ## registerLetter).PCR[registerPin] |= PORT_PCR_PE_MASK ;\
				}\
				else\
				{\
					(*PORT ## registerLetter).PCR[registerPin] &= ~PORT_PCR_PE_MASK ;\
				}\
				if (kDriveStrengthHigh & options)\
				{\
					(*PORT ## registerLetter).PCR[registerPin] |= PORT_PCR_DSE_MASK ;\
				}\
				else\
				{\
					(*PORT ## registerLetter).PCR[registerPin] &= ~PORT_PCR_DSE_MASK ;\
				}\
				if (kPinMuxSet & options)\
				{\
					(*PORT ## registerLetter).PCR[registerPin] = \
						((*PORT ## registerLetter).PCR[registerPin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX((options & kPinMuxMask) >> kPinMuxOffset) ;\
				}\
			};\
			uint32_t getOptions() {\
				return (((*PORT ## registerLetter).PCR[registerPin] & PORT_PCR_PE_MASK) ? kPullUp : 0)\
					| ( ( ((*PORT ## registerLetter).PCR[registerPin] & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT ) << kPinMuxOffset );\
			};\
			void set() {\
				(*FPT ## registerLetter).PSOR = mask ;\
			};\
			void clear() {\
				(*FPT ## registerLetter).PCOR = mask ;\
			};\
			void write(const bool value) {\
				if (!value)\
					clear();\
				else\
					set();\
			};\
			void toggle()  {\
				(*FPT ## registerLetter).PTOR = mask ;\
			};\
			uint32_t get() {\
				return /*(*FPT ## registerLetter).PDDR & mask ?  */ \
				/*	(*FPT ## registerLetter).PDOR & mask :*/ \
					(*FPT ## registerLetter).PDIR & mask ; \
			};\
			uint32_t getInputValue() {\
				return (*FPT ## registerLetter).PDIR & mask ; \
			};\
			uint32_t getOutputValue() {\
				return (*FPT ## registerLetter).PDOR & mask ; \
			};\
            void setInterrupts(const uint32_t interrupts) {\
                port ## registerLetter.setInterrupts(interrupts, mask);\
            };\
			bool isNull() { return false; };\
			static uint32_t maskForPort(const uint8_t otherPortLetter) {\
				return portLetter == otherPortLetter ? mask : 0x00u;\
			};\
            /* Placeholder for user code. */\
            static void interrupt() __attribute__ ((weak));\
			/* Motate internal use only: */\
			bool getInterruptStatus() {\
				return (*PORT ## registerLetter).PCR[registerPin] & PORT_PCR_ISF_MASK;\
			};\
			void clearInterruptStatus() {\
				(*PORT ## registerLetter).PCR[registerPin] = PORT_PCR_ISF_MASK;\
			};\
		}; /* template<> struct Pin<pinNum> */\
		typedef Pin<pinNum> Pin ## pinNum;\
		static Pin ## pinNum pin ## pinNum;\
        template<>\
        struct ReversePinLookup<registerChar, registerPin> : Pin<pinNum> {\
			ReversePinLookup() {};\
			ReversePinLookup(const PinMode type, const PinOptions options = kNormal) : Pin<pinNum>(type, options) {};\
        };


    

	static const uint32_t kDefaultPWMFrequency = 1000;
	template<int8_t pinNum>
	struct PWMOutputPin : Pin<pinNum> {
		PWMOutputPin() : Pin<pinNum>(kOutput) {};
		PWMOutputPin(const PinOptions options, const uint32_t freq = kDefaultPWMFrequency) : Pin<pinNum>(kOutput, options) {};
		PWMOutputPin(const uint32_t freq) : Pin<pinNum>(kOutput, kNormal) {};
		void setFrequency(const uint32_t freq) {};
		void operator=(const float value) { write(value); };
		void write(const float value) { Pin<pinNum>::write(value >= 0.5); };
		void writeRaw(const uint16_t duty) { Pin<pinNum>::write(duty >= 50); };
		uint16_t getTopValue() { return 100; };
		bool canPWM() { return false; };

		/*Override these to pick up new methods */

	private: /* Make these private to catch them early. */
		/* These are intentially not defined. */
		void init(const PinMode type, const PinOptions options = kNormal);

		/* WARNING: Covariant return types! */
		bool get();
		operator bool();
	};

	#define _MAKE_MOTATE_PWM_PIN(registerChar, registerPin, timerOrPWM, channel, pinmuxNum, invertedByDefault)\
		template<>\
		struct PWMOutputPin< ReversePinLookup<registerChar, registerPin>::number > : Pin< ReversePinLookup<registerChar, registerPin>::number >, timerOrPWM {\
			static const pin_number pinNum = ReversePinLookup<registerChar, registerPin>::number;\
			PWMOutputPin() :\
				Pin<pinNum>(kPeripheralOutput, kPinMux ## pinmuxNum), timerOrPWM(Motate::kTimerUpToMatch, kDefaultPWMFrequency)\
				{pwmpin_init(kNormal);};\
			\
			PWMOutputPin(const PinOptions options, const uint32_t freq = kDefaultPWMFrequency) :\
			Pin<pinNum>(kPeripheralOutput, kPinMux ## pinmuxNum), timerOrPWM(Motate::kTimerUpToMatch, kDefaultPWMFrequency)\
			{pwmpin_init(options);};\
			\
			PWMOutputPin(const uint32_t freq) :\
				Pin<pinNum>(kPeripheralOutput, kPinMux ## pinmuxNum), timerOrPWM(Motate::kTimerUpToMatch, freq)\
				{pwmpin_init(kNormal);};\
			\
			void pwmpin_init(const PinOptions options) {\
				timerOrPWM::setOutputOptions(channel, (invertedByDefault ^ ((options & kPWMPinInverted)?true:false)) ? kPWMOnInverted : kPWMOn);\
				timerOrPWM::start();\
			};\
			\
			void setFrequency(const uint32_t freq) {\
				timerOrPWM::setModeAndFrequency(Motate::kTimerUpToMatch, freq);\
				timerOrPWM::start();\
			};\
			void operator=(const float value) { write(value); };\
			void write(const float value) {\
				uint16_t duty = getTopValue() * value;\
				timerOrPWM::setExactDutyCycle(channel, duty);\
			};\
			void writeRaw(const uint16_t duty) {\
				timerOrPWM::setExactDutyCycle(channel, duty);\
			};\
			bool canPWM() { return true; };\
			/*Override these to pick up new methods */\
		private: /* Make these private to catch them early. */\
			/* These are intentially not defined. */\
			void init(const PinMode type, const PinOptions options = kNormal);\
			/* WARNING: Covariant return types! */\
			bool get();\
			operator bool();\
		};

    template<int8_t pinNum>
	struct SPIChipSelectPin {
		static const bool is_real = std::false_type::value;
	};

    template<int8_t pinNum>
	constexpr const bool IsSPICSPin() { return SPIChipSelectPin<pinNum>::is_real; };

    #define _MAKE_MOTATE_SPI_CS_PIN(registerChar, registerPin, pinmuxNum)\
        template<>\
        struct SPIChipSelectPin< ReversePinLookup<registerChar, registerPin>::number > : ReversePinLookup<registerChar, registerPin> {\
            SPIChipSelectPin() : ReversePinLookup<registerChar, registerPin>(kUnchanged, kPinMux ## pinmuxNum) {};\
            static const uint8_t moduleId = 0; \
			static const bool is_real = std::true_type::value;\
        };

    
    template<int8_t pinNum>
	struct SPIMISOPin {
		static const bool is_real = std::false_type::value;
	};

	template <int8_t pinNum>
	constexpr const bool IsSPIMISOPin() { return SPIMISOPin<pinNum>::is_real; };

    #define _MAKE_MOTATE_SPI_MISO_PIN(registerChar, registerPin, pinmuxNum)\
        template<>\
        struct SPIMISOPin< ReversePinLookup<registerChar, registerPin>::number > : ReversePinLookup<registerChar, registerPin> {\
            SPIMISOPin() : ReversePinLookup<registerChar, registerPin>(kInput, kPinMux ## pinmuxNum) {};\
            static const uint8_t moduleId = 0; \
			static const bool is_real = std::true_type::value;\
        };

    
    template<int8_t pinNum>
	struct SPIMOSIPin {
		static const bool is_real = std::false_type::value;
	};

	template <int8_t pinNum>
	constexpr const bool IsSPIMOSIPin() { return SPIMOSIPin<pinNum>::is_real; };

    #define _MAKE_MOTATE_SPI_MOSI_PIN(registerChar, registerPin, pinmuxNum)\
        template<>\
        struct SPIMOSIPin< ReversePinLookup<registerChar, registerPin>::number > : ReversePinLookup<registerChar, registerPin> {\
            SPIMOSIPin() : ReversePinLookup<registerChar, registerPin>(kOutput, kPinMux ## pinmuxNum) {};\
            static const uint8_t moduleId = 0; \
			static const bool is_real = std::true_type::value;\
        };

	
    
    template<int8_t pinNum>
	struct SPISCKPin {
		static const bool is_real = std::false_type::value;
	};

	template <int8_t pinNum>
	constexpr const bool IsSPISCKPin() { return SPISCKPin<pinNum>::is_real; };

    #define _MAKE_MOTATE_SPI_SCK_PIN(registerChar, registerPin, pinmuxNum)\
        template<>\
        struct SPISCKPin< ReversePinLookup<registerChar, registerPin>::number > : ReversePinLookup<registerChar, registerPin> {\
            SPISCKPin() : ReversePinLookup<registerChar, registerPin>(kOutput, kPinMux ## pinmuxNum) {};\
            static const uint8_t moduleId = 0; \
			static const bool is_real = std::true_type::value;\
        };

    

	#define _MAKE_MOTATE_PORT32(registerLetter, registerChar)\
		template <>\
		struct Port32<registerChar> {\
			static const uint8_t letter = (uint8_t) registerChar;\
			void setModes(const uintPort_t value, const uintPort_t mask) {\
				(*FPT ## registerLetter).PDDR = ((*FPT ## registerLetter).PDDR & ~mask) | value ;\
			};\
			void setOptions(const uint16_t options, const uintPort_t mask) {\
				uint16_t tempWriteValue = 0;\
				if (kPullUp & options)\
				{\
					tempWriteValue |= PORT_PCR_PE_MASK ;\
				}\
				if (kDriveStrengthHigh & options)\
				{\
					tempWriteValue |= PORT_PCR_DSE_MASK ;\
				}\
				if (kPinMuxSet & options)\
				{\
					tempWriteValue |= PORT_PCR_MUX((options & kPinMuxMask) >> kPinMuxOffset) ;\
				}\
				if (mask & 0x0000ffff) {\
					(*PORT ## registerLetter).GPCHR = PORT_GPCLR_GPWD(tempWriteValue) | PORT_GPCLR_GPWE(mask & 0x0000ffff);\
				}\
				if (mask & 0xffff0000) {\
					(*PORT ## registerLetter).GPCHR = PORT_GPCHR_GPWD(tempWriteValue) | PORT_GPCHR_GPWE((mask >> 16) & 0x0000ffff);\
				}\
			};\
			void set(const uintPort_t value) {\
				(*FPT ## registerLetter).PSOR = value ;\
			};\
			void clear(const uintPort_t value) {\
				(*FPT ## registerLetter).PCOR = value ;\
			};\
			void write(const uintPort_t value) {\
				(*FPT ## registerLetter).PDOR = value ;\
			};\
			void write(const uintPort_t value, const uintPort_t mask) {\
				(*FPT ## registerLetter).PDOR = ((*FPT ## registerLetter).PDOR & ~mask) | (value & ~mask) ;\
			};\
			uintPort_t getInputValues(const uintPort_t mask) {\
				return (*FPT ## registerLetter).PDIR & mask ; \
			};\
			uintPort_t getOutputValues(const uintPort_t mask) {\
				return (*FPT ## registerLetter).PDOR & mask ; \
			};\
            void setInterrupts(const uint32_t interrupts, const uintPort_t mask) {\
                if (mask != 0) {\
					/* We can't use the Global Port Control registers for interrupts */ \
					/* WARNING: This uses a gcc extension! */ \
					uintPort_t mask_temp = mask;\
					do {\
						uint32_t pinNum = __builtin_ctz(mask_temp);\
						mask_temp ^= 1 << pinNum; \
						(*PORT ## registerLetter).PCR[pinNum] = \
							((*PORT ## registerLetter).PCR[pinNum] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(interrupts & kPinInterruptTypeMask) ;\
					} while (mask_temp != 0); \
				}\
				if (interrupts != kPinInterruptsOff) {\
                    /* Set interrupt priority */\
                    if (interrupts & kPinInterruptPriorityMask) {\
                        if (interrupts & kPinInterruptPriorityHighest) {\
                            NVIC_SetPriority(PORT ## registerLetter ## _IRQn, 0);\
                        }\
                        else if (interrupts & kPinInterruptPriorityHigh) {\
                            NVIC_SetPriority(PORT ## registerLetter ## _IRQn, 3);\
                        }\
                        else if (interrupts & kPinInterruptPriorityMedium) {\
                            NVIC_SetPriority(PORT ## registerLetter ## _IRQn, 7);\
                        }\
                        else if (interrupts & kPinInterruptPriorityLow) {\
                            NVIC_SetPriority(PORT ## registerLetter ## _IRQn, 11);\
                        }\
                        else if (interrupts & kPinInterruptPriorityLowest) {\
                            NVIC_SetPriority(PORT ## registerLetter ## _IRQn, 15);\
                        }\
                    }\
                    /* Enable the IRQ */\
                    NVIC_EnableIRQ(PORT ## registerLetter ## _IRQn);\
                } else {\
					/* Ugly: We'd have to lop through all the pins of this port to make sure all interrupts are off. */\
					/* NVIC_DisableIRQ(PORT ## registerLetter ## _IRQn); */\
                }\
            };\
		};\
		typedef Port32<registerChar> Port ## registerLetter;\
		static Port ## registerLetter port ## registerLetter;

	typedef Pin<-1> NullPin;
	static NullPin nullPin;
    
} // end namespace Motate


#define MOTATE_PIN_INTERRUPT(number) \
    Pin<number>::interrupt_defined = true;\
    template<> void Pin<number>::interrupt()


// Note: We end the namespace before including in case the included file need to include
//   another Motate file. If it does include another Motate file, we end up with
//   Motate::Motate::* definitions and weird compiler errors.
#include <motate_pin_assignments.h>

namespace Motate {

// disable pinholder for Due for now -- nned to convert to 32bit
	// PinHolder - 32bit virtual ports (I've never made a template with 32 parameters before.)
	template<
		int8_t PinBit31num,
		int8_t PinBit30num = -1,
		int8_t PinBit29num = -1,
		int8_t PinBit28num = -1,
		int8_t PinBit27num = -1,
		int8_t PinBit26num = -1,
		int8_t PinBit25num = -1,
		int8_t PinBit24num = -1,
		int8_t PinBit23num = -1,
		int8_t PinBit22num = -1,
		int8_t PinBit21num = -1,
		int8_t PinBit20num = -1,
		int8_t PinBit19num = -1,
		int8_t PinBit18num = -1,
		int8_t PinBit17num = -1,
		int8_t PinBit16num = -1,
		int8_t PinBit15num = -1,
		int8_t PinBit14num = -1,
		int8_t PinBit13num = -1,
		int8_t PinBit12num = -1,
		int8_t PinBit11num = -1,
		int8_t PinBit10num = -1,
		int8_t PinBit9num  = -1,
		int8_t PinBit8num  = -1,
		int8_t PinBit7num  = -1,
		int8_t PinBit6num  = -1,
		int8_t PinBit5num  = -1,
		int8_t PinBit4num  = -1,
		int8_t PinBit3num  = -1,
		int8_t PinBit2num  = -1,
		int8_t PinBit1num  = -1,
		int8_t PinBit0num  = -1>
	class PinHolder32 {

		static Pin<PinBit31num> PinBit31;
		static Pin<PinBit30num> PinBit30;
		static Pin<PinBit29num> PinBit29;
		static Pin<PinBit28num> PinBit28;
		static Pin<PinBit27num> PinBit27;
		static Pin<PinBit26num> PinBit26;
		static Pin<PinBit25num> PinBit25;
		static Pin<PinBit24num> PinBit24;
		static Pin<PinBit23num> PinBit23;
		static Pin<PinBit22num> PinBit22;
		static Pin<PinBit21num> PinBit21;
		static Pin<PinBit20num> PinBit20;
		static Pin<PinBit19num> PinBit19;
		static Pin<PinBit18num> PinBit18;
		static Pin<PinBit17num> PinBit17;
		static Pin<PinBit16num> PinBit16;
		static Pin<PinBit15num> PinBit15;
		static Pin<PinBit14num> PinBit14;
		static Pin<PinBit13num> PinBit13;
		static Pin<PinBit12num> PinBit12;
		static Pin<PinBit11num> PinBit11;
		static Pin<PinBit10num> PinBit10;
		static Pin<PinBit9num>  PinBit9;
		static Pin<PinBit8num>  PinBit8;
		static Pin<PinBit7num>  PinBit7;
		static Pin<PinBit6num>  PinBit6;
		static Pin<PinBit5num>  PinBit5;
		static Pin<PinBit4num>  PinBit4;
		static Pin<PinBit3num>  PinBit3;
		static Pin<PinBit2num>  PinBit2;
		static Pin<PinBit1num>  PinBit1;
		static Pin<PinBit0num>  PinBit0;

	#define _MOTATE_PH32_CREATE_CLEAR_AND_COPY_MASKS(aPortLetter) \
		static const uint32_t port ## aPortLetter ## ClearMask =\
			(Pin<PinBit31num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit31num>::mask : 0x00) |\
			(Pin<PinBit30num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit30num>::mask : 0x00) |\
			(Pin<PinBit29num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit29num>::mask : 0x00) |\
			(Pin<PinBit28num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit28num>::mask : 0x00) |\
			(Pin<PinBit27num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit27num>::mask : 0x00) |\
			(Pin<PinBit26num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit26num>::mask : 0x00) |\
			(Pin<PinBit25num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit25num>::mask : 0x00) |\
			(Pin<PinBit24num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit24num>::mask : 0x00) |\
			(Pin<PinBit23num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit23num>::mask : 0x00) |\
			(Pin<PinBit22num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit22num>::mask : 0x00) |\
			(Pin<PinBit21num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit21num>::mask : 0x00) |\
			(Pin<PinBit20num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit20num>::mask : 0x00) |\
			(Pin<PinBit19num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit19num>::mask : 0x00) |\
			(Pin<PinBit18num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit18num>::mask : 0x00) |\
			(Pin<PinBit17num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit17num>::mask : 0x00) |\
			(Pin<PinBit16num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit16num>::mask : 0x00) |\
			(Pin<PinBit15num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit15num>::mask : 0x00) |\
			(Pin<PinBit14num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit14num>::mask : 0x00) |\
			(Pin<PinBit13num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit13num>::mask : 0x00) |\
			(Pin<PinBit12num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit12num>::mask : 0x00) |\
			(Pin<PinBit11num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit11num>::mask : 0x00) |\
			(Pin<PinBit10num>::portLetter == Port ## aPortLetter::letter ? Pin<PinBit10num>::mask : 0x00) |\
			(Pin<PinBit9num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit9num>::mask  : 0x00) |\
			(Pin<PinBit8num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit8num>::mask  : 0x00) |\
			(Pin<PinBit7num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit7num>::mask  : 0x00) |\
			(Pin<PinBit6num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit6num>::mask  : 0x00) |\
			(Pin<PinBit5num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit5num>::mask  : 0x00) |\
			(Pin<PinBit4num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit4num>::mask  : 0x00) |\
			(Pin<PinBit3num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit3num>::mask  : 0x00) |\
			(Pin<PinBit2num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit2num>::mask  : 0x00) |\
			(Pin<PinBit1num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit1num>::mask  : 0x00) |\
			(Pin<PinBit0num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit0num>::mask  : 0x00);\
\
		static const uint32_t port ## aPortLetter ## CopyMask =\
		(Pin<PinBit31num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit31num>::mask == 0x80000000u ? Pin<PinBit31num>::mask : 0x00) |\
		(Pin<PinBit30num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit30num>::mask == 0x40000000u ? Pin<PinBit30num>::mask : 0x00) |\
		(Pin<PinBit29num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit29num>::mask == 0x20000000u ? Pin<PinBit29num>::mask : 0x00) |\
		(Pin<PinBit28num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit28num>::mask == 0x10000000u ? Pin<PinBit28num>::mask : 0x00) |\
		(Pin<PinBit27num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit27num>::mask == 0x08000000u ? Pin<PinBit27num>::mask : 0x00) |\
		(Pin<PinBit26num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit26num>::mask == 0x04000000u ? Pin<PinBit26num>::mask : 0x00) |\
		(Pin<PinBit25num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit25num>::mask == 0x02000000u ? Pin<PinBit25num>::mask : 0x00) |\
		(Pin<PinBit24num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit24num>::mask == 0x01000000u ? Pin<PinBit24num>::mask : 0x00) |\
		(Pin<PinBit23num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit23num>::mask == 0x00800000u ? Pin<PinBit23num>::mask : 0x00) |\
		(Pin<PinBit22num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit22num>::mask == 0x00400000u ? Pin<PinBit22num>::mask : 0x00) |\
		(Pin<PinBit21num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit21num>::mask == 0x00200000u ? Pin<PinBit21num>::mask : 0x00) |\
		(Pin<PinBit20num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit20num>::mask == 0x00100000u ? Pin<PinBit20num>::mask : 0x00) |\
		(Pin<PinBit19num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit19num>::mask == 0x00080000u ? Pin<PinBit19num>::mask : 0x00) |\
		(Pin<PinBit18num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit18num>::mask == 0x00040000u ? Pin<PinBit18num>::mask : 0x00) |\
		(Pin<PinBit17num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit17num>::mask == 0x00020000u ? Pin<PinBit17num>::mask : 0x00) |\
		(Pin<PinBit16num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit16num>::mask == 0x00010000u ? Pin<PinBit16num>::mask : 0x00) |\
		(Pin<PinBit15num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit15num>::mask == 0x00008000u ? Pin<PinBit15num>::mask : 0x00) |\
		(Pin<PinBit14num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit14num>::mask == 0x00004000u ? Pin<PinBit14num>::mask : 0x00) |\
		(Pin<PinBit13num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit13num>::mask == 0x00002000u ? Pin<PinBit13num>::mask : 0x00) |\
		(Pin<PinBit12num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit12num>::mask == 0x00001000u ? Pin<PinBit12num>::mask : 0x00) |\
		(Pin<PinBit11num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit11num>::mask == 0x00000800u ? Pin<PinBit11num>::mask : 0x00) |\
		(Pin<PinBit10num>::portLetter == Port ## aPortLetter::letter && Pin<PinBit10num>::mask == 0x00000400u ? Pin<PinBit10num>::mask : 0x00) |\
		(Pin<PinBit9num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit9num>::mask  == 0x00000200u ? Pin<PinBit9num>::mask  : 0x00) |\
		(Pin<PinBit8num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit8num>::mask  == 0x00000100u ? Pin<PinBit8num>::mask  : 0x00) |\
		(Pin<PinBit7num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit7num>::mask  == 0x00000080u ? Pin<PinBit7num>::mask  : 0x00) |\
		(Pin<PinBit6num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit6num>::mask  == 0x00000040u ? Pin<PinBit6num>::mask  : 0x00) |\
		(Pin<PinBit5num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit5num>::mask  == 0x00000020u ? Pin<PinBit5num>::mask  : 0x00) |\
		(Pin<PinBit4num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit4num>::mask  == 0x00000010u ? Pin<PinBit4num>::mask  : 0x00) |\
		(Pin<PinBit3num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit3num>::mask  == 0x00000008u ? Pin<PinBit3num>::mask  : 0x00) |\
		(Pin<PinBit2num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit2num>::mask  == 0x00000004u ? Pin<PinBit2num>::mask  : 0x00) |\
		(Pin<PinBit1num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit1num>::mask  == 0x00000002u ? Pin<PinBit1num>::mask  : 0x00) |\
		(Pin<PinBit0num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit0num>::mask  == 0x00000001u ? Pin<PinBit0num>::mask  : 0x00);

		_MOTATE_PH32_CREATE_CLEAR_AND_COPY_MASKS(A);
		_MOTATE_PH32_CREATE_CLEAR_AND_COPY_MASKS(B);
#ifdef PIOC
		_MOTATE_PH32_CREATE_CLEAR_AND_COPY_MASKS(C);
#endif
#ifdef PIOD
		_MOTATE_PH32_CREATE_CLEAR_AND_COPY_MASKS(D);
#endif
	public:
		PinHolder32() {
			
		};
		
		void write(uint32_t in_value) {
			uint32_t port_value    = 0x00; // Port<> handles reading the port and setting the masked pins
#define _MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, bitNumber, bitMask) \
			if (PinBit ## bitNumber.maskForPort(Port ## portLetter::letter) &&\
					(PinBit ## bitNumber.mask != (bitMask)) && (in_value & (bitMask))) {\
				port_value |= PinBit ## bitNumber.mask;\
			}

// Using direct 0x00000000 notation instead of 1<<x, since the compiler occasionally won't precompile that.
// Shortcut: ruby -e '(0..31).each() { |x| print "_MOTATE_PINHOLDER_CHECKANDSETPIN(portLetter, %2d, 0x%08x);\\\n" % [31-x, (1<<(31-x))]}'
#define _MOTATE_PH32_PINHOLDER_SETPORT(portLetter) \
			if (port ## portLetter ## ClearMask != 0x00) {\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 31, 0x80000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 30, 0x40000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 29, 0x20000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 28, 0x10000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 27, 0x08000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 26, 0x04000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 25, 0x02000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 24, 0x01000000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 23, 0x00800000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 22, 0x00400000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 21, 0x00200000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 20, 0x00100000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 19, 0x00080000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 18, 0x00040000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 17, 0x00020000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 16, 0x00010000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 15, 0x00008000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 14, 0x00004000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 13, 0x00002000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 12, 0x00001000u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 11, 0x00000800u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter, 10, 0x00000400u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  9, 0x00000200u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  8, 0x00000100u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  7, 0x00000080u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  6, 0x00000040u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  5, 0x00000020u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  4, 0x00000010u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  3, 0x00000008u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  2, 0x00000004u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  1, 0x00000002u);\
				_MOTATE_PH32_PINHOLDER_CHECKANDSETPIN(portLetter,  0, 0x00000001u);\
				port_value |= in_value & port ## portLetter ## CopyMask;\
				port ## portLetter.write(port_value, ~port ## portLetter ## ClearMask);\
			}
			
			_MOTATE_PH32_PINHOLDER_SETPORT(A);
			_MOTATE_PH32_PINHOLDER_SETPORT(B);
#ifdef PIOC
			_MOTATE_PH32_PINHOLDER_SETPORT(C);
#endif
#ifdef PIOD
			_MOTATE_PH32_PINHOLDER_SETPORT(D);
#endif
		}
	};
	
	// disable pinholder for Due for now -- nned to convert to 32bit
	// PinHolder - 32bit virtual ports (I've never made a template with 32 parameters before.)
	template<
		int8_t PinBit7num,
		int8_t PinBit6num  = -1,
		int8_t PinBit5num  = -1,
		int8_t PinBit4num  = -1,
		int8_t PinBit3num  = -1,
		int8_t PinBit2num  = -1,
		int8_t PinBit1num  = -1,
		int8_t PinBit0num  = -1>
	class PinHolder8 {

		static Pin<PinBit7num>  PinBit7;
		static Pin<PinBit6num>  PinBit6;
		static Pin<PinBit5num>  PinBit5;
		static Pin<PinBit4num>  PinBit4;
		static Pin<PinBit3num>  PinBit3;
		static Pin<PinBit2num>  PinBit2;
		static Pin<PinBit1num>  PinBit1;
		static Pin<PinBit0num>  PinBit0;
	public:
#define _MOTATE_PH8_CREATE_CLEAR_AND_COPY_MASKS(aPortLetter) \
		static const uint32_t port ## aPortLetter ## ClearMask =\
			(Pin<PinBit7num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit7num>::mask  : 0x00u) |\
			(Pin<PinBit6num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit6num>::mask  : 0x00u) |\
			(Pin<PinBit5num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit5num>::mask  : 0x00u) |\
			(Pin<PinBit4num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit4num>::mask  : 0x00u) |\
			(Pin<PinBit3num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit3num>::mask  : 0x00u) |\
			(Pin<PinBit2num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit2num>::mask  : 0x00u) |\
			(Pin<PinBit1num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit1num>::mask  : 0x00u) |\
			(Pin<PinBit0num>::portLetter  == Port ## aPortLetter::letter ? Pin<PinBit0num>::mask  : 0x00u);\
\
		static const uint32_t port ## aPortLetter ## CopyMask =\
		(Pin<PinBit7num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit7num>::mask  == 0x00000080u ? Pin<PinBit7num>::mask  : 0x00u) |\
		(Pin<PinBit6num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit6num>::mask  == 0x00000040u ? Pin<PinBit6num>::mask  : 0x00u) |\
		(Pin<PinBit5num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit5num>::mask  == 0x00000020u ? Pin<PinBit5num>::mask  : 0x00u) |\
		(Pin<PinBit4num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit4num>::mask  == 0x00000010u ? Pin<PinBit4num>::mask  : 0x00u) |\
		(Pin<PinBit3num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit3num>::mask  == 0x00000008u ? Pin<PinBit3num>::mask  : 0x00u) |\
		(Pin<PinBit2num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit2num>::mask  == 0x00000004u ? Pin<PinBit2num>::mask  : 0x00u) |\
		(Pin<PinBit1num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit1num>::mask  == 0x00000002u ? Pin<PinBit1num>::mask  : 0x00u) |\
		(Pin<PinBit0num>::portLetter  == Port ## aPortLetter::letter && Pin<PinBit0num>::mask  == 0x00000001u ? Pin<PinBit0num>::mask  : 0x00u);

		_MOTATE_PH8_CREATE_CLEAR_AND_COPY_MASKS(A);
		_MOTATE_PH8_CREATE_CLEAR_AND_COPY_MASKS(B);
#ifdef PIOC
		_MOTATE_PH8_CREATE_CLEAR_AND_COPY_MASKS(C);
#endif
#ifdef PIOD
		_MOTATE_PH8_CREATE_CLEAR_AND_COPY_MASKS(D);
#endif
	public:
		PinHolder8() {

		};

		void write(uint8_t in_value) {
			uint32_t port_value = 0; // Port<> handles reading the port and setting the masked pins
			#define _MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter, bitNumber, bitMask) \
				if (PinBit ## bitNumber.maskForPort(Port ## portLetter::letter) &&\
						(PinBit ## bitNumber.mask != (bitMask)) && ((uint32_t)in_value & (bitMask))) {\
					port_value |= PinBit ## bitNumber.mask;\
				}

			// Using direct 0x00000000 notation instead of 1<<x, since the compiler occasionally won't precompile that.
			// Shortcut: ruby -e '(0..7).each() { |x| print "_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter, %2d, 0x%02x);\\\n" % [7-x, (1<<(7-x))]}'
			#define _MOTATE_PH8_PINHOLDER_SETPORT(portLetter) \
				if (port ## portLetter ## ClearMask) {\
					port_value = 0;\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  7, 0x00000080u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  6, 0x00000040u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  5, 0x00000020u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  4, 0x00000010u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  3, 0x00000008u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  2, 0x00000004u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  1, 0x00000002u);\
					_MOTATE_PH8_PINHOLDER_CHECKANDSETPIN(portLetter,  0, 0x00000001u);\
					port_value |= (uint32_t)in_value & port ## portLetter ## CopyMask;\
					port ## portLetter.write(port_value, port ## portLetter ## ClearMask);\
				}

			_MOTATE_PH8_PINHOLDER_SETPORT(A);
			_MOTATE_PH8_PINHOLDER_SETPORT(B);
#ifdef PIOC
			_MOTATE_PH8_PINHOLDER_SETPORT(C);
#endif
#ifdef PIOD
			_MOTATE_PH8_PINHOLDER_SETPORT(D);
#endif
		}
	};
}
#endif /* end of include guard: KL05ZPINS_H_ONCE */
