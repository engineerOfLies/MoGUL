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

/**
 * @brief get a level layer by name
 * @param level the level to get the layer from
 * @param name the name of the layer to find
 * @return NULL on error or not found, a layer pointer otherwise
 */
MglLayer *mgl_level_get_layer_by_name(MglLevel *level,MglLine name);

/**
 * @brief register a custom draw function for the draw list layer of the level
 * @param level the level to set the layer information for
 * @param name the name of the layer to set the callback for
 * @param cb the draw function.  this function will receive the layer pointer as the context and MglLevelParallaxContext as the data.
 *  The user data will be a provided though the context.data
 */
void mgl_level_register_list_draw_function(MglLevel *level,MglLine name, MglCallback draw);
void mgl_level_register_list_think_function(MglLevel *level,MglLine name, MglCallback draw);
void mgl_level_register_list_update_function(MglLevel *level,MglLine name, MglCallback draw);
void mgl_level_register_list_preprocess_function(MglLevel *level,MglLine name, MglCallback draw);
void mgl_level_register_list_postprocess_function(MglLevel *level,MglLine name, MglCallback draw);

/**
 * @brief add a draw item to the level layer.  This should be the same type of data in the rest of the layer, though no type checking will be done
 * NOTE: items are not considered to be owned by the draw layer and will not be cleaned up by the layer should it be deleted.
 * @param level the level to add to.
 * @param layername the draw layer to add too.  If the layer is not a MglLayerDrawList or is not found, this will be a no-op
 * @param item the item to add to the list, if this is NULL the function will be a no-op
 */
void mgl_level_append_draw_item_to_layer(MglLevel *level,MglLine layername,void *item);

/**
 * @brief get the tilemap associated with the level layer
 * @param level the level to get the layer information for
 * @param name the name of the layer to get the tilemap from
 * @return NULL on error or not a tilemap layer or not found.  The tilemap pointer otherwise
 */
MglTileMap *mgl_level_get_layer_tilemap_by_name(MglLevel *level,MglLine name);

/**
 * @brief get the collision space associated with the level layer
 * @param level the level to get the layer information for
 * @param name the name of the layer to get the collision space from
 * @return NULL on error or not a collision layer or not found.  The collision space pointer otherwise
 */
MglCollision *mgl_level_get_layer_collision_by_name(MglLevel *level,MglLine name);

/**
 * @brief add the tilemap solid data into the collision layer
 * @param tileLayer the tilemap to add
 * @param layerName the collision layer that the tile data should be added to
 * @param level the level that contains it all
 */
void mgl_tilemap_add_to_collision(MglLine tileLayer,MglLine layerName,MglLevel *level);

#endif
