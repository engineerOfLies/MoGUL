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
 * @brief load a tilemap from a definition dictionary
 * @param def the definition contain tilemap data
 * @return NULL on error or the loaded tilemap otherwise
 */
MglTileMap *mgl_tilemap_load_from_def(MglDict *def);


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
 * @param color the color to adjust the layer to
 */
void mgl_tilemap_draw(MglTileMap *tilemap, MglVec2D position,MglVec4D color);

/**
 * @brief get the index of the tile found at the position provided
 * NOTE: a Zero index implies no tile.  this (index - 1) maps to the tile set
 * @param map the tile mape to query
 * @param position the position to check
 * @return -1 on error or the tile index otherwise
 */
MglInt mgl_tilemap_get_tile_index_by_position(MglTileMap *map,MglVec2D position);

/**
 * @brief Check if the position in the tilemap is considered solid.
 * NOTE: this should be absolute position, not tile number
 * @param map the tilemap to query
 * @param position the position to check
 * @return MglTrue if the tile is solid or if the tile is off the map, MglFalse if there is no tile or the tile is not solid
 */
MglBool mgl_tilemap_get_tile_solid_by_position(MglTileMap *map,MglVec2D position);

/**
 * @brief Check if the tile position in the tilemap is considered solid.
 * NOTE: this should be the tile number, not absolute position
 * @param map the tilemap to query
 * @param position the position to check
 * @return MglTrue if the tile is solid or if the tile is off the map, MglFalse if there is no tile or the tile is not solid
 */
MglBool mgl_tilemap_get_tile_solid_by_tile_position(MglTileMap *map,MglVec2D position);

/**
 * @brief get the data definition dictionary associated with the tile a the map position
 * NOTE: Tiles might now have data associated with them
 * @param map the tilemap to query
 * @param position the position to check
 * @return NULL if not found or error.  A pointer to a diction IF there is data associated with the tile
 */
MglDict * mgl_tilemap_get_tile_info_by_position(MglTileMap *map,MglVec2D position);

#endif
