#ifndef __MGL_TILEMAP_H__
#define __MGL_TILEMAP_H__
/**
 * mgl_tilemap
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */

#include "mgl_types.h"
#include "mgl_vector.h"
#include "mgl_dict.h"

/**
 * @brief the MglTileMap holds information about the layout of the tiles that make up an individual tile level
 */


typedef struct MglTileMap_S MglTileMap;

/**
 * @brief free the loaded tilemap
 * @param tileset a pointer to your tilemap pointer, set to NULL on completion
 */
void mgl_tilemap_free(MglTileMap **tilemap);

/**
 * @brief load a tilemap from a config file
 * @param filename the config file to load
 * @return NULL on error or the loaded tilemap otherwise
 */
MglTileMap *mgl_tilemap_load(char * filename);

#endif
