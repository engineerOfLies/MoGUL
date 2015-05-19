#ifndef __MGL_CONFIG__
#define __MGL_CONFIG__

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
 */

#include "mgl_dict.h"

typedef struct MglConfig_S MglConfig;

/**
 * @brief initializes the config loading system.  cleanup will happen Automatically on program exit
 */
void mgl_config_init();

/**
 * @brief loads a config file from disk.  Supports JSON and YAML.
 * Automatically detects file type and loads the data into an MglDict
 * @param filename the filename / path to the file to load.
 * @return a loaded MglConfig or NULL on error.  See logs for errors.
 */
MglConfig *mgl_config_load(MglLine filename);

/**
 * @brief free a config file loaded.
 * 
 * @param config the config file loaded.
 */
void mgl_config_free(MglConfig **config);

/**
 * @brief Get the dictionary from the config file
 * Note: Do NOT free the dictionary, it is owned by the config loader
 * @param config the config loaded.
 * @return a pointer to the config file's data as a dictionary.
 */
MglDict *mgl_config_get_dictionary(MglConfig *config);


#endif
