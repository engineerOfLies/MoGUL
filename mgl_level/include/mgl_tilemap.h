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
#include "mgl_tileset.h"

/**
 * @brief the MglTileMap holds information about the layout of the tiles that make up an individual tile level
 */


typedef struct MglTileMap_S MglTileMap;


/**
 * @brief initialize tile map system
 * @param maxMaps how many tilemaps you want to support in memory at the same time
 * @param cacheSurface if true, tilemap data is cached in a surface, otherwise only to texture
 */
void mgl_tilemap_init(
    MglUint maxMaps,
    MglBool cacheSurface);


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

/**
 * @brief create a new empty tilemap
 * @param tileset if provided, this will tile set will be associated with the map
 * @param width create the tilemap this many tiles wide
 * @param height create the tilemap this many tiles high
 * @return NULL on error or a new blank tilemap
 */
MglTileMap *mgl_tilemap_new(
    MglTileSet *tileset,
    MglUint     width,
    MglUint     height);


/**
 * @brief rebuild tilemap rendering surface / texture based on current tile data
 * This process may take some time, so it is best not to do it unless tile data has changed.
 * @param tilemap the tile map to render
 */
void mgl_tilemap_render(MglTileMap *tilemap);

/**
 * @brief draw the pre-rendered tile map to the current rendering context
 * @param tilemap the tilemap to draw
 * @param position where to draw the tilemap to
 */
void mgl_tilemap_draw(MglTileMap *tilemap, MglVec2D position);

#endif
