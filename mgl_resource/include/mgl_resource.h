#ifndef __MGL_RESOURCE__
#define __MGL_RESOURCE__
/**
 * mgl_resource
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
#include "mgl_text.h"

/**
 * @purpose this resource manager will handle all of the boilerplate code
 *          that goes into loading and managing a resource
 */

typedef struct
{
  MglBool   _initialized;
  MglLine   name;
  MglUint   _data_count;
  MglUint   _data_max;
  MglUint   _data_size;
  MglBool   _data_unique;
  MglUint   _data_id_pool;/**<increments with every allocated resource.*/
  char *    _data_list;   /**<character buffer of data*/
  void (*data_delete)(void *data);
  MglBool (*data_load)(char *filename,void *data);
}MglResourceManager;

/*All resources managed by this system must contain this header structure.*/
typedef struct
{
  MglUint refCount;
  MglLine filename;
  MglUint index;
  MglUint id;       /**<unique identifier.  In case of a memory re-use it can be
                        compared with expected for validity*/
  MglUint timeFree; /**<time when free was called on resource.  Oldest get reclaimed first*/
  MglUint underflowprot;
}MglResourceHeader;

MglResourceManager *mgl_resource_manager_new();

MglResourceManager * mgl_resource_manager_init(
    MglLine managerName,
    MglUint max,
    MglUint dataSize,
    MglBool dataUnique,
    void    (*data_delete)(void *data),
    MglBool (*data_load)(char *filename,void *data)
    );

/**
 * @brief returns a pointer to a new allocated element or NULL on error
 *
 * @param manager the resource manager to get an element from
 *
 * @return a pointer to the data or NULL on error
 */
void * mgl_resource_new_element(MglResourceManager *manager);

/**
 * @brief reduces the reference count to the element passed in.  if data in the
 *        manager is unique. it will invoke delete on it.
 *
 * @param manager the resource manager where the element belongs
 * @param data a pointer to the element pointer.  The element pointer will be
 *             set to NULL upon completion.
 *
 */
void mgl_resource_free_element(MglResourceManager *manager,void **data);

void mgl_resource_manager_free(MglResourceManager **manager);

/**
 * @brief delete any unreferenced data from the resource manager
 *        a delete function must be set.
 *
 * @param manager the resource manager to be cleaned.
 */
void mgl_resource_manager_clean(MglResourceManager *manager);

/**
* @brief delete ALL allocated resources.
*        a delete function must be set.
*
* @param manager the resource manager to be cleared.
*/
void mgl_resource_manager_clear(MglResourceManager *manager);

/**
 * @brief Allocated and loads a resource from file.  Calls the manager's data load
 * function pointer.
 * @param manager to load a resource for
 * @param filename the file to load the resource from
 * @return a pointer to the allocated and loaded resource or NULL on error
 */
void *mgl_resource_manager_load_resource(MglResourceManager *manager,char *filename);

/**
 * @brief Gets the count of active elements in the resource manager.
 * @param manager to resource manager to check
 * @return the number of elements in use (ref count > 0)
 */
MglUint mgl_resource_manager_get_element_count(MglResourceManager *manager);

/**
 * @brief returns the index of the element passed.
 * @param manager the resource manager to check
 * @param data the resource data pointer to check
 * @return the unsigned integer index of the element, or -1 on error
 */
MglInt mgl_resource_element_get_index(MglResourceManager *manager,void *element);

/**
* @brief returns the index of the element passed.
* @param manager the resource manager to check
* @param data the resource data pointer to check
* @return the unsigned integer id of the element, or -1 on error
*/
MglInt mgl_resource_element_get_id(MglResourceManager *manager,void *element);

/**
 * @brief iterates through the resource list
 *
 * @param manager the resource manager to iterate through
 * @param data the position to iterate from.  If passed NULL, it will
 * iterate through the list from the beginning.
 * 
 * @return a pointer to the next element in the resource manager or NULL if
 * there are no more elements.
 */
void * mgl_resource_get_next_data(MglResourceManager *manager,void *data);

/**
 * @brief confirms that the resource pointed at by element is the one expected based
 * on the id.
 * @param manager the resource manager for which this element is a member
 * @param element the element in question
 * @param id the expected id of the element
 * @return MglTrue if the id's match or MglFalse on error or mismatch.
 */
MglBool mgl_resource_element_id_valid(MglResourceManager *manager,void *element,MglUint id);

MglUint mgl_resource_element_get_refcount(MglResourceManager *manager,void *element);
void mgl_resource_element_get_filename(MglLine filename, MglResourceManager *manager,void *element);

#endif

