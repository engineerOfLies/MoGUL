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

void mgl_collision_update(MglCollision *collision)
{
    MglFloat step;
    MglUint i;
    if (!collision)return;
    if (!collision->space)return;
    if (!collision->iterations)return;
    step = 1/(MglFloat)collision->iterations;
    for (i = 0; i < collision->iterations; i++)
    {
        cpSpaceStep(collision->space, step);
    }
}


/*eol@eof*/
