#ifndef __MGL_DICT_H__
#define __MGL_DICT_H__
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

#include "mgl_types.h"
#include "mgl_vector.h"
#include "mgl_rect.h"
#include <glib.h>

/**
 * @brief the typed pointer is a pointer to a very limited subset of types.  It packs
 * type information in with the pointer so that destroying and copying can be done without
 * prior knowledge of its set up.
 */

typedef enum mglDictTypes {
  MGL_DICT_VOID,   /**<custom data*/
  MGL_DICT_INT,
  MGL_DICT_UINT,
  MGL_DICT_FLOAT,
  MGL_DICT_STRING, /**<GString*/
  MGL_DICT_LIST,   /**<GList of something*/
  MGL_DICT_HASH,   /**<GHashTable of something*/
  MGL_DICT_CUSTOM0 /**<for user defined types.  MGL will not use Custom0 or after.*/
}MglDictTypes;

typedef void (*MglDictFree)(void *data);

/**
* @brief his structure wraps description information for a pointer to a container type
* it will be used in spawn and config types where we deal with pointers to unknown types.
*/
typedef struct dict_S
{
  MglDictTypes keyType;
  MglUint itemCount;  /*in the case of list or hash*/
  MglDictFree keyFree;
  struct dict_S * (*keyClone)(struct dict_S *src);
  void *keyValue;
}MglDict;

/**
* @brief frees the MglDict and sets the passed in pointer to NULL
* @param a pointer to a pointer to a typed pointer.
*/
void mgl_dict_free(MglDict **point);

/**
* @brief destroys the typed pointer passed
* @param point the pointer to be destroyed
*/
void mgl_dict_destroy(MglDict *point);

/**
* @brief allocates and returns a pointer to an empty typed pointer
* @return NULL on allocation error or a zero'd out MglDict
*/
MglDict *mgl_dict_new();

/**
 * @brief duplicate a dict
 * @param src the original dict to be duplicated
 * @return a pointer to the duplicated dict or NULL on error
 */
MglDict *mgl_dict_clone(MglDict *src);

/**
* @brief allocated and sets up a pointer to a GString filled with the default text
* @param text starting text.  May be empty
* @return NULL on allocation error or a set up MglDict of a GString
*/
MglDict *mgl_dict_new_string(char *text);

/*creates new key values based on inputs of different types*/

MglDict *mgl_dict_new_bool(MglBool n);
MglDict *mgl_dict_new_int(MglInt n);
MglDict *mgl_dict_new_uint(MglInt n);
MglDict *mgl_dict_new_float(MglFloat n);
MglDict *mgl_dict_new_vec2d(MglVec2D n);
MglDict *mgl_dict_new_vec3d(MglVec3D n);
MglDict *mgl_dict_new_vec4d(MglVec4D n);
MglDict *mgl_dict_new_rect(MglRect n);
MglDict *mgl_dict_new_rectf(MglRectFloat n);


/**
* @brief allocated and sets up a pointer to an Empty GList.
* @return NULL on allocation error or a set up MglDict of a GList
*/
MglDict *mgl_dict_new_list();

/**
* @brief allocated and sets up a pointer to an Empty GHash.
* @return NULL on allocation error or a set up MglDict of a GHashTable
*/
MglDict *mgl_dict_new_hash();

/**
* @brief Retrieves the string information for a string dict.
* if its not a pointer to a string, it will return without doing anything.
* @param output the output MglLine.  Untouched on error
* @param key the MglDict string to retrieve
* @return MglTrue if found and returned correctly, MglFalse otherwise
*/
MglBool mgl_dict_get_line(MglLine output,MglDict *key);

/**
* @brief appends a list item to the MglDict list value.
* if its not a pointer to a list, it will return without doing anything.
* @param list the typed pointer of a glist
* @param item the MglDict to the item to be added to the list.
*/
void mgl_dict_list_append(MglDict *list,MglDict *item);

/**
 * @brief get the number of elements in this hash.
 * NOTE: This does not recurse into the elements of the hash
 * @param hash the dict has to query
 * @return 0 if empty or not a hash, the count otherwise
 */
MglUint mgl_dict_get_hash_count(MglDict *hash);

MglDict *mgl_dict_get_hash_nth(MglLine key, MglDict *hash, MglUint n);

/**
* @brief Insert or replace a key in the MglDict of a hash.
* if it is not a pointer to a hash it will return without doing anything.
* @param hash the typed pointer of a ghash
* @param key the key to be inserted.  Limited to the size of MglLine.
* @param value the MglDict to the item to be added to the hash.
*/
void mgl_dict_hash_insert(MglDict *hash,MglLine key,MglDict *value);

/**
* @brief Removes a key from the MglDict of a hash.
* if it is not a pointer to a hash it will return without doing anything.
* if found, mgl_dict_destroy is called on the value.
* @param hash the typed pointer of a ghash
* @param key the key to be removed.  Limited to the size of MglLine.
*/
void mgl_dict_hash_remove(MglDict *hash,char *key);

/**
 * @brief looks up the key in the hash.
 * checks type before any operation
 * @param hash the typed pointer of a ghash
 * @param key the key to be found.  Limited to the size of MglLine.
 * @return NULL if not a hash, or not found. A typed pointer to the value if found.
 */
MglDict *mgl_dict_get_hash_value(MglDict *hash,MglLine key);

/*type value accessors*/
MglBool mgl_dict_get_hash_value_as_bool(MglBool *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_uint(MglUint *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_int(MglInt *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_float(MglFloat *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_line(MglLine output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_vec2d(MglVec2D *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_vec3d(MglVec3D *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_vec4d(MglVec4D *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_rect(MglRect *output, MglDict *hash, MglLine key);
MglBool mgl_dict_get_hash_value_as_rectfloat(MglRectFloat *output, MglDict *hash, MglLine key);

/**
* @brief looks up the nth item in the list
* checks type before any operation
* @param list the typed pointer of a glist
* @param n the index of the item to be found.
* @return NULL if not a list, or not found. A typed pointer to the value if found.
*/
MglDict *mgl_dict_get_list_nth(MglDict *list, MglUint n);

/**
 * @brief retrieve the count of items in a list dict
 * @param list the dict that is a a list
 * @return the number of items in the list (note that any sub items are not counted))
 */
MglUint mgl_dict_get_list_count(MglDict *list);

/**
 * @brief removed the nth item from the list
 * checks type before any operation
 * calls mgl_dict_destroy on the item found.
 * @param list the typed pointer of a glist
 * @param n the index of the item to be found and deleted
 */
void mgl_dict_list_remove_nth(MglDict *list, MglUint n);

/**
* @brief moves the nth item in the list to the bottom of the list
* checks type before any operation
* @param list the typed pointer of a glist
* @param n the index of the item to be moved
*/
void mgl_dict_list_move_nth_bottom(MglDict *list, MglUint n);

/**
* @brief moves the nth item in the list to the top of the list
* checks type before any operation
* @param list the typed pointer of a glist
* @param n the index of the item to be moved
*/
void mgl_dict_list_move_nth_top(MglDict *list, MglUint n);

/**
 * @brief clears all items from the list, resetting it back to new
 * @param list the dict to be cleared.  No-op if this is not a dict list
 */
void mgl_dict_list_clear(MglDict *list);

/**
 * @brief prints to terminal the dict specified
 */
void mgl_dict_print(MglDict *chain);

/**
* @brief convenience function when working with GString types.
* @param string the string to free.
*/
void mgl_g_string_free(char *string);

#endif
