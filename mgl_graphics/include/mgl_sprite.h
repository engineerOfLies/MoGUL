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
 * @return NULL on error, or a sprite pointer to the loaded file.
 */
MglSprite *mgl_sprite_load_from_image(
    char *filename,
    MglInt frameWidth,
    MglInt frameHeight,
    MglUint framesPerLine,
    MglVec4D *redSwap,
    MglVec4D *greenSwap,
    MglVec4D *blueSwap);

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
 * @param frame which frame to draw
 */
void mgl_sprite_draw(MglSprite *sprite, MglVec2D position,MglUint frame);

/**
 * @brief draw a sprite to the specified surface
 * @param surface the surface to draw to
 * @param sprite the sprite to draw
 * @param postion where the draw the sprite to
 * @param frame which frame to draw
 */
void mgl_sprite_draw_to_surface(SDL_Surface *surface, MglSprite *sprite, MglVec2D position,MglUint frame);

#endif