#include "mgl_collision.h"
#include "mgl_resource.h"
#include "mgl_config.h"
#include "mgl_logger.h"
#include <chipmunk/chipmunk.h>

struct MglCollision_S
{
    MglLine name;
    cpSpace *space;
    MglUint iterations;
    MglFloat slop;
    MglFloat bias;
    MglFloat damping;
    MglVec2D gravity;
    MglCallback touchBegin;
    MglCallback touchPreSolve;
    MglCallback touchPostSolve;
    MglCallback touchSeparate;
};

static MglResourceManager * __mgl_collision_resource_manager = NULL;

void mgl_collision_close();
MglBool mgl_collision_load_resource(char *filename,void *data);
void mgl_collision_delete(void *data);

void mgl_collision_init(
    MglUint maxSpaces)
{
    __mgl_collision_resource_manager = mgl_resource_manager_init(
        "mgl collision",
        maxSpaces,
        sizeof(struct MglCollision_S),
        MglTrue,
        mgl_collision_delete,
        mgl_collision_load_resource
    );
    atexit(mgl_collision_close);
}

void mgl_collision_close()
{
    mgl_resource_manager_free(&__mgl_collision_resource_manager);
}

void mgl_collision_delete(void *data)
{
    MglCollision *collision;
    if (!data)return;
    collision = data;
    if (collision->space != NULL)
    {
        cpSpaceFree(collision->space);
        collision->space = NULL;
    }
}

MglCollision *mgl_collision_new()
{
    return mgl_resource_new_element(__mgl_collision_resource_manager);
}

void mgl_collision_free(MglCollision **collision)
{
    if (!collision)return;
    mgl_resource_free_element(__mgl_collision_resource_manager,(void **)collision);
}

MglCollision * mgl_collision_load_from_def(MglDict *def)
{
    MglCollision *collision = NULL;
    if (!def)return NULL;
    /*TODO*/
    return collision;
}

MglCollision *mgl_collision_load_from_dict(MglCollision *collision,MglDict *def)
{
    cpSpace *space;
    cpVect gravity;
    if (!def)
    {
        return NULL;
    }
    space = cpSpaceNew();
    if (!space)return NULL;
    
    if (!collision)
    {
        collision = mgl_resource_new_element(__mgl_collision_resource_manager);
        if (!collision)
        {
            return NULL;
        }
    }
    collision->space = space;
    if (mgl_dict_get_hash_value_as_uint(&collision->iterations,def,"iterations"))
    {
        cpSpaceSetIterations(space,collision->iterations);
    }
    else
    {
        collision->iterations = cpSpaceGetIterations(space);
    }
    if (mgl_dict_get_hash_value_as_float(&collision->bias,def,"bias"))
    {
        cpSpaceSetCollisionBias(space, collision->bias);
    }
    else
    {
        collision->bias = cpSpaceGetCollisionBias(space);
    }
    if (mgl_dict_get_hash_value_as_float(&collision->slop,def,"slop"))
    {
        cpSpaceSetCollisionSlop(space, collision->slop);
    }
    else
    {
        collision->slop = cpSpaceGetCollisionSlop(space);
    }
    if (mgl_dict_get_hash_value_as_float(&collision->damping,def,"damping"))
    {
        cpSpaceSetDamping(space, collision->damping);
    }
    else
    {
        collision->damping = cpSpaceGetDamping(space);
    }
    if (mgl_dict_get_hash_value_as_vec2d(&collision->gravity,def,"gravity"))
    {
        cpSpaceSetGravity(space, cpv(collision->gravity.x,collision->gravity.y));
    }
    else
    {
        gravity = cpSpaceGetGravity(space);
        collision->gravity.x = gravity.x;
        collision->gravity.y = gravity.y;
    }
    
    return collision;
}

MglBool mgl_collision_load_resource(char *filename,void *data)
{
    if (!data)
    {
        return MglFalse;
    }
    return MglTrue;
}

cpSpace *mgl_collision_get_space(MglCollision *collision)
{
    if (!collision)return NULL;
    return collision->space;
}

void mgl_collision_update(MglCollision *collision)
{
    MglFloat step;
    MglUint i;
    if (!collision)
    {
        mgl_logger_warn("NULL collision provided");
        return;
    }
    if (!collision->space)
    {
        mgl_logger_warn("no space in collision layer");
        return;
    }
    if (!collision->iterations)
    {
        mgl_logger_warn("collision space has no update iterations specified");
        return;
    }
    step = 1/(MglFloat)collision->iterations;
    for (i = 0; i < collision->iterations; i++)
    {
        mgl_logger_warn("physics...");
        cpSpaceStep(collision->space, step);
    }
}

void mgl_collision_add_static_edge(MglCollision *collision,MglVec2D p1,MglVec2D p2)
{
    cpSpace *space;
    cpBody *body;
    cpShape *shape;
    if (!collision)
    {
        mgl_logger_error("no collision layer provided");
        return;
    }
    space = mgl_collision_get_space(collision);
    if (!space)
    {
        mgl_logger_error("collision layer %s has no space",collision->name);
        return;
    }
    body = cpSpaceGetStaticBody(space);
    if (!body)
    {
        mgl_logger_error("collision layer %s space has no static body",collision->name);
        return;
    }
    shape = cpSegmentShapeNew(body, cpv(p1.x,p1.y), cpv(p2.x,p2.y), 0.5);
    if (!shape)
    {
        mgl_logger_error("failed to make edge for collision space %s",collision->name);
        return;
    }
    cpSpaceAddStaticShape(space,shape);
}

void mgl_collision_add_static_rect(MglCollision *collision,MglRect rect)
{
    mgl_collision_add_static_edge(collision,mgl_vec2d(rect.x,rect.y),mgl_vec2d(rect.x + rect.w,rect.y));
    mgl_collision_add_static_edge(collision,mgl_vec2d(rect.x+rect.w,rect.y),mgl_vec2d(rect.x + rect.w,rect.y+rect.h));
    mgl_collision_add_static_edge(collision,mgl_vec2d(rect.x,rect.y),mgl_vec2d(rect.x,rect.y + rect.h));
    mgl_collision_add_static_edge(collision,mgl_vec2d(rect.x,rect.y + rect.h),mgl_vec2d(rect.x + rect.w,rect.y + rect.h));
}

void mgl_collision_add_static_cirlce(MglCollision *collision,MglVec2D center,MglFloat radius)
{
    cpSpace *space;
    cpBody *body;
    cpShape *shape;
    if (!collision)
    {
        mgl_logger_error("no collision layer provided");
        return;
    }
    space = mgl_collision_get_space(collision);
    if (!space)
    {
        mgl_logger_error("collision layer %s has no space",collision->name);
        return;
    }
    body = cpSpaceGetStaticBody(space);
    if (!body)
    {
        mgl_logger_error("collision layer %s space has no static body",collision->name);
        return;
    }
    shape = cpCircleShapeNew(body, radius, cpv(center.x,center.y));
    if (!shape)
    {
        mgl_logger_error("failed to make edge for collision space %s",collision->name);
        return;
    }
    cpSpaceAddStaticShape(space,shape);
}


/*eol@eof*/
