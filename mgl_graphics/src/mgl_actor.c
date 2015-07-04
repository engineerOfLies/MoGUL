#include "mgl_actor.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include "mgl_resource.h"
#include "mgl_sprite.h"
#include "mgl_graphics.h"
#include "mgl_logger.h"
#include <glib/ghash.h>
#include <glib/glist.h>
#include <glib.h>

/**
 *@brief an action defines a specific animation sequence for an actor 
 */
typedef struct
{
    MglLine name;       /**<the name of the action*/
    MglUint begin;      /**<the begin frame of animation for an action*/
    MglUint end;        /**<the end frame of an action*/
    MglInt  type;       /**<the MglActionType for this action*/
    MglFloat frameRate; /**<the frame rate for this action.  Higher numbers mean
                            faster animation. 0 means no animation*/
    MglCallback onBegin;/**<the callback to be called when the beginning frame is reached*/
    MglCallback onEnd;  /**<the callback to be called when the end frame is reached*/
    GHashTable *callbacks;/**<a hash of frame to registered callbacks*/    
}MglAction;

struct MglActor_S
{
    MglLine     name;
    MglSprite * sprite;
    MglVec4D    color; /**<color shift for the actor*/
    MglAction * action; /**<the active action*/
    MglUint     lastFrame;
    MglUint     lastTime;
    MglFloat    frame;
    MglFloat    frameRate;
    MglInt      frameDirection;
    MglVec2D    scaleCenter;
    MglVec2D    rotationCenter;
    GList     * actionList;
};

static MglResourceManager * __mgl_actor_resource_manager = NULL;
static MglBool __mgl_actor_use_timing = MglFalse;
static MglFloat __mgl_ticks_per_frame = 33;

MglBool mgl_actor_load_resource(char *filename,void *data);
void mgl_actor_delete(void *data);
static MglAction * __mgl_actor_find_action(MglActor *actor,MglLine action);
static void mgl_actor_close();

void mgl_actor_init_from_config(char *filename)
{
    MglConfig *conf;
    MglDict *dict;
    MglBool useTiming = MglFalse;
    MglFloat ticksPerFrame = 33;
    MglUint maxActors = 1000;
    conf = mgl_config_load(filename);
    if (!conf)
    {
        mgl_logger_error("failed to load config file for actor system: %s",filename);
        return;
    }
    dict = mgl_config_get_dictionary(conf);
    mgl_dict_get_hash_value_as_bool(&useTiming,dict,"useTiming");
    mgl_dict_get_hash_value_as_float(&ticksPerFrame,dict,"ticksPerFrame");
    mgl_dict_get_hash_value_as_uint(&maxActors,dict,"maxActors");
    mgl_config_free(&conf);
    mgl_actor_init(
        maxActors,
        useTiming,
        ticksPerFrame
    );
}

void mgl_actor_init(
    MglUint maxActors,
    MglBool useTiming,
    MglUint ticksPerFrame
)
{
    __mgl_actor_resource_manager = mgl_resource_manager_init(
        "mgl actor",
        maxActors,
        sizeof(struct MglActor_S),
        MglTrue,
        mgl_actor_delete,
        mgl_actor_load_resource
    );
    __mgl_actor_use_timing = useTiming;
    __mgl_ticks_per_frame = ticksPerFrame;
    atexit(mgl_actor_close);
}

