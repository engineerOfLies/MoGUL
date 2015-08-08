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

/**
 * @brief parses a json string into an Mgl Dictionary
 * 
 * @param filename the string containing json data
 * 
 * @return NULL on error or not-a-json file or a valid MglDict
 */
MglDict *mgl_json_parse_string(char *string);

/**
 * @brief converts an MglDict back into a human readable json string
 * @param dict the dictionary to convert
 * @return NULL on error or a character array otherwise
 * NOTE: the string must be free()'d;
 */
char *mgl_json_convert_dict_to_string(MglDict *dict);

/**
 * @brief converts an MglDict back into a packed json string
 * @param dict the dictionary to convert
 * @return NULL on error or a character array otherwise
 * NOTE: the string must be free()'d;
 */
char *mgl_json_convert_dict_to_packed_string(MglDict *dict);

#endif
