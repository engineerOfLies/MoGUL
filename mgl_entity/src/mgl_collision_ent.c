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
//        cpSpaceConvertBodyToStatic(space, ent->body);
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
  //          cpSpaceConvertBodyToDynamic(space, ent->body, mass, inertia);
            return;
        }
        cpBodySetMass(ent->body, mass);
        cpBodySetMoment(ent->body, inertia);
        return;
    }
    ent->body = cpBodyNew(mass, inertia);
}

void mgl_entity_make_cirlce_shape(MglEntity *ent,MglFloat m, MglFloat r,MglVec2D offset)
{
    MglFloat i;
    if (!ent)return;
    i = cpMomentForCircle(m, r, 0, cpv(offset.x,offset.y));
    mgl_entity_make_collision_body(ent,m,i);
    cpCircleShapeNew(ent->body, r, cpv(offset.x,offset.y));
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
}

/*eol@eof*/