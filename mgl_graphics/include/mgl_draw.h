#ifndef __MGL_DRAW_H__
#define __MGL_DRAW_H__

/**
 * mgl_draw
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
#include "mgl_vector.h"
#include "mgl_rect.h"
#include <SDL.h>

/**
 * @purpose provides drawing of shape primitives and integration with Chipmunk physics engine.
 */


/**
 * @brief draws a pixel to the current screen surface
 * @param point the coordinates to draw the pixel
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_pixel(MglVec2D point,MglVec4D color);

/**
 * @brief draws a pixel to the specified surface
 * @param surface the SDL surface to draw to
 * @param point the coordinates to draw the pixel
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_pixel_to_surface(SDL_Surface *surface,MglVec2D point,MglVec4D color);

/**
 * @brief draws a filled rectangle to the screen
 * @param point the dimensions of the axis aligned rectangle
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_solid_rect(MglRect rect, MglVec4D color);

/**
 * @brief draws a filled rectangle to the specified surface
 * @param surface the SDL surface to draw to
 * @param point the dimensions of the axis aligned rectangle
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_solid_rect_to_surface(SDL_Surface *surface,MglRect rect, MglVec4D color);

/**
 * @brief draws a hollow rectangle to the screen
 * @param point the dimensions of the axis aligned rectangle
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_rect(MglRect rect, MglVec4D color);

/**
 * @brief draws a hollow rectangle to the specified surface
 * @param surface the SDL surface to draw to
 * @param point the dimensions of the axis aligned rectangle
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_rect_to_surface(SDL_Surface *surface,MglRect rect, MglVec4D color);

/**
 * @brief draw a line segment to the screen
 * @param p1 the first end point of the line to draw
 * @param p2 the second end point of the line to draw
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_line(MglVec2D p1,MglVec2D p2,MglVec4D color);

/**
 * @brief draw a line segment to a surface
 * @param surface the SDL surface to draw to
 * @param p1 the first end point of the line to draw
 * @param p2 the second end point of the line to draw
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_line_to_surface(SDL_Surface *surface,MglVec2D p1,MglVec2D p2,MglVec4D color);

/**
 * @brief draw a hollow circle to the screen
 * @param center the center point to draw the circle
 * @param radiua the radius of the cirlce
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_circle(MglVec2D center, int radius, MglVec4D color);

/**
 * @brief draw a hollow circle to the surface
 * @param surface the SDL surface to draw to
 * @param center the center point to draw the circle
 * @param radiua the radius of the cirlce
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_circle_to_surface(SDL_Surface *surface,MglVec2D center, int radius, MglVec4D color);

/**
 * @brief draw a filled circle to the screen
 * @param center the center point to draw the circle
 * @param radiua the radius of the cirlce
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_solid_circle(MglVec2D center, int radius, MglVec4D color);

/**
 * @brief draw a solid circle to the surface
 * @param surface the SDL surface to draw to
 * @param center the center point to draw the circle
 * @param radiua the radius of the cirlce
 * @param color the color (including alpha) to draw with
 */
void mgl_draw_solid_circle_to_surface(SDL_Surface *surface,MglVec2D center, int radius, MglVec4D color);

#endif
