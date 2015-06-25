#include "mgl_parallax.h"
#include "mgl_resource.h"

#include "mgl_sprite.h"
#include "mgl_actor.h"
#include "mgl_logger.h"
#include "mgl_config.h"

#include <glib.h>

/**
 * @brief the layer type holds information on how to draw a given layer of the parallax background
 */
typedef struct
{
    MglSprite *image; /**<the image to draw*/
    MglActor  *actor; /**<optionally an actor can be used instead*/
    MglVec2D   size;  /**<size of the layer*/
    MglVec2D   aspect;/**<the percent offset from the camera position this should be drawn at.  This is calculated*/
    MglBool    cameraPlane; /**<if true, this layer is drawn 1:1 to camera position*/
}MglLayer;

/**
 * @brief a parallax background context
 */
struct MglParallax_S
{
    GList    *layers;      /**<the list of layers that make up this parallax background context*/
    MglLayer *cameraPlane; /**<the layer that is drawn 1:1 to the camera / All other planes are moved accordingly*/
    MglUint   cameraLayer; /**<the layer index for the camera plane*/
    MglUint   layerCount;  /**<how many layers are loaded*/
    MglCamera *cam;        /**<aspect ratios are calculated based on this camera*/
};


static MglResourceManager * __mgl_parallax_resource_manager = NULL;
static MglCamera          * __mgl_parallax_default_camera = NULL;
void mgl_parallax_close();
MglBool mgl_parallax_load_resource(char *filename,void *data);
void mgl_parallax_delete(void *data);

void mgl_parallax_init(
    MglUint maxParallaxs,
    MglCamera * defaultCamera)
{
    __mgl_parallax_resource_manager = mgl_resource_manager_init(
        "mgl parallax",
        maxParallaxs,
        sizeof(struct MglParallax_S),
        MglFalse,
        mgl_parallax_delete,
        mgl_parallax_load_resource
    );
    __mgl_parallax_default_camera = defaultCamera;
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
    MglUint sw,sh;
    if (!data)return NULL;
    layer = (MglLayer *)malloc(sizeof(MglLayer));
    if (!layer)
    {
        mgl_logger_error("failed to allocate layer data");
        return NULL;
    }
    memset(layer,0,sizeof(MglLayer));
    if (mgl_dict_get_hash_value_as_line(file, data, "image"))
    {
        mgl_logger_info("loading layer image: %s",file);
        layer->image = mgl_sprite_load_image(file);
        mgl_sprite_get_size(layer->image,&sw,&sh);
        layer->size.x = sw;
        layer->size.y = sh;
    }
    if (mgl_dict_get_hash_value_as_line(file, data, "actor"))
    {
        mgl_logger_info("loading layer actor: %s",file);
        layer->actor =  mgl_actor_load(file);
        mgl_actor_get_size(layer->actor,&sw,&sh);
        layer->size.x = sw;
        layer->size.y = sh;
    }
    mgl_dict_get_hash_value_as_bool(&layer->cameraPlane, data, "cameraPlane");
    return layer;
}

void mgl_parallax_change_camera(MglParallax *par,MglCamera *cam)
{
    if (!par)
    {
        return;
    }
    par->cam = cam;
    mgl_parallax_change_camera_plane(par, par->cameraLayer);
}

void mgl_parallax_change_camera_plane(MglParallax *par, MglUint n)
{
    MglLayer *layer;
    int i;
    MglUint w = 0,h = 0;
    if (!par)
    {
        return;
    }
    if (par->cam)
    {
        mgl_camera_get_size(par->cam,&w, &h);
    }
    par->cameraPlane = g_list_nth_data(par->layers,n);
    if (!par->cameraPlane)
    {
        return;
    }
    if ((par->cameraPlane->size.x == 0) || (par->cameraPlane->size.y == 0))
    {
        /*cant be dividing by zero*/
        return;
    }
    par->cameraPlane->aspect.x = 1;
    par->cameraPlane->aspect.y = 1;
    for (i = 0;i < par->layerCount;i++)
    {
        layer = g_list_nth_data(par->layers,i);
        if (layer == par->cameraPlane)continue;
        layer->aspect.x = ((layer->size.x - w) / (par->cameraPlane->size.x - w));
        layer->aspect.y = ((layer->size.y - h) / (par->cameraPlane->size.y - h));
    }
}

