#ifndef __MGL_JSON_PARSE__
#define __MGL_JSON_PARSE__

#include "mgl_dict.h"

/**
 * @brief parses a json file into an Mgl Dictionary
 * 
 * @param filename the file path to the json file
 * 
 * @return NULL on error or not-a-json file or a valid MglDict
 */
MglDict *mgl_json_parse(char *filename);

#endif
