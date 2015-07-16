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

/**
 * @brief the MglEntity is an instance of in-game object.  Anything that can act or be acted upon should
 * be modeled with an entity
 */
typedef struct MglEntity_S MglEntity;

/**
 * @brief initialize the entity system
 * @param maxEntities how many simultaneous active entities to support
 * @param useCustomData if true, the custom data will be allocated and linked.
 * @param customDataSize if you use custom data, how large is the memory footprint
 * @param customDelete if you use custom data, provide a custom delete function for the data
 */
void mgl_entity_init(
    MglUint maxEntities,
    MglBool useCustomData,
    size_t  customDataSize,
    MglCallback *customDelete
                    );

MglEntity *mgl_entity_new();


/**
 * @brief draw all entities based on their custom draw functions or by drawing their actors
 */
void mgl_entity_draw_all();

/**
 * @brief run think functions for all living entities
 */
void mgl_entity_think_all();

/**
 * @brief run the update functions for all living entities
 */
void mgl_entity_update_all();

/**
 * @brief run pre-processing before the physics simulation step is applied
 * after this function is called the data is sent to the physics simulator
 */
void mgl_entity_pre_physics_all();

/**
 * @brief run post-processing after the physics simulation step is applied
 * before this function is called the data is pulled from the physics simulator
 */
void mgl_entity_post_physics_all();

/**
 * @brief get a pointer to your custom data block.
 * NOTE: this data should never be freed if the system is using customData.  This memory is managed internally by mgl
 * @param ent the entity to query
 * @return NULL on error or if the system is not configured to support custom data.
 */
void * mgl_entity_get_custom_data(MglEntity *ent);

/**
 * @brief get a pointer to your custom definition dictionary for the provided entity
 * NOTE: this should not be freed by the user, this data will be automatically cleaned up when the entity is freed
 * @param ent the entity to query
 * @return NULL if none, or error;  a pointer to the dictionary data otherwise.
 */
MglDict * mgl_entity_get_custom_dict(MglEntity *ent);

/**
 * @brief copy the dictionary data to the entity
 * NOTE: The data is copied from the dictionary provided, so feel free to clean that up as needed
 * @param ent the entity to add data to
 * @param dict the data definition dictionary to copy the data from
 */
void mgl_entity_set_custom_dict(MglEntity *ent,MglDict *dict);

/**
 * @brief set the actor for an entity
 * NOTE: previous actor will be freed
 * @param ent the entity to set
 * @param actor the actor to assign.  If NULL, this effectively unsets the actor
 */
void mgl_entity_set_actor(MglEntity *ent, MglActor *actor);

/**
 * @brief set the position for the entity
 * NOTE this should not be called during the physics simulation  ie: touch / damage function
 * @param ent the entity to set
 * @param position the new position to set the entity to
 */
void mgl_entity_set_position(MglEntity *ent,MglVec2D position);

/**
 * @brief get a pointer to the entity's actor
 * @param ent the entity to query
 * @return NULL on error 
 */
MglActor *mgl_entity_get_actor(MglEntity *ent);

#endif
