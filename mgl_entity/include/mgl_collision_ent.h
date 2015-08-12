#ifndef __MGL_COLLISION_ENT_H__
#define __MGL_COLLISION_ENT_H__
/**
 * mgl_collision_ent
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
#include "mgl_entity.h"
#include "mgl_collision.h"
#include <chipmunk/chipmunk.h>

/**
 * @brief this file is meant to provide a suite of utility functions that will support entites
 * as physics enabled objects.  These functions act on the standard entity, but can be ignored
 * if the game in question does not use the physics system provided by MoGUL.
 */

#define MGL_INFINITY INFINITY

/**
 * @brief sets the entity up as a static body.  Static bodies do not move within a collision space.
 * NOTE: A NO-OP if the entity already has a body associated with it.
 * @param ent the entity to change
 */
void mgl_entity_make_static_body(MglEntity *ent);


/**
 * @brief sets the entity up as a static body.  Static bodies do not move within a collision space.
 * NOTE: if the entity already has a body associated with it, this will set the parameters of mass and
 * moment-of-inertia on the body of the entity
 * @param ent the entity to change
 * @param mass the mass of the body
 * @param inertia the moment of inertia of the body
 */
void mgl_entity_make_collision_body(MglEntity *ent,MglFloat mass,MglFloat inertia);

/**
 * @brief sets the entity up as a dynamic body with a circle shape.
 * NOTE: A NO-OP if the entity already has a shape associated with it
 * @param ent the entity to change
 * @param m the mass of the entity for physics simulation
 * @param r the radius of the circle
 * @param offset the positional offset of the center of the cirlce from the entity's position
 */
void mgl_entity_make_circle_shape(MglEntity *ent,MglFloat m, MglFloat r,MglVec2D offset);

/**
 * @brief assigns the collision entity to the collision space.
 * NOTE: the entity MUST have a physics shape and body associated with it to be added.
 * @param ent the entity to add
 * @param collision the collision space to add the entity to
 */
void mgl_entity_add_to_collision_space(MglEntity *ent, MglCollision *collision);

#endif