MglBool mgl_actor_load_resource(char *filename,void *data)
{
    MglConfig * conf;
    MglDict * actionList;
    MglDict * actionConf;
    MglDict * act;
    MglActor *actor;
    MglLine   spriteFilename,actionType;
    MglAction *action;
    MglInt actionCount,i;
    if (!data)return MglFalse;
    
    actor = (MglActor *)data;
    conf = mgl_config_load(filename);
    if (!conf)
    {
        mgl_logger_error("failed to load actor file: %s",filename);
        return MglFalse;
    }
    act = mgl_config_get_dictionary(conf);
    if (!act)
    {
        mgl_logger_error("actor file: %s, contained no data",filename);
        mgl_config_free(&conf);
        return MglFalse;
    }
    mgl_dict_get_hash_value_as_line(actor->name,act,"name");
    if (mgl_dict_get_hash_value_as_line(spriteFilename,act,"spriteFile"))
    {
        actor->sprite = mgl_sprite_load_from_def(spriteFilename);
    }
    else
    {
        mgl_logger_error("failed to load actor file: %s, no spriteFile",filename);
    }
    actor->frameRate = 1; /*default value*/
    mgl_dict_get_hash_value_as_float(&actor->frameRate,act,"frameRate");
    mgl_dict_get_hash_value_as_vec2d(&actor->scaleCenter,act,"scaleCenter");
    mgl_dict_get_hash_value_as_vec2d(&actor->rotationCenter,act,"rotationCenter");
    mgl_vec4d_set(actor->color,255,255,255,255);
    mgl_dict_get_hash_value_as_vec4d(&actor->color,act,"color");
    actor->frameDirection = 1;
    actor->lastTime = mgl_graphics_get_render_time();
    actionList = mgl_dict_get_hash_value(act,"actionList");
    if (actionList)
    {
        actionCount = mgl_dict_get_list_count(actionList);
        for (i = 0; i < actionCount;i++)
        {
            action = (MglAction *)malloc(sizeof(MglAction));
            if (!action)break;
            memset(action,0,sizeof(MglAction));
            actionConf = mgl_dict_get_list_nth(actionList,i);
            if (!actionConf)break;
            mgl_dict_get_hash_value_as_line(action->name,actionConf,"name");
            mgl_dict_get_hash_value_as_uint(&action->begin,actionConf,"begin");
            mgl_dict_get_hash_value_as_uint(&action->end,actionConf,"end");
            mgl_dict_get_hash_value_as_float(&action->frameRate,actionConf,"frameRate");
            mgl_dict_get_hash_value_as_line(actionType,actionConf,"type");
            if (mgl_line_cmp(actionType,"pass") == 0)
            {
                action->type = MglActionPass;
            }
            else if (mgl_line_cmp(actionType,"loop") == 0)
            {
                action->type = MglActionLoop;
            }
            else if (mgl_line_cmp(actionType,"osci") == 0)
            {
                action->type = MglActionOsci;
            }
            actor->actionList = g_list_append(actor->actionList,action);
            action->callbacks = g_hash_table_new (g_direct_hash,g_direct_equal);
        }
    }
    mgl_config_free(&conf);
    return MglTrue;
}

static void mgl_actor_close()
{
    mgl_resource_manager_free(&__mgl_actor_resource_manager);
}

void mgl_actor_delete(void *data)
{
    GList *it,*hvalues,*hit;
    MglActor *actor;
    MglAction *action;
    MglCallback *cb;
    if (!data)return;
    actor = (MglActor*)data;
    for (it = actor->actionList;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        action = (MglAction *)it->data;
        hvalues = g_hash_table_get_values (action->callbacks);
        for (hit = hvalues;hit != NULL;hit = hit->next)
        {
            cb = (MglCallback *)hit->data;
            mgl_callback_free(&cb);
            hit->data = NULL;
        }
        g_list_free(hvalues);
        g_hash_table_destroy (action->callbacks);
        free(action);
    }
    g_list_free(actor->actionList);
    mgl_sprite_free(&actor->sprite);
}

void mgl_actor_set_action(
    MglActor * actor,
    MglLine    actionName
)
{
    MglAction *action;
    if (!actor)return;
    action = __mgl_actor_find_action(actor,actionName);
    if (!action)
    {
        mgl_logger_warn("actor %s has no action named %s",actor->name,actionName);
        return;
    }
    actor->frame = action->begin;
    actor->action = action;
}

void mgl_actor_set_action_begin_callback(
    MglActor *actor,
    MglLine actionName,
    MglCallback cb
)
{
    MglAction *action;
    if (!actor)return;
    action = __mgl_actor_find_action(actor,actionName);
    if (!action)return;
    mgl_callback_copy(&action->onBegin,cb);
}

void mgl_actor_set_action_end_callback(
    MglActor *actor,
    MglLine actionName,
    MglCallback cb
)
{
    MglAction *action;
    if (!actor)return;
    action = __mgl_actor_find_action(actor,actionName);
    if (!action)return;
    mgl_callback_copy(&action->onEnd,cb);
}

void mgl_actor_set_action_frame_callback(
    MglActor *actor,
    MglLine actionName,
    MglUint frame,
    MglCallback cb
)
{
    MglAction *action;
    MglCallback *CB;
    if (!actor)return;
    action = __mgl_actor_find_action(actor,actionName);
    if (!action)return;
    CB = g_hash_table_lookup(action->callbacks,GINT_TO_POINTER(frame));
    if (CB)
    {
        mgl_callback_copy(CB,cb);
        return;
    }
    CB = mgl_callback_new();
    mgl_callback_copy(CB,cb);
    g_hash_table_insert(action->callbacks,GINT_TO_POINTER(frame),CB);
}

static MglAction * __mgl_actor_find_action(MglActor *actor,MglLine actionName)
{
    GList *it;
    MglAction *action;
    if (!actor)return NULL;
    for (it = actor->actionList;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        action = (MglAction *)it->data;
        if (mgl_line_cmp(actionName,action->name) == 0)
        {
            /*found it*/
            return action;
        }
    }
    return NULL;
}

