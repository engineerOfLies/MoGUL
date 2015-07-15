#ifndef __MGL_ENTITY_H__
#define __MGL_ENTITY_H__
/**
 * mgl_entity
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
#include "mgl_actor.h"
#include "mgl_vector.h"
#include "mgl_dict.h"

typedef struct
{
    MglVec2D position;
    MglVec2D vector;
    MglVec2D acceleration;
    MglActor *actor;
    void    * data;         /**<custom data pointer*/
    MglDict * def;          /**<custom data definition dictionary*/
}MglEntity;

/**
 * @brief initialize the entity system
 * @param maxEntities how many simultaneous active entities to support
 * @param useCustomData if true, the custom data will be allocated and linked.
 * @param customDataSize if you use custom data, how large is the memory footprint
 */
void mgl_entity_init(
    MglUint maxEntities,
    MglBool useCustomData,
    size_t  customDataSize);

MglEntity *mgl_entity_new();

#endif
