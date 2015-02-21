#ifndef __MGL_TYPES_H__
#define __MGL_TYPES_H__

/**
 * mgl_types
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
#include <SDL.h>


#define MGL_EPSILON   1e-6f

#define MGL_2PI 6.283185308

/* conversion factor for converting from radians to degrees*/
#define MGL_RADTODEG  57.295779513082

/* conversion factor for converting from degrees to radians*/
#define MGL_DEGTORAD  0.017453292519943295769236907684886
/* 
 * The Integers
 * the SDL suit handles cross platform integers very well, so we are just going to alias them
 */
typedef Uint8    MglUI8;
typedef Sint8    MglSI8;
typedef Uint32   MglUI32;
typedef Sint32   MglSI32;
typedef Uint64   MglUI64;
typedef Sint64   MglSI64;

typedef MglSI32  MglInt;
typedef MglUI32  MglUint;

typedef MglUI8   MglBool;

enum MglBoolEnum {
  MglTrue  = 1,
  MglFalse = 0
  
};


#endif
