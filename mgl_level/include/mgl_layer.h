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
    MglLayerTiles,
    MglLayerParallax,
    MglLayerParallaxLayer,
    MglLayerImage,
    MglLayerCustom
}MglLayerType;

typedef union
{
    MglTileMap  * map;
    MglParallax * par;      /**<if this layer uses a parallax background*/
    MglParallax * parLayer; /**<if this layer uses a parallax background*/
    MglSprite   * image;    /**<if this layer is a simple image*/
}MglLayerSelection;

typedef struct
{
    MglLayerSelection layer;
    MglLayerType selection; /**<which layer type this layer is*/
    MglUint bglayer;        /**<if this layer is parLayer, which layer should be drawn*/
    MglBool useParallax;    /**<if true, this layer uses the parallax adjusted drawing position*/
}MglLayer;

#endif
