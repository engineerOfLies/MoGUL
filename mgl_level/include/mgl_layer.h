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
    MglLayerCollision   /**<a collision mask made from either a tile layer or shapes list*/
}MglLayerType;

typedef union
{
    MglTileMap  * map;      /**<if this layer uses a tilemap background*/
    MglParallax * par;      /**<if this layer uses a parallax background*/
    MglSprite   * image;    /**<if this layer is a simple image*/
}MglLayerSelection;

typedef struct
{
    MglLayerSelection layer;/**<*/
    MglLayerType selection; /**<which layer type this layer is*/
    MglBool useParallax;    /**<if true, this layer uses the parallax adjusted drawing position*/
    MglUint bglayer;        /**<if this layer usesParallax, this is the layer to use*/
    MglColor color;         /**<color shift for the layer, 255,255,255,255 is no change*/
}MglLayer;


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
 * @brief draw a single layer
 * @param layer the layer to draw
 * @param par the parallax context to use to adjust the layer
 * @param cam the camera to use for it
 * @param position the positional offset 
 */
void mgl_layer_draw(MglLayer *layer,MglParallax *par,MglCamera *cam,MglVec2D position);

#endif
