#ifndef __MGL_SAVE_H__
#define __MGL_SAVE_H__
/**
 * mgl_save
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

#include "mgl_dict.h"

/**
 * @brief convert a dictionary to a human readable json format and save it to file
 * NOTE: overwrites file at filepath
 * @param dict the dictionary to convert
 * @param filepath the path to the file to save to
 */
void mgl_save_dict_as_json(MglDict *dict, char *filepath);

/**
 * @brief converts a dicitonary to a binary file format that is not human readable
 * 
 * @param dict the dictionary to convert
 * @param filepath the path to the file to save to
 */
void mgl_save_dict_as_binary_config(MglDict *dict, char *filepath);

/**
 * @brief loads a previously saved binary config file from disk
 * NOTE returned character data must be free()d
 * @param filepath the path to load the file from
 * @return NULL on error, or a character buffer otherwise
 */
char *mgl_save_binary_load(char *filepath);


#endif
