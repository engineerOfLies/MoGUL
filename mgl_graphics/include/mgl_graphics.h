#ifndef __MGL_GRAPHICS_H__
#define __MGL_GRAPHICS_H__

/**
 * mgl_graphics
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
 * @purpose mgl_graphics initializes and configures rendering pipleline and provides an interface to 
 * do basic drawing and frame timing.
 */

/**
 * @brief initializes the mgl sub systems and sets up the video system based on the parameters passed
 *
 * @param windowName the name for the main game window
 * @param viewWidth the screen width, if -1 defaults to system width 
 * @param viewHeight the screen height, if -1 defaults to system 
 * @param renderWidth the desired functional width of the game screen
 * @param renderHeight the desired functional height of the game screen
 * @param bgcolor the default background color
 * @param fullscreen if true, initialize in fullscreen mode, otherwise you get windowed mode
 * @return 0 on success, otherwise an error.  See the logs for info
 */

void mgl_graphics_init(
    char *windowName,
    MglInt viewWidth,
    MglInt viewHeight,
    MglInt renderWidth,
    MglInt renderHeight,
    MglVec3D bgcolor,
    MglBool fullscreen
);

/**
 * @brief Initialize the graphics system with the config file specified
 * Uses the config file loaded to initialize the graphics system.  If the file fails to load,
 * the graphics system will not initialize
 * 
 * @param configFile the file path to the config file to loaded.  Must be supported by mgl_config
 * @return 0 on success, otherwise an error.  See the logs for info
 */
int mgl_graphics_init_by_config(char *configFile);

/**
 * @brief return a pointer to the active main game screen rendering surface.
 * 
 * @return NULL on error, or a pointer to the sdl surface that will be copied to the screen on next render
 */
SDL_Surface *mgl_graphics_get_screen_surface();

/**
 * @brief create a new SDL surface that is compatable with the screen surface
 * It should be freed with SDL_FreeSurface
 * @param w the width for the new surface
 * @param h the height for the new surface
 * @return NULL on error or a newly created SDL Surface.
 */
SDL_Surface *mgl_graphics_create_surface(MglUint w,MglUint h);

/**
 * @brief copy one surface on the current screen surface
 * @param surface the source of the copy
 * @param srcRect the clip rectangle for the source surface
 * @param dstRect the clip rectangle for the destination surface (the sceen)
 */
void mgl_graphics_blit_surface_to_screen(SDL_Surface *surface,const MglRect * srcRect,const MglRect * dstRect);

/**
 * @brief renders the contents of the screen buffer to the physical screen.  Internally waits to 
 * ensure a steady frame rate based on the frameDelay configured
 */
void mgl_grahics_next_frame();

/**
 * @brief set the internal frame delay to wait between rendering frames.  This is to ensure a steady frame rate.
 */
void mgl_graphics_set_frame_delay(MglUint frameDelay);

/**
 * @brief get the rendering time for the current game timestamp
 * 
 * @return the number of ticks that have passed since the start of the program and the last rendered frame.
 */
MglUint mgl_graphics_get_render_time();

/**
 * @brief clears the rendering screen with the default color or background image if it is set
 */
void mgl_graphics_clear_screen();

/**
 * @brief set the default background color when clearing the screen
 * 
 * @param bgcolor the color vector (r,g,b)
 */
void mgl_graphics_set_bgcolor(MglVec3D bgcolor);

/**
 * @brief convert a component color vector (r,g,b,a) to the SDL color expected by the surface provided
 * 
 * @param surface the surface that the color will be made compatable for
 * @param color the component vector where x=>r, y=>g, z=>b, w=>a.
 * @return an SDL color that will work with the surface provided
 */
MglUint mgl_graphics_vec_to_surface_color(SDL_Surface *surface,MglVec4D color);

/**
 * @brief convert a component color vector (r,g,b,a) to the SDL color expected by the loaded screen surface
 * 
 * @param color the component vector where x=>r, y=>g, z=>b, w=>a.
 * @return an SDL color that will work with the screen surface
 */
MglUint mgl_graphics_vec_to_screen_color(MglVec4D color);

/**
 * @brief get the currect render screen surface resolution
 * 
 * @param w [output] if provided, this will be populated with the screen's width
 * @param h [output] if provided, this will be populated with the screen's height
 * 
 * @return 0 on success, other on error
 */
MglInt mgl_graphics_get_screen_resolution(MglUint *w,MglUint *h);

#endif
