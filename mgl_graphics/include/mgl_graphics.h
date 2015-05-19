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
#endif
