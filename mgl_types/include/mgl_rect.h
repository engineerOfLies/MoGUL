#ifndef __MGL_RECT_H__
#define __MGL_RECT_H__
/**
 * mgl_rect
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

#include "mgl_types.h"

/**
 * @brief MglRect is an alias for SDL_Rect
 */
typedef SDL_Rect MglRect;

/**
 * @brief constructs and returns an MglRect
 * @param x component of the rect
 * @param y component of the rect
 * @param w component of the rect
 * @param h component of the rect
 */
MglRect mgl_rect(
  MglInt  x,
  MglInt  y,
  MglUint w,
  MglUint h
);

/**
 * @brief copies rect data from s into d
 *
 * @param d pointer to destination rect
 * @param s source rect
 */
void mgl_rect_copy(
  MglRect * d,
  MglRect   s
);

typedef struct
{
  MglDouble x,y,w,h;
}MglRectFloat;

/**
 * @brief constructs and returns an MglRectFloat
 * @param x component of the rect
 * @param y component of the rect
 * @param w component of the rect
 * @param h component of the rect
 */
MglRectFloat mgl_rectf(
  MglDouble  x,
  MglDouble  y,
  MglDouble  w,
  MglDouble  h
);


/**
 * @brief copies float rect data from s into d
 *
 * @param d pointer to destination rect
 * @param s source rect
 */
void mgl_rectf_copy(
  MglRectFloat * d,
  MglRectFloat   s
);

void mgl_rect_set(
  MglRect * r,
  MglInt    x,
  MglInt    y,
  MglUint   w,
  MglUint   h
);

void mgl_rectf_set(
  MglRectFloat * d,
  MglFloat       x,
  MglFloat       y,
  MglFloat       w,
  MglFloat       h
);

/**
 * @brief checks if the two rects are overlapping
 * @param a one of the rects to check
 * @param b the other rect to check
 * @return MglTrue if they overlap, MglFalse otherwise
 */
MglBool mgl_rect_lap_rect(MglRect a, MglRect b);

/**
 * @brief checks if the second rect is inside the first rect.
 * @param a outer rect
 * @param b inner rect
 * @return MglTrue if b is contained by a, MglFalse if any part of b lies outside of a
 */
MglBool mgl_rect_in_rect(MglRect a, MglRect b);


#endif
