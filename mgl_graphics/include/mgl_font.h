#ifndef __MGL_FONT_H__
#define __MGL_FONT_H__

/**
 * mgl_font
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

#include "mgl_text.h"
#include "mgl_vector.h"

/**
 * @purpose font data wraps SDL_TTF
 */
typedef struct MglFont_S MglFont;

/**
 * @brief initialize font system
 */
void mgl_font_init(
    MglInt maxFonts,
    MglLine defaultFontFile,
    MglUint defaultFontPoint
);

/**
 * @brief load the font system from config, including any fonts to load
 * @param filename the config file to load
 */
void mgl_font_init_from_config(char * filename);

/**
 * @brief load a ttf font from file at the specified point pointSize
 * @param filename the font file to load
 * @param pointSize the size to load the font at
 * @return NULL on error or an MglFont pointer on success
 */
MglFont * mgl_font_load(char *filename,MglUint pointSize);

/**
 * @brief free a loaded font
 * @param font a pointer to your font pointer
 */
void mgl_font_free(MglFont **font);

/**
 * @brief simple text drawing function.  renders the provided text with the default font to the screen
 * @param font the font to draw with
 * @param position the upper left corner to draw the text from
 * @param text the text
 * @param color the color of the text
 */
void mgl_font_draw_text_basic(
    MglVec2D position,
    MglLine text,
    MglVec4D color);

#endif
