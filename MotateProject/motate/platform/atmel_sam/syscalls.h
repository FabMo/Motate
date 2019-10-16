/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
  * \file syscalls.h
  *
  * Implementation of newlib syscall.
  *
  */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

// These aren't need for all-at-once compiling, and cause evil warnings

#ifndef SINGLE_TRANSLATION_BUILD

#define IS_USED  __attribute__ ((used));

extern caddr_t _sbrk( int incr ) IS_USED;

extern int link( char *cOld, char *cNew ) IS_USED;

extern int _close( int file ) IS_USED;

extern int _fstat( int file, struct stat *st ) IS_USED;

extern int _isatty( int file ) IS_USED;

extern int _lseek( int file, int ptr, int dir ) IS_USED;

extern int _read(int file, char *ptr, int len) IS_USED;

extern int _write( int file, char *ptr, int len ) IS_USED;

#undef IS_USED

#endif // SINGLE_TRANSLATION_BUILD


#ifdef __cplusplus
}
#endif