MglBool mgl_parallax_load_resource(char *filename,void *data)
{
    MglParallax *par;
    MglLayer *layer;
    MglConfig *conf;
    MglDict *dict;
    MglDict *layerList;
    MglInt cameraLayer = -1;
    MglUint count,i;
    par = (MglParallax *)data;
    conf = mgl_config_load(filename);
    if (!conf)return MglFalse;
    dict = mgl_config_get_dictionary(conf);
    layerList = mgl_dict_get_hash_value(dict,"layers");
    if (!layerList)
    {
        mgl_logger_warn("parallax config contains no layers object");
    }
    
    /*parse the conf file*/
    count = mgl_dict_get_list_count(layerList);
    for (i = 0; i < count;i++)
    {
        layer = mgl_parallax_load_layer(mgl_dict_get_list_nth(layerList, i));
        par->layers = g_list_append(par->layers,layer);
        if (layer->cameraPlane)
        {
            cameraLayer = i;
            par->cameraLayer = i;
        }
    }
    par->cam = __mgl_parallax_default_camera;
    par->layerCount = count;
    if (cameraLayer != -1)
    {
        mgl_parallax_change_camera_plane(par, (MglUint)cameraLayer);
    }
    return MglTrue;
}

MglParallax *mgl_parallax_load(char * filename,MglCamera *cam)
{
    MglParallax * par;
    par = (MglParallax *)mgl_resource_manager_load_resource(__mgl_parallax_resource_manager,filename);
    if (!par)return NULL;
    if (cam)
    {
        mgl_parallax_change_camera(par,cam);
    }
    return par;
}

void mgl_parallax_draw_layer(MglParallax *par,MglUint l,MglVec2D position)
{
    MglLayer *layer;
    if (!par)
    {
        mgl_logger_error("no parallax background provided");
        return;
    }
    if (l >= par->layerCount)
    {
        mgl_logger_error("layer is out of range");
        return;
    }
    layer = g_list_nth_data(par->layers,l);
    if (!layer)
    {
        mgl_logger_error("layer %i does not exist",l);
        return;
    }
    if (layer->actor)
    {
        mgl_actor_draw(
            layer->actor,
            mgl_vec2d(layer->aspect.x * position.x,layer->aspect.y * position.y),
            0,
            NULL,
            NULL,
            NULL);
    }
    if (layer->image)
    {
        mgl_logger_info("drawing layer %i at position (%f,%f)",l,layer->aspect.x * position.x,layer->aspect.y * position.y);
         mgl_sprite_draw_image(
             layer->image,
             mgl_vec2d(layer->aspect.x * position.x,layer->aspect.y * position.y));
    }
}

void mgl_parallax_draw_all_layers(MglParallax *par,MglVec2D position)
{
    int i;
    if (!par)
    {
        mgl_logger_error("no parallax background provided");
        return;
    }
    for (i = 0;i < par->layerCount; i++)
    {
       mgl_parallax_draw_layer(par,i,position); 
    }
}

void mgl_parallax_get_size(MglParallax *par,MglUint *w,MglUint *h)
{
    if (!par)
    {
        mgl_logger_error("no parallax background provided");
        return;
    }
    if (!par->cameraPlane)
    {
        mgl_logger_error("no parallax camera plane defined");
        return;
    }
    if (w)
    {
        *w = par->cameraPlane->size.x;
    }
    if (h)
    {
        *h = par->cameraPlane->size.y;
    }
}

/*eol@eof*/
