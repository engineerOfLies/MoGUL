#ifndef __MGL_CAMERA_H__
#define __MGL_CAMERA_H__
/**
 * mgl_camera
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
#include "mgl_rect.h"

/**
 * @purpose the camera library provides a viewing context for viewing game levels.
 * It is built as part of the level system since so much of the level rendering is
 * based on the camera position.
 */

typedef struct MglCamera_S MglCamera;

/**
 * @brief Initialize camera system
 * @param maxCameras how many cameras to support, 1 should work for most games.
 */
void mgl_camera_init(
    MglUint maxCameras
);

/**
 * @brief create a new camera context
 * @param size the width and height of the camera to create
 */
MglCamera *mgl_camera_new(
    MglVec2D size);

/**
 * @brief frees a camera context, setting your pointer to NULL
 * @param cam a pointer to your camera pointer.  Set to NULL upon completion
 */
void mgl_camera_free(MglCamera **cam);

/**
 * @brief check if the provided rectangle is within camera bounds
 * @param cam the camera to check
 * @param rect the rect to check for
 * @return MglTrue if the rectangle clips the camera view, MglFalse otherwise
 */
MglBool mgl_camera_check_bounds(MglCamera *cam,MglRect rect);

/**
 * @brief adjust the provided position relative to the camera
 * @param cam the camera to adjust to
 * @param position the position to check
 * @return a new position relative to the camera position
 */
MglVec2D mgl_camera_get_adjusted_position(MglCamera *cam,MglVec2D position);

/**
 * @brief set a maximum range for the camera to exist in.  Typically this will be the bounds of a game level
 * Without a call to this, no bounds are adhered to.
 * @param cam the camera to change
 * @param bounds the bounds to set.
 */
void mgl_camera_set_bounds(MglCamera *cam,MglRect bounds);

/**
 * @brief enable / disable bound limits for the camera
 * Bounds must be set by mgl_camera_set_bounds
 * @param cam the camera to change
 * @param enable if true, turns bounds checking on, if false disables it.
 */
void mgl_camera_enable_bounds(MglCamera *cam, MglBool enable);

/**
 * @brief sets a new position for the camera.  If bounds are enabled and the new position would
 * put it outside of the bounds, the camera will be placed as far as it can go without violating the bounds
 * @param cam the camera to move
 * @param position the new position to move the cam to
 */
void mgl_camera_change_position(MglCamera *cam, MglVec2D position);


#endif
