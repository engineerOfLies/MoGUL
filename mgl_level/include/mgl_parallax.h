#ifndef __MGL_PARALLAX_H__
#define __MGL_PARALLAX_H__
/**
 * mgl_parallax
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

#include "mgl_vector.h"
#include "mgl_camera.h"
/**
 * @purpose parallax backgrounds are drawn in such a way that different layers 
 */

typedef struct MglParallax_S MglParallax;

/**
 * @brief initialize the parallax manager
 * @param maxParallaxs how many simulataneous background you want to support
 * @param defaultCam if provided this will be the assumed camera for parallax calculations
 */
void mgl_parallax_init(
    MglUint maxParallaxs,
    MglCamera *defaultCam);

/**
 * @brief load a parallax background
 * @param filename the definition file to load
 * @param cam use the camera to set proper aspects
 * @return NULL on error or a parallax background on success.
 */
MglParallax *mgl_parallax_load(char * filename,MglCamera *cam);

/**
 * @brief draw all parallax background layers back to front
 * @param par the parallax background to draw
 * @param position the position to draw it from
 */
void mgl_parallax_draw_all_layers(MglParallax *par,MglVec2D position);

/**
 * @brief draw only 1 specific layer of a pallax background
 * @param par the parallax background to draw
 * @param l the layer index to draw
 * @param position the position to draw the layer at
 * @param color adjust the layer's color by this.
 */
void mgl_parallax_draw_layer(MglParallax *par,MglUint l,MglVec2D position,MglVec4D * color);

/**
 * @brief mutate the provided position based on the parallax layer's relative position
 * @param par the parallax context to adjust to
 * @param l the layer index to adjust by
 * @param position the position to change
 * @return a new position made relative to the parallax context
 */
MglVec2D mgl_parallax_layer_adjust_position(MglParallax *par,MglUint l,MglVec2D position);

/**
 * @brief change which plane will be used as the camera 1:1 layer
 * @param par the parallax background to change
 * @param n which layer should be the focus
 */
void mgl_parallax_change_camera_plane(MglParallax *par, MglUint n);

/**
 * @brief change which camera the parallax context uses for calculations
 * @param par the parallax background to change
 * @param cam use the camera to set proper aspects
 */
void mgl_parallax_change_camera(MglParallax *par,MglCamera *cam);

/**
 * @brief get the size of the camera plane
 * @param par the parallax background to check
 * @param w [output] optional if provided this will be populated with the layer's width
 * @param h [output] optional if provided this will be populated with the layer's height
 */
void mgl_parallax_get_size(MglParallax *par,MglUint *w,MglUint *h);


#endif
