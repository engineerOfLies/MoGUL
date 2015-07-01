#ifndef __MGL_TILESET_H__
#define __MGL_TILESET_H__
/**
 * mgl_tileset
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
 * @brief the MglTileSet holds information about tiles that will appear in a tile map.
 * The tileset is defined by a config file that details what image, sprite or actor is used
 * to draw the sprite.  Additional information can be stored with a tile type or the map as a whole
 */

typedef struct MglTileSet_S MglTileSet;

/**
 * @brief initialized the tileset manager
 * @param maxSets how many simultaneous tiles to support
 */
void mgl_tileset_init(
    MglUint maxSets);


/**
 * @brief free the loaded tileset
 * @param tileset a pointer to your tileset pointer, set to NULL on completion
 */
void mgl_tileset_free(MglTileSet **tileset);

/**
 * @brief load a tileset from a config file
 * @param filename the config file to load
 * @return NULL on error or the loaded tileset otherwise
 */
MglTileSet *mgl_tileset_load(char * filename);

/**
 * @brief draws a tile to the provided surface
 * @param tileset the tileset to draw from
 * @param index which tile number to draw
 * @param surface which surface to draw to
 * @param position where to draw the tile
 */
void mgl_tileset_draw_tile_to_surface(MglTileSet *tileset,MglUint index,SDL_Surface *surface,MglVec2D position);

/**
 * @brief draws a tile to the current rendering context
 * @param tileset the tileset to draw from
 * @param index which tile number to draw
 * @param position where to draw the tile
 */
void mgl_tileset_draw_tile(MglTileSet *tileset,MglUint index,MglVec2D position);

/**
 * @brief get the custom data associated with a tile set.
 * NOTE: You get a handle to the data, do not delete it or you will lose it
 * @param set the tileset to get the info for
 * @return NULL if not available or bad tile, the Dictionary otherwise
 */
MglDict *mgl_tileset_get_custom_info(MglTileSet *set);

/**
 * @brief get the custom data associated with a tile set.
 * NOTE: You get a handle to the data, do not delete it or you will lose it
 * @param set the tileset to get the info from
 * @param index the tile to get the info for
 * @return NULL if not available or bad tile, the Dictionary otherwise
 */
MglDict *mgl_tileset_get_tile_custom_info(MglTileSet *set,MglUint index);

/**
 * @brief check if a tile is solid or not from the tile set
 * @param set the tileset to get the info from
 * @param index the tile to get the info for
 * @return MglFalse if not solid or not found, MglTrue if solid
 */
MglBool mgl_tileset_tile_solid(MglTileSet *set,MglUint index);

/**
 * @brief get the tile size for a given tile map
 * @param set the tile set to check
 * @param size [output] the tile size of the tile map in pixels
 */
void mgl_tileset_get_tile_size(MglTileSet *set, MglVec2D *size);

#endif
