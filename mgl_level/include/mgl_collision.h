#ifndef __MGL_COLLISION_H__
#define __MGL_COLLISION_H__
/**
 * mgl_collision
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
#include "mgl_dict.h"
#include "mgl_callback.h"
#include <chipmunk/chipmunk.h>

/**
 * @brief the Mgl Collision Object is a data structure describing the "other" object in a collision.
 * This structure will be passed in the context field of a collision callback
 */
typedef struct
{
    void *object;       /**<a pointer to the other data*/
    MglWord objType;    /**<a string describing what was hit "border","tile","entity", etc*/
    MglVec2D contact;   /**<the point of contact*/
    MglVec2D normal;    /**<the normal vector at the point of contact*/
}MglCollisionObject;

/**
 * @brief The Collision context is a wrapper for a chipmunk space.  This will be added as a level layer and
 * will be populated with level geometry as well as entity data.
 */
typedef struct MglCollision_S MglCollision;

/**
 * @brief initialize and configure the collision system
 * @param maxSpaces how many collision spaces you want to support simultaneously
 */
void mgl_collision_init(
    MglUint maxSpaces);

/**
 * @brief run an update of the physics engine
 * @param collision the collision physics context to update
 */
void mgl_collision_update(MglCollision *collision);

/**
 * @brief load a collision space context from data definition dictionary
 * @param def the data definition dictionary to load by
 * @return NULL on error or a collision pointer otherwise
 */
MglCollision * mgl_collision_load_from_def(MglDict *def);

/**
 * @brief allocate an empty collision context
 */
MglCollision *mgl_collision_new();

/**
 * @brief free a collision context
 * @param collision a pointer to your collision pointer
 */
void mgl_collision_free(MglCollision **collision);

/**
 * @brief get the chipmunk space from the collision space
 * @param collision the collision space to query
 * @return NULL on error or the chipmunk space pointer
 */
cpSpace *mgl_collision_get_space(MglCollision *collision);

void mgl_collision_add_static_edge(MglCollision *collision,MglVec2D p1,MglVec2D p2);
void mgl_collision_add_static_rect(MglCollision *collision,MglRect rect);
void mgl_collision_add_static_cirlce(MglCollision *collision,MglVec2D center,MglFloat radius);

#endif
