#ifndef __MGL_ENT_DEF_H__
#define __MGL_ENT_DEF_H__

#include "mgl_vector.h"
#include "mgl_callback.h"
#include "mgl_actor.h"
#include "mgl_dict.h"
#include "mgl_tilemap.h"
#include <chipmunk/chipmunk.h>
/**
 * @purpose this file is meant to hold the definition of the entity structure.  This is to be kept private to the
 * internal functions of the mgl_entity library so that all entity functions can directly access the entity members
 * but allowing for users of the library to be kept from the details of it
 */

struct MglEntity_S
{
    MglVec2D position;
    MglVec2D velocity;
    MglVec2D acceleration;
    MglVec2D scale;
    MglFloat rotation;
    MglVec2D flip;
    MglColor color;
    
    MglActor *actor;
    
    void    * data;         /**<custom data pointer*/
    MglDict * dict;         /**<custom data definition dictionary*/
    
    MglCallback draw;
    MglCallback think;
    MglCallback update;
    MglCallback touch;
    MglCallback damage;
    MglCallback pre_physics;
    MglCallback post_physics;
    
    MglTileMap *map;        /**<the map that this entity is a part of*/
    
    cpSpace *space;         /**<the physics space that this entity is a part of*/
    cpShape *shape;         /**<the collision shape for this entity*/
    cpBody *body;           /**<the collision body for this entity*/
};


#endif
