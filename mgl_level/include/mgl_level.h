#ifndef __MGL_LEVEL_H__
#define __MGL_LEVEL_H__
/**
 * mgl_level
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
#include "mgl_layer.h"
#include "mgl_camera.h"
#include "mgl_tilemap.h"
#include "mgl_parallax.h"

/**
 * @brief the level structure keeps a context for rendering a single game level.
 * Levels support layering of parallax contexts, tile maps, collision spaces and flat images.
 */
typedef struct MglLevel_S MglLevel;

/**
 * @brief initialize the level resource manager and configure it
 * Must be run before the rest of the level functions are used.
 * @param maxLevels how many concurrent layers you wish to support
 * @param defaultCamera if specified, use this camera as default
 */
void mgl_level_init(
    MglUint maxLevels,
    MglCamera *defaultCamera);

/**
 * @brief create a new level, initialized to zero.
 * @return NULL on error or out of resources, a level pointer otherwise
 */
MglLevel *mgl_level_new();

/**
 * @brief free a level resource
 * @param level a pointer to the level pointer to free.
 */
void mgl_level_free(MglLevel **level);

/**
 * @brief load a level from the file provided
 * @param the filename to load
 * @return NULL on error, or the loaded file otherwise
 */
MglLevel *mgl_level_load(char *filename);

/**
 * @brief draw the level
 * @param level the level to draw
 */
void mgl_level_draw(MglLevel *level);

#endif
