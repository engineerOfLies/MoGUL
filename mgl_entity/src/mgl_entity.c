#include "mgl_entity.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include "mgl_level.h"
#include <chipmunk/chipmunk.h>

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
    
    cpShape *shape;
    cpBody *body;
};


static MglResourceManager * __mgl_entity_resource_manager = NULL;
static MglBool              __mgl_entity_use_custom_data = MglFalse;
static MglCallback          __mgl_entity_custom_delete = {NULL,NULL};

void mgl_entity_close();
MglBool mgl_entity_load_resource(char *filename,void *data);
void mgl_entity_delete(void *data);

void mgl_entity_init(
    MglUint maxEntities,
    MglBool useCustomData,
    size_t  customDataSize,
    MglCallback * customDelete
                   )
{
    if (!useCustomData)customDataSize = 0;
    __mgl_entity_resource_manager = mgl_resource_manager_init(
        "mgl entity",
        maxEntities,
        sizeof(struct MglEntity_S) + customDataSize,
        MglTrue,
        mgl_entity_delete,
        mgl_entity_load_resource
    );
    if (customDelete)
    {
        mgl_callback_copy(&__mgl_entity_custom_delete,*customDelete);
    }
    atexit(mgl_entity_close);
}

void mgl_entity_close()
{
    mgl_resource_manager_free(&__mgl_entity_resource_manager);
}

void mgl_entity_delete(void *data)
{
    MglEntity *ent;
    if (!data)return;
    ent = (MglEntity *)data;
    if (ent->actor)
    {
        mgl_actor_free(&ent->actor);
    }
    mgl_dict_free(&ent->dict);
    if (__mgl_entity_custom_delete.function != NULL)
    {
        __mgl_entity_custom_delete.function(__mgl_entity_custom_delete.data,ent);
    }
}

MglBool mgl_entity_load_resource(char *filename,void *data)
{
    MglEntity *ent;
    if (!data)return MglFalse;
    if (__mgl_entity_use_custom_data)
    {
        ent = (MglEntity *)data;
        ent->data = &ent[1];
    }
    return MglTrue;
}

void mgl_entity_free(MglEntity **ent)
{
    if (!ent)return;
    mgl_resource_free_element(__mgl_entity_resource_manager,(void **)ent);
}

MglEntity *mgl_entity_new()
{
    MglEntity *ent;
    ent = mgl_resource_new_element(__mgl_entity_resource_manager);
    if (!ent)return NULL;
    if (__mgl_entity_use_custom_data)
    {
        ent->data = &ent[1];
    }
    mgl_vec4d_set(ent->color,255,255,255,255);
    mgl_vec2d_set(ent->scale,1,1);
    return ent;
}

void mgl_entity_set_position(MglEntity *ent,MglVec2D position)
{
    if (!ent)return;
    mgl_vec2d_copy(ent->position,position);
}

MglActor *mgl_entity_get_actor(MglEntity *ent)
{
    if (!ent)return NULL;
    return ent->actor;
}

void mgl_entity_set_actor(MglEntity *ent, MglActor *actor)
{
    if (!ent)return;
    if (ent->actor)
    {
        mgl_actor_free(&ent->actor);
    }
    ent->actor = actor;
}

void * mgl_entity_get_custom_data(MglEntity *ent)
{
    if (!ent)return NULL;
    return ent->data;
}

MglDict * mgl_entity_get_custom_dict(MglEntity *ent)
{
    if (!ent)return NULL;
    return ent->dict;
}

void mgl_entity_set_custom_dict(MglEntity *ent,MglDict *dict)
{
    if (!ent)return;
    if (ent->dict != NULL)
    {
        mgl_dict_free(&ent->dict);
    }
    ent->dict = mgl_dict_clone(dict);
}

void mgl_entity_set_callbacks(
    MglEntity   * ent,
    MglCallback * think,
    MglCallback * update,
    MglCallback * touch,
    MglCallback * damage,
    MglCallback * pre_physics,
    MglCallback * post_physics)
{
    if (!ent)return;
    if (think)
    {
        mgl_callback_copy(&ent->think,*think);
    }
    if (update)
    {
        mgl_callback_copy(&ent->update,*update);
    }
    if (touch)
    {
        mgl_callback_copy(&ent->touch,*touch);
    }
    if (damage)
    {
        mgl_callback_copy(&ent->damage,*damage);
    }
    if (pre_physics)
    {
        mgl_callback_copy(&ent->pre_physics,*pre_physics);
    }
    if (post_physics)
    {
        mgl_callback_copy(&ent->post_physics,*post_physics);
    }
}

void mgl_entity_update_all()
{
    MglEntity *ent;
    for (
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,NULL);
        ent != NULL;
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,ent)
    )
    {
        if (ent->update.function != NULL)
        {
            ent->update.function(ent->update.data,ent);
        }
        if (ent->actor)
        {
            mgl_actor_next_frame(ent->actor);
        }
    }
}

void mgl_entity_draw_all()
{
    MglEntity *ent;
    for (
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,NULL);
        ent != NULL;
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,ent)
    )
    {
        if (ent->draw.function != NULL)
        {
            ent->draw.function(ent->draw.data,ent);
        }
        else if (ent->actor != NULL)
        {
            mgl_actor_draw(
                ent->actor,
                ent->position,
                ent->rotation,
                &ent->scale,
                &ent->flip,
                &ent->color);

        }
    }
}

void mgl_entity_think_all()
{
    MglEntity *ent;
    for (
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,NULL);
        ent != NULL;
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,ent)
    )
    {
        if (ent->think.function != NULL)
        {
            ent->think.function(ent->think.data,ent);
        }
    }
}

void mgl_entity_pre_physics_all()
{
    MglEntity *ent;
    for (
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,NULL);
        ent != NULL;
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,ent)
    )
    {
        if (ent->body != NULL)
        {
            if (ent->pre_physics.function != NULL)
            {
                ent->pre_physics.function(ent->pre_physics.data,ent);
            }
            cpBodySetPos(ent->body, cpv(ent->position.x,ent->position.y));
            cpBodySetVel(ent->body, cpv(ent->velocity.x,ent->velocity.y));
        }
    }
}

void mgl_entity_post_physics_all()
{
    MglEntity *ent;
    cpVect vect;
    for (
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,NULL);
        ent != NULL;
        ent = mgl_resource_get_next_data(__mgl_entity_resource_manager,ent)
    )
    {
        if (ent->body != NULL)
        {
            vect = cpBodyGetPos(ent->body);
            mgl_vec2d_set(ent->position,vect.x,vect.y);
            vect = cpBodyGetVel(ent->body);
            mgl_vec2d_set(ent->velocity,vect.x,vect.y);
            if (ent->post_physics.function != NULL)
            {
                ent->post_physics.function(ent->post_physics.data,ent);
            }
        }
    }
}

/*eol@eof*/
