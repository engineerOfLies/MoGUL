#include "mgl_parallax.h"
#include "mgl_resource.h"

#include "mgl_sprite.h"
#include "mgl_actor.h"

#include "mgl_config.h"

#include <glib.h>

/**
 * @brief the layer type holds information on how to draw a given layer of the parallax background
 */
typedef struct
{
    MglSprite *image; /**<the image to draw*/
    MglActor  *actor; /**<optionally an actor can be used instead*/
    MglVec2D   aspect;/**<the percent offset from the camera position this should be drawn at.  This is calculated*/
    MglBool    cameraPlane; /**<if true, this layer is drawn 1:1 to camera position*/
}MglLayer;

/**
 * @brief a parallax background context
 */
struct MglParallax_S
{
    GList    *layers;      /**<the list of layers that make up this parallax background context*/
    MglLayer *cameraPlane;  /**<the layer that is drawn 1:1 to the camera / All other planes are moved accordingly*/
};


static MglResourceManager * __mgl_parallax_resource_manager = NULL;
void mgl_parallax_close();
MglBool mgl_parallax_load_resource(char *filename,void *data);
void mgl_parallax_delete(void *data);

void mgl_parallax_init(
    MglUint maxParallaxs)
{
    __mgl_parallax_resource_manager = mgl_resource_manager_init(
        "mgl parallax",
        maxParallaxs,
        sizeof(struct MglParallax_S),
        MglFalse,
        mgl_parallax_delete,
        mgl_parallax_load_resource
    );
    atexit(mgl_parallax_close);
}

void mgl_parallax_close()
{
    mgl_resource_manager_free(&__mgl_parallax_resource_manager);
}

void mgl_parallax_delete(void *data)
{
    MglParallax *par;
    MglLayer *layer;
    GList *it;
    par = (MglParallax *)data;
    if (!par)return;
    for (it = par->layers;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        layer = (MglLayer *)it->data;
        mgl_sprite_free(&layer->image);
        mgl_actor_free(&layer->actor);
        free(layer);
        it->data = NULL;
    }
    par->cameraPlane = NULL;
    g_list_free(par->layers);
    return;
}

void mgl_parallax_free(MglParallax **parallax)
{
    if (!parallax)return;
    mgl_resource_free_element(__mgl_parallax_resource_manager,(void **)parallax);
}

MglLayer *mgl_parallax_load_layer(MglDict *data)
{
    MglLayer *layer;
    MglLine file;
    if (!data)return NULL;
    layer = (MglLayer *)malloc(sizeof(MglLayer));
    if (!layer)return NULL;
    memset(layer,0,sizeof(MglLayer));
    if (mgl_dict_get_hash_value_as_line(file, data, "image"))
    {
        layer->image = mgl_sprite_load_image(file);
    }
    if (mgl_dict_get_hash_value_as_line(file, data, "actor"))
    {
        layer->actor =  mgl_actor_load(file);
    }
    mgl_dict_get_hash_value_as_bool(&layer->cameraPlane, data, "cameraPlane");
    return layer;
}

MglBool mgl_parallax_load_resource(char *filename,void *data)
{
    MglParallax *par;
    MglLayer *layer;
    MglConfig *conf;
    MglDict *dict;
    MglDict *layerList;
    MglUint count,i;
    par = (MglParallax *)data;
    conf = mgl_config_load(filename);
    if (!conf)return MglFalse;
    dict = mgl_config_get_dictionary(conf);
    layerList = mgl_dict_get_hash_value(dict,"layers");
    
    /*parse the conf file*/
    count = mgl_dict_get_list_count(layerList);
    for (i = 0; i < count;i++)
    {
        layer = mgl_parallax_load_layer(mgl_dict_get_list_nth(layerList, i));
        par->layers = g_list_append(par->layers,layer);
        if (layer->cameraPlane)
        {
            par->cameraPlane = g_list_nth_data(par->layers,i);
        }
    }
    /*now calculate the offsets*/
    for (i = 0; i < count; i++)
    {
        layer = g_list_nth_data(par->layers,i);
        if (!layer)continue;
        if (layer == par->cameraPlane)
        {
            mgl_vec2d_set(layer->aspect,1,1);
            continue;
        }
    }
    return MglTrue;
}

MglParallax *mgl_parallax_new(char * filename)
{
    MglParallax * parallax;
    parallax = (MglParallax *)mgl_resource_manager_load_resource(__mgl_parallax_resource_manager,filename);
    if (!parallax)return NULL;
    return parallax;
}

/*eol@eof*/
