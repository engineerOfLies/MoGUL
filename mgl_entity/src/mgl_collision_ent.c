#include "mgl_logger.h"
#include "mgl_collision_ent.h"
#include "mgl_ent_def.h"

void mgl_entity_make_static_body(MglEntity *ent)
{
    cpSpace *space;
    if (!ent)return;
    if (ent->body != NULL)
    {
        if (cpBodyIsStatic(ent->body))
        {
            return;
        }
        space = cpBodyGetSpace(ent->body);
        if (!space)
        {
            mgl_logger_warn("body cannot be converted to static until it has been added to a space");
            return;
        }
    }
    ent->body = cpBodyNewStatic();
}

void mgl_entity_make_collision_body(MglEntity *ent,MglFloat mass,MglFloat inertia)
{
    cpSpace *space;
    if (!ent)return;
    if (ent->body != NULL)
    {
        if (cpBodyIsStatic(ent->body))
        {
            space = cpBodyGetSpace(ent->body);
            if (!space)
            {
                mgl_logger_warn("body cannot be converted to dynamic until it has been added to a space");
                return;
            }
            return;
        }
        cpBodySetMass(ent->body, mass);
        cpBodySetMoment(ent->body, inertia);
        return;
    }
    mgl_logger_debug("setting mass(%f) and inertia(%f) for entity %s",mass,inertia,ent->name);
    ent->body = cpBodyNew(mass, inertia);
}

void mgl_entity_make_circle_shape(MglEntity *ent,MglFloat m, MglFloat r,MglVec2D offset)
{
    MglFloat i;
    if (!ent)return;
    i = cpMomentForCircle(m, 0, r, cpv(offset.x,offset.y));
    mgl_entity_make_collision_body(ent,m,i);
    ent->shape = cpCircleShapeNew(ent->body, r, cpv(offset.x,offset.y));
    cpShapeSetGroup(ent->shape,CP_NO_GROUP);
    cpShapeSetLayers(ent->shape,CP_ALL_LAYERS);
    cpShapeSetCollisionType(ent->shape, 2);
    mgl_logger_error("entity collision layer: %i",cpShapeGetLayers(ent->shape));
    mgl_logger_error("entity collision group: %i",cpShapeGetGroup(ent->shape));
    mgl_logger_error("entity collision type: %i",cpShapeGetCollisionType(ent->shape));
    cpShapeSetUserData(ent->shape, (const cpDataPointer)ent);
}

void mgl_entity_add_to_collision_space(MglEntity *ent, MglCollision *collision)
{
    cpSpace *space;
    if (!ent)return;
    if (!collision)return;
    if ((!ent->body) || (!ent->shape))
    {
        mgl_logger_warn("entity cannot be added to collision space until it has been given shape and body information");
        return;
    }
    space = mgl_collision_get_space(collision);
    if (!space)
    {
        mgl_logger_warn("collision space is invalid");
        return;
    }
    cpSpaceAddBody(space, ent->body);
    cpSpaceAddShape(space,ent->shape);
}

/*eol@eof*/