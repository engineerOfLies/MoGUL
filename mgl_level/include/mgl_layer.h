#ifndef __MGL_LAYER_H__
#define __MGL_LAYER_H__
/**
 * mgl_layer
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
#include "mgl_tilemap.h"
#include "mgl_parallax.h"
#include "mgl_collision.h"
#include "mgl_actor.h"
/**
 * @purpose layers are part of the level, a level will be made up of one or more layers.
 * This lets the game support parallax backgrounds interlaced with tilemaps.
 */

typedef enum {
    MglLayerNone,       /**<Not specified*/
    MglLayerTiles,      /**<Layer uses tile map*/
    MglLayerParallax,   /**<Layer uses a parallax background layer or set*/
    MglLayerImage,      /**<A flat Image as background*/
    MglLayerCollision,  /**<a collision mask made from either a tile layer or shapes list*/
    MglLayerDrawList    /**<a list of custom items to be drawn.  This can be used by the entity system and anything custom*/
}MglLayerType;

typedef struct
{
    GList *list;
    MglCallback draw;
    MglCallback think;
    MglCallback preprocess;
    MglCallback update;
    MglCallback postprocess;
}MglDrawList;

typedef union
{
    MglTileMap  * map;      /**<if this layer uses a tilemap background*/
    MglParallax * par;      /**<if this layer uses a parallax background*/
    MglSprite   * image;    /**<if this layer is a simple image*/
    MglDrawList * drawlist;     /**<list of items to draw*/
    MglCollision *collision;/**<if the layer is a collision mask*/
}MglLayerSelection;

typedef struct
{
    MglLine name;           /**<a searchable name,  this should be unique, but is not enforced*/
    MglLayerSelection layer;/**<which layer type this*/
    MglLayerType selection; /**<which layer type this layer is*/
    MglBool useParallax;    /**<if true, this layer uses the parallax adjusted drawing position*/
    MglUint bglayer;        /**<if this layer usesParallax, this is the layer to use*/
    MglColor color;         /**<color shift for the layer, 255,255,255,255 is no change*/
    MglBool useCollision;   /**<if this layer uses a collision context*/
    MglLine collisionLayer; /**<which collision layer to work with*/
}MglLayer;


/**
 * @brief this structure is used to pass parallax and camera information about the level to any custom layer
 * drawing functions
 */
typedef struct
{
    MglParallax *par;       /**<the parallax context for the level*/
    MglCamera *cam;         /**<the camera context for the level*/
    MglUint layer;          /**<the layer of the parallax context*/
    void *data;             /**<custom data*/
}MglLevelParallaxContext;


/**
 * @brief deletes a layer and cleans up all its data
 * @param layer a pointer to your layer pointer
 */
void mgl_layer_free(MglLayer **layer);

/**
 * @brief load a layer from a definition dictionary
 * @param def the data definition dictionary
 * @return NULL on error, or the layer otherwise
 */
MglLayer *mgl_layer_load_from_def(MglDict *def);

/**
 * @brief make a new tilemap level layer
 * @param map a pointer to the loaded map layer
 * @param useParallax if true, this layer will be treated as if it were part of the level's parallax context
 * @param bglayer if using parallax context, use this layer.  Otherwise this value is ignored
 * @return NULL on error or the allocated layer when done
 */
MglLayer *mgl_layer_new_tile_layer(
    MglTileMap *map,
    MglBool useParallax,
    MglUint bglayer,
    MglColor color
);

/**
 * @brief make a new parallax level layer
 * @param par the parallax context to draw the layer from
 * @param useParallax if true, this parallax layer will be treated as if it were part of the level's parallax context, if false, it will use its own context for all of its layers.
 * @param bglayer which layer this will use
 * @return NULL on error or the allocated layer when done
 */
MglLayer *mgl_layer_new_parallax_layer(
    MglParallax *par,
    MglBool useParallax,
    MglUint bglayer,
    MglColor color
);

/**
 * @brief make a new layer of custom objects to draw
 * @param useParallax if true, this layer will be treated as if it were part of the level's parallax context
 * @param bglayer if using parallax context, use this layer.  Otherwise this value is ignored
 * @return NULL on error or the allocated layer when done
 */
MglLayer *mgl_layer_new_draw_list(
    MglBool useParallax,
    MglUint bglayer,
    MglColor color
);

/**
 * @brief get the type of the layer specified
 * @param layer the layer to query
 * @return the MglLayerType of the layer or MglLayerNone on error
 */
MglLayerType mgl_layer_get_type(MglLayer *layer);

/**
 * @brief draw a single layer
 * @param layer the layer to draw
 * @param par the parallax context to use to adjust the layer
 * @param cam the camera to use for it
 * @param position the positional offset 
 */
void mgl_layer_draw(MglLayer *layer,MglParallax *par,MglCamera *cam,MglVec2D position);

/**
 * @brief run all pre-physics simulation update functions
 */
void mgl_layer_preprocess(MglLayer *layer);

/**
 * @brief run all physics simulations & update functions
 */
void mgl_layer_update(MglLayer *layer);

/**
 * @brief run all physics simulations & update functions
 */
void mgl_layer_postprocess(MglLayer *layer);

/**
 * @brief add a draw item to the layer.  This should be the same type of data in the rest of the layer, though no type checking will be done
 * NOTE: items are not considered to be owned by the draw layer and will not be cleaned up by the layer should it be deleted.
 * @param layer the draw layer to add too.  If the layer is not a MglLayerDrawList, this will be a no-op
 * @param item the item to add to the list, if this is NULL the function will be a no-op
 */
void mgl_layer_add_draw_item(MglLayer *layer,void *item);

/**
 * @brief remove an item from the draw layer.
 * NOTE: items are not considered to be owned by the draw layer and will not be cleaned up by this call, just the reference to it by the list
 * @param layer the draw layer to remove the item from
 * @param item the item to remove from the list.  If not found, nothing will be done.
 */
void mgl_layer_remove_draw_item(MglLayer *layer,void *item);

/**
 * @brief register a custom draw function with the draw list layer
 * @param layer the layer to register a draw function with
 * @param cb the draw function.  This will receive a copy of your callback.data as well as a pointero the layer as the context
 */
void mgl_layer_draw_list_register_draw_function(MglLayer *layer,MglCallback cb);
void mgl_layer_draw_list_register_think_function(MglLayer *layer,MglCallback cb);
void mgl_layer_draw_list_register_preprocess_function(MglLayer *layer,MglCallback cb);
void mgl_layer_draw_list_register_update_function(MglLayer *layer,MglCallback cb);
void mgl_layer_draw_list_register_postprocess_function(MglLayer *layer,MglCallback cb);

/**
 * @brief get the tilemap of the layer specified
 * @param layer the layer to query
 * @return NULL on error or the layer is not a tile map, a tilemap pointer otherwise
 */
MglTileMap *mgl_layer_get_tile_map(MglLayer * layer);

#endif
