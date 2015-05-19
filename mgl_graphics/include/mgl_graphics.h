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
/**
 * @brief initializes the mgl sub systems and sets up the video system based on the parameters passed
 *
 * @param windowName the name for the main game window
 * @param width the screen width, if -1 defaults to system width 
 * @param height the screen height, if -1 defaults to system 
 * @param fullscreen if true, initialize in fullscreen mode, otherwise you get windowed mode
 * @return 0 on success, otherwise an error.  See the logs for info
 */

void mgl_graphics_init(
    char *windowName,
    MglInt width,
    MglInt height,
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
#endif
