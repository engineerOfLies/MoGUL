#include "mgl_level.h"
#include "mgl_resource.h"
#include "mgl_audio.h"
#include "mgl_config.h"
#include "mgl_logger.h"
#include <glib.h>

struct MglLevel_S
{
    GList *layers;      /**<a list of layers to be rendered that make up this level*/
    MglCamera *cam;     /**<the camera to draw the level relative to*/
    MglParallax *par;   /**<the parallax context for this level.  If present, it can be used to adjust different layer positions relative to the camera*/
    MglVec2D position;  /**<the positional offset for drawing this level*/
    MglColor color;     /**<a global color shift for the whole level*/
};


static MglResourceManager * __mgl_level_resource_manager = NULL;
static MglCamera          * __mgl_level_default_camera = NULL;

void mgl_level_close();
MglBool mgl_level_load_resource(char *filename,void *data);
void mgl_level_delete(void *data);

void mgl_level_init(
    MglUint maxLevels,
    MglCamera *defaultCamera
                   )
{
    __mgl_level_resource_manager = mgl_resource_manager_init(
        "mgl level",
        maxLevels,
        sizeof(struct MglLevel_S),
        MglTrue,
        mgl_level_delete,
        mgl_level_load_resource
    );
    __mgl_level_default_camera = defaultCamera;
    atexit(mgl_level_close);    
}

void mgl_level_close()
{
    mgl_resource_manager_free(&__mgl_level_resource_manager);
}

MglLevel *mgl_level_load(char *filename)
{
    return mgl_resource_manager_load_resource(__mgl_level_resource_manager,filename);
}

MglBool mgl_level_create_from_def(MglLevel *level,MglDict *def)
{
    MglDict *layers;
    int count,i;
    if (!level)return MglFalse;
    if (!def)return MglFalse;
    
    level->cam = mgl_camera_load_from_def(mgl_dict_get_hash_value(def,"camera"));
    if (level->cam == NULL)
    {
        level->cam = __mgl_level_default_camera;
    }
    level->par = mgl_parallax_load_from_def(mgl_dict_get_hash_value(def,"parallax"));
    if (level->par)
    {
        mgl_parallax_change_camera(level->par,level->cam);
    }
    
    layers = mgl_dict_get_hash_value(def,"layers");
    if (!layers)
    {
        mgl_logger_warn("no layers specified for level");
        return MglFalse;
    }
    
    count = mgl_dict_get_list_count(layers);
    for (i = 0;i < count;i++)
    {
        mgl_logger_warn("appending layer %i",i);
        level->layers = g_list_append(level->layers,mgl_layer_load_from_def(mgl_dict_get_list_nth(layers,i)));
    }
    
    mgl_dict_get_hash_value_as_vec2d(&level->position,def,"position");
    mgl_dict_get_hash_value_as_vec4d(&level->color,def,"color");

    return MglTrue;
}

MglBool mgl_level_load_resource(char *filename,void *data)
{
    MglLevel *level;
    MglConfig *conf;
    if (!data)
    {
        return MglFalse;
    }
    level = (MglLevel *)data;
    conf = mgl_config_load(filename);
    if (!conf)
    {
        return MglFalse;
    }
    mgl_level_create_from_def(level,mgl_config_get_object_dictionary(conf,"level"));
    mgl_config_free(&conf);
    return MglTrue;
}

void mgl_level_delete(void *data)
{
    int i,count;
    MglLevel *level;
    MglLayer *layer;
    if (!data)
    {
        return;
    }
    level = (MglLevel *)data;
    count = g_list_length(level->layers);
    for (i = 0;i < count;i++)
    {
        layer = g_list_nth_data(level->layers,i);
        if (!layer)continue;
        mgl_layer_free(&layer);
    }
    g_list_free(level->layers);
    mgl_parallax_free(&level->par);
}

MglLevel *mgl_level_new()
{
    return mgl_resource_new_element(__mgl_level_resource_manager);
}

void mgl_level_free(MglLevel **level)
{
    if (!level)return;
    mgl_resource_free_element(__mgl_level_resource_manager,(void **)level);
}

void mgl_level_draw(MglLevel *level)
{
    int i,count;
    MglLayer *layer;
    if (!level)return;
    count = g_list_length(level->layers);
    for (i = 0;i < count;i++)
    {
        layer = g_list_nth_data(level->layers,i);
        if (!layer)continue;
        mgl_layer_draw(layer,level->par,level->cam,level->position);
    }    
}

void mgl_level_remove_draw_item_from_layer(MglLevel *level,MglLine layername,void *item)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,layername);
    mgl_layer_remove_draw_item(layer,item);
}

void mgl_level_append_draw_item_to_layer(MglLevel *level,MglLine layername,void *item)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,layername);
    mgl_layer_add_draw_item(layer,item);
}

MglLayer *mgl_level_get_layer_by_name(MglLevel *level,MglLine name)
{
    GList *it;
    MglLayer *layer;
    if (!level)
    {
        mgl_logger_info("provided null level");
        return NULL;
    }
    for (it = level->layers;it != NULL; it = it->next)
    {
        if (!it->data)continue;
        layer = (MglLayer *)it->data;
        if (mgl_line_cmp(layer->name,name)== 0)
        {
            return layer;
        }
    }
    mgl_logger_info("level does not contain layer %s",name);
    return NULL;
}

void mgl_level_register_list_draw_function(MglLevel *level,MglLine name, MglCallback draw)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    mgl_layer_draw_list_register_draw_function(layer,draw);
}

void mgl_level_register_list_think_function(MglLevel *level,MglLine name, MglCallback think)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    mgl_layer_draw_list_register_think_function(layer,think);
}

void mgl_level_register_list_update_function(MglLevel *level,MglLine name, MglCallback update)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    mgl_layer_draw_list_register_update_function(layer,update);
}

void mgl_level_register_list_preprocess_function(MglLevel *level,MglLine name, MglCallback preprocess)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    mgl_layer_draw_list_register_preprocess_function(layer,preprocess);
}

void mgl_level_register_list_postprocess_function(MglLevel *level,MglLine name, MglCallback postprocess)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    mgl_layer_draw_list_register_postprocess_function(layer,postprocess);
}

MglTileMap *mgl_level_get_layer_tilemap_by_name(MglLevel *level,MglLine name)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    return mgl_layer_get_tile_map(layer);
}

MglCollision *mgl_level_get_layer_collision_by_name(MglLevel *level,MglLine name)
{
    MglLayer *layer;
    layer = mgl_level_get_layer_by_name(level,name);
    return mgl_layer_get_collision(layer);
}

/*eol@eof*/
