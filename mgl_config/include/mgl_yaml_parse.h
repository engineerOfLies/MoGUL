#ifndef __MGL_YAML_PARSE__
#define __MGL_YAML_PARSE__
#include "mgl_dict.h"

/**
 * @brief parse a yaml file into Mgl Dictionary
 * 
 * @param filename the path to the file to parse
 * 
 * @return NULL on error or a pointer to a valid MglDict
 */
MglDict *mgl_yaml_parse(char* filename);


#endif

