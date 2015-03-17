#ifndef __MGL_TEXT_H__
#define __MGL_TEXT_H__
/**
 * mgl_types
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
    @purpose the MGL text library provides common string support for fixed length
    strings.  For use with unlimited strings mgl will use glib's GString type
 */

#include "mgl_types.h"

/**
 * constant lenth character buffers.
 * These will be used when the convenience of GString is not needed
 */

/**
 * @brief 16 character buffer used for short tags
 */
typedef char MglWord[16];
#define MGLWORDLEN   16
#define mgl_word_cmp(a,b) (strncmp(a,b,MGLWORDLEN))
#define mgl_word_cpy(dst,src) (strncpy(dst,src,MGLWORDLEN))
#define mgl_word_clear(a)  (memset(a,0,sizeof(char)*MGLWORDLEN))
/**
 * @brief 128 character buffer used for statements
 */
typedef char MglLine[128];
#define MGLLINELEN   128
#define mgl_line_cmp(a,b) (strncmp(a,b,MGLLINELEN))
#define mgl_line_cpy(dst,src) (strncpy(dst,src,MGLLINELEN))
#define mgl_line_clear(a)  (memset(a,0,sizeof(char)*MGLLINELEN))
/**
 * @brief 512 character buffer used for text blocks
 */
typedef char MglText[512];
#define MGLTEXTLEN   512
#define mgl_text_cmp(a,b) (strncmp(a,b,MGLTEXTLEN))
#define mgl_text_cpy(dst,src) (strncpy(dst,src,MGLTEXTLEN))
#define mgl_text_clear(a)  (memset(a,0,sizeof(char)*MGLTEXTLEN))

#endif