MglBool mgl_actor_next_frame(MglActor * actor)
{
    MglAction *action;
    MglCallback *cb;
    MglUint now,diff;
    MglUint frame;
    MglFloat timefactor;
    if (!actor)return MglFalse;
    if (!actor->action)return MglFalse;
    action = actor->action;
    actor->lastFrame = actor->frame;
    
    if ((!__mgl_actor_use_timing)||(__mgl_ticks_per_frame == 0))
    {
        /*driven by calls to this function*/
        actor->frame = actor->frame + (actor->frameRate * action->frameRate * actor->frameDirection);
    }
    else
    {
        /*based on sdl_ticks*/
        now = mgl_graphics_get_render_time();
        diff = now - actor->lastTime;
        timefactor = (MglFloat)diff / __mgl_ticks_per_frame;
        actor->frame = actor->frame + (actor->frameRate * action->frameRate * actor->frameDirection)*timefactor;
        actor->lastTime = now;
    }
    frame = (MglUint)actor->frame;
    if (actor->frame > action->end)
    {
        switch(actor->action->type)
        {
            case MglActionLoop:
                actor->frame = actor->action->begin;
                return MglFalse;
            case MglActionPass:
                actor->frame = actor->action->end;
                return MglTrue;
            case MglActionOsci:
                actor->frame = actor->action->end;
                actor->frameDirection = -1;
                return MglFalse;
        }
        if (action->onEnd.function != NULL)
        {
            action->onEnd.function(action->onEnd.data,&frame);
        }
    }
    if (actor->frame < actor->action->begin)
    {
        switch(actor->action->type)
        {
            case MglActionLoop:
                actor->frame = actor->action->begin;
                return MglFalse;
            case MglActionPass:
                actor->frame = actor->action->begin;
                return MglFalse;
            case MglActionOsci:
                actor->frame = actor->action->begin;
                actor->frameDirection = 1;
                return MglTrue;
        }
        if (action->onBegin.function != NULL)
        {
            action->onBegin.function(action->onBegin.data,&frame);
        }
    }
    if ((MglUint)actor->lastFrame != (MglUint)actor->frame)
    {
        cb = g_hash_table_lookup(action->callbacks,GINT_TO_POINTER((MglUint)actor->frame));
        if (cb != NULL)
        {
            if (cb->function)
            {
                cb->function(cb->data,&frame);
            }
        }
    }
    return MglFalse;
}

void mgl_actor_draw(
    MglActor *actor,
    MglVec2D position,
    MglFloat rotation,
    MglVec2D *scale,
    MglVec2D *flip,
    MglVec4D *color
)
{
    MglVec4D colorShift;
    MglVec3D rotationCenter;
    if (!actor)return;
    rotationCenter.x = actor->rotationCenter.x;
    rotationCenter.y = actor->rotationCenter.y;
    rotationCenter.z = rotation;
    if (color)
    {
        colorShift.x = (color->x / 255.0) * actor->color.x; 
        colorShift.y = (color->y / 255.0) * actor->color.y; 
        colorShift.z = (color->z / 255.0) * actor->color.z; 
        colorShift.w = (color->w / 255.0) * actor->color.w; 
    }
    else
    {
        mgl_vec4d_copy(colorShift,actor->color);
    }
    mgl_sprite_draw(
        actor->sprite,
        position,
        scale,
        &actor->scaleCenter,
        &rotationCenter,
        flip,
        &colorShift,
        (MglUint)actor->frame);
}

void mgl_actor_draw_to_surface(
    MglActor *actor,
    MglVec2D position,
    MglVec2D *scale,
    MglVec4D *color,
    SDL_Surface *surface
)
{
    MglVec4D colorShift;
    if (!actor)return;
    if (color)
    {
        colorShift.x = (color->x / 255.0) * actor->color.x; 
        colorShift.y = (color->y / 255.0) * actor->color.y; 
        colorShift.z = (color->z / 255.0) * actor->color.z; 
        colorShift.w = (color->w / 255.0) * actor->color.w; 
    }
    else
    {
        mgl_vec4d_copy(colorShift,actor->color);
    }
    mgl_sprite_draw_to_surface(
        surface,
        actor->sprite,
        position,
        scale,
        &actor->scaleCenter,
        &colorShift,
        (MglUint)actor->frame);

}


MglActor *mgl_actor_load(char * filename)
{
    MglActor *actor;
    actor = mgl_resource_manager_load_resource(__mgl_actor_resource_manager,filename);
    return actor;
}

void mgl_actor_free(MglActor **actor)
{
    mgl_resource_free_element(__mgl_actor_resource_manager,(void **)actor);
}

void mgl_actor_get_size(MglActor *actor,MglUint *w,MglUint *h)
{
    if (!actor)return;
    mgl_sprite_get_size(actor->sprite,w,h);
}
/*eol@eof*/
