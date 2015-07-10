#include "mgl_level.h"
#include "mgl_resource.h"
#include "mgl_audio.h"
#include "mgl_config.h"
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

void mgl_level_close();
MglBool mgl_level_load_resource(char *filename,void *data);
void mgl_level_delete(void *data);

void mgl_level_init(
    MglUint maxLevels)
{
    __mgl_level_resource_manager = mgl_resource_manager_init(
        "mgl level",
        maxLevels,
        sizeof(struct MglLevel_S),
        MglTrue,
        mgl_level_delete,
        mgl_level_load_resource
    );
    atexit(mgl_level_close);    
}

void mgl_level_close()
{
    mgl_resource_manager_free(&__mgl_level_resource_manager);
}

MglBool mgl_level_create_from_def(MglLevel *level,MglDict *def)
{
    MglDict *layers;
    int count,i;
    if (!level)return MglFalse;
    if (!def)return MglFalse;

    level->cam = mgl_camera_load_from_def(mgl_dict_get_hash_value(def,"camera"));
    level->par = mgl_parallax_load_from_def(mgl_dict_get_hash_value(def,"parallax"));
    if (level->par)
    {
        mgl_parallax_change_camera(level->par,level->cam);
    }
    
    layers = mgl_dict_get_hash_value(def,"layers");
    if (!layers)return MglFalse;
    
    count = mgl_dict_get_list_count(layers);
    for (i = 0;i < count;i++)
    {
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
        mgl_level_create_from_def(level,mgl_config_get_object_dictionary(conf,"level"));
        return MglFalse;
    }
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

/*eol@eof*/
