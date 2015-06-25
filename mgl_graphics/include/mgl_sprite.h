#ifndef __MGL_SPRITE_H__
#define __MGL_SPRITE_H__
/**
 * mgl_sprite
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

typedef enum {
    MglSpriteSurface = 1,
    MglSpriteTexture = 2,
    MglSpriteBoth = 3
} MglSpriteMode;

typedef struct MglSprite_S MglSprite;


/**
 * @brief initializes the sprite system based on the passed in parameters
 * @param maxSprites the maximum number of sprites that can be loaded into memory at once
 * @param defaultFramesPerLine the how many frames per line a sprite is assumed to be if not specified
 */
void mgl_sprite_init(
    MglUint maxSprites,
    MglUint defaultFramesPerLine);

/**
 * @brief initializes the sprite system based on the information in the conf file specified
 * @param configFile the file to load configuration from
 */
void mgl_sprite_init_from_config(char * configfile);

/**
 * @brief loads a sprite based on the sprite definition file specified
 * File must be in a supported config format
 * @param filename the sprite definition file to use to load the sprite
 * @return NULL on error, or a sprite pointer to the loaded file.
 */
MglSprite *mgl_sprite_load_from_def(char *filename);

/**
 * @brief load an image with the sprite system.  Sprite specific data is defaulted
 * @param imageFile the image file to load
 * @return NULL on error, or a sprite pointer to the loaded file.
 */
MglSprite *mgl_sprite_load_image(char *imageFile);

/**
 * @brief loads a sprite based on the parameters specified
 * @param filename the name of the image file to load as a sprite
 * @param frameWidth the width of a frame in pixels
 * @param frameHeight the height of a frame in pixels
 * @param framesPerLine the number of frames that appear in each row of the sprite sheet
 * @param redSwap if Not NULL, any pixel that is pure red is replaced with this color mask, keeping the original pixel's intensity
 * @param greenSwap if Not NULL, any pixel that is pure green is replaced with this color mask, keeping the original pixel's intensity
 * @param blueSwap if Not NULL, any pixel that is pure blue is replaced with this color mask, keeping the original pixel's intensity
 * @param colorKey if Not NULL, this color will be set to the clear color fo this sprite
 * @return NULL on error, or a sprite pointer to the loaded file.
 */
MglSprite *mgl_sprite_load_from_image(
    char *filename,
    MglInt frameWidth,
    MglInt frameHeight,
    MglUint framesPerLine,
    MglVec4D *redSwap,
    MglVec4D *greenSwap,
    MglVec4D *blueSwap,
    MglVec4D *colorKey);

/**
 * @brief frees a sprite no longer in use.  Sets your pointer to it to NULL.
 * Frequently accessed sprites are kept in memory for speed of loading
 * @param sprite a pointer to your sprite pointer
 */
void mgl_sprite_free(MglSprite **sprite);

/**
 * @brief draw a sprite to the screen
 * @param sprite the sprite to draw
 * @param postion where the draw the sprite to
 * @param scale Scaling factor in the x and y axis.  If NULL, no scaling is applied.  (1,1) is also no scaling
 * @param scaleCenter Where to scale from.  If NULL, the sprite scales from 0,0
 * @param rotation the rotation point (x,y) and rotation angle (z) in degrees
 * @param flip if specified flip.x (if true) will mirror along the X axis, and flip.y will mirrior along the Y axis
 * @param colorShift if specified (and supported) this color value will be used to color shift the entire sprite.  Alpha Supported
 * @param frame which frame to draw
 */
void mgl_sprite_draw(
    MglSprite *sprite,
    MglVec2D position,
    MglVec2D * scale,
    MglVec2D * scaleCenter,
    MglVec3D * rotation,
    MglVec2D * flip,
    MglVec4D * colorShift,
    MglUint frame);

/**
 * @brief draw an image to the screen at a specific position
 * @param image the image to draw
 * @param position the position to draw the image to
 */
void mgl_sprite_draw_image(MglSprite *image,MglVec2D position);

/**
 * @brief draw a sprite to the specified surface, no support for additional features
 * @param surface the surface to draw to
 * @param sprite the sprite to draw
 * @param postion where the draw the sprite to
 * @param frame which frame to draw
 */
void mgl_sprite_draw_to_surface(SDL_Surface *surface, MglSprite *sprite, MglVec2D position,MglUint frame);

/**
 * @brief get a sprite's frame width and height
 * @param sprite the sprite to check
 * @param w [output] optional if provided this will be populated with the sprite's width
 * @param h [output] optional if provided this will be populated with the sprite's height
 */
void mgl_sprite_get_size(MglSprite *sprite,MglUint *w,MglUint *h);

#endif
