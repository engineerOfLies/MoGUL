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
    MglVec2D   scale;
    MglVec3D   rotation;
    MglVec2D   scaleCenter;
    MglVec2D   flip;
    MglVec4D   color;
    MglVec2D   size;  /**<size of the layer*/
    MglVec2D   offset;/**<position offset to draw layer*/
    MglVec2D   aspect;/**<the percent offset from the camera position this should be drawn at.  This is calculated*/
    MglBool    cameraPlane; /**<if true, this layer is drawn 1:1 to camera position*/
    MglBool    placeholder; /**<if true, this layer is calculated, but not drawn.  Used by other systems to keep in synce with parallax*/
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
    
    mgl_dict_get_hash_value_as_bool(&layer->placeholder,data,"placeholder");
    if (!layer->placeholder)
    {
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
    }
    /*if size is specified, it will override the image/actor size*/
    mgl_dict_get_hash_value_as_vec2d(&layer->size, data, "size");
    mgl_dict_get_hash_value_as_vec2d(&layer->flip, data, "flip");
    mgl_dict_get_hash_value_as_vec3d(&layer->rotation, data, "rotation");
    mgl_dict_get_hash_value_as_vec3d(&layer->rotation, data, "rotation");
    if (!mgl_dict_get_hash_value_as_vec2d(&layer->scale, data, "scale"))
    {
        layer->scale = mgl_vec2d(1,1);
    }
    if (!mgl_dict_get_hash_value_as_vec4d(&layer->color, data, "color"))
    {
        layer->color = mgl_vec4d(255,255,255,255);
    }
    mgl_dict_get_hash_value_as_vec2d(&layer->offset, data, "offset");
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

void mgl_parallax_layer_calc_aspect(MglLayer *layer,MglVec2D camerSize,MglVec2D cameraPlaneSize)
{
    layer->aspect.x = ((layer->size.x - camerSize.x) / (cameraPlaneSize.x - camerSize.x));
    layer->aspect.y = ((layer->size.y - camerSize.y) / (cameraPlaneSize.y - camerSize.y));    
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
        mgl_parallax_layer_calc_aspect(layer,mgl_vec2d(w,h),par->cameraPlane->size);
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

MglVec2D mgl_parallax_layer_adjust_position(MglParallax *par,MglUint l,MglVec2D position)
{
    MglLayer *layer;
    if (!par)
    {
        mgl_logger_error("no parallax background provided");
        return mgl_vec2d(0,0);
    }
    if (l >= par->layerCount)
    {
        mgl_logger_error("layer is out of range");
        return mgl_vec2d(0,0);
    }
    layer = g_list_nth_data(par->layers,l);
    if (!layer)
    {
        mgl_logger_error("layer %i does not exist",l);
        return mgl_vec2d(0,0);
    }
    return mgl_vec2d(layer->aspect.x * position.x,layer->aspect.y * position.y);
}

void mgl_parallax_draw_layer(MglParallax *par,MglUint l,MglVec2D position,MglVec4D * color)
{
    MglLayer *layer;
    MglVec4D drawColor;
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
    if (layer->placeholder)
    {
        return;
    }
    if (color)
    {
        drawColor = mgl_color_shift(layer->color, *color);
    }
    else
    {
        drawColor = layer->color;
    }
    if (layer->actor)
    {
        mgl_actor_draw(
            layer->actor,
            mgl_vec2d(layer->aspect.x * (position.x + layer->offset.x),layer->aspect.y * (position.y + layer->offset.y)),
            layer->rotation.z,
            &layer->scale,
            &layer->flip,
            &drawColor);
    }
    if (layer->image)
    {
         mgl_sprite_draw_image_full(
             layer->image,
             mgl_vec2d(layer->aspect.x * (position.x + layer->offset.x),layer->aspect.y * (position.y + layer->offset.y)),
             &layer->scale,
             &layer->scaleCenter,
             &layer->rotation,
             &layer->flip,
             &drawColor);
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
       mgl_parallax_draw_layer(par,i,position,NULL);
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

/**
 * @brief insert a new layer into the parallax background
 * @param par the parallax background to insert into
 * @param index the layer position to insert at
 * @param image [optional] a loaded sprite to draw
 * @param actor [optional] a loaded actor to draw
 * @param size [optional] if provided this will be the size of the layer
 * @param offset [optional] if provideds this will be the draw position offset
 * @param cameraPlane if true, the layer will move 1:1 relative to the camera
 * @param placeholder if true, this layer expects no graphics and is just used as a placeholder
 */
void mgl_parallax_insert_layer(
    MglParallax * par,
    MglUint       index,
    MglSprite   * image,
    MglActor    * actor,
    MglVec2D    * size,
    MglVec2D    * offset,
    MglBool       cameraPlane,
    MglBool       placeholder)
{
    MglUint w = 0,h = 0;
    MglUint sw = 0,sh = 0;
    MglLayer *layer;
    if (!par)
    {
        return;
    }
    /*build the layer*/
    layer = (MglLayer *)malloc(sizeof(MglLayer));
    if (!layer)
    {
        mgl_logger_error("failed to allocate layer data");
        return;
    }
    memset(layer,0,sizeof(MglLayer));
    layer->placeholder = placeholder;
    if (!placeholder)
    {
        if (image)
        {
            layer->image = image;
            mgl_sprite_get_size(layer->image,&sw,&sh);
            layer->size.x = sw;
            layer->size.y = sh;
        }
        if (actor)
        {
            layer->actor = actor;
            mgl_actor_get_size(layer->actor,&sw,&sh);
            layer->size.x = sw;
            layer->size.y = sh;
        }
    }
    if (size)
    {
        layer->size.x = size->x;
        layer->size.y = size->y;
    }
    if (offset)
    {
        layer->offset.x = offset->x;
        layer->offset.y = offset->y;
    }
    layer->cameraPlane = cameraPlane;
    
    if (par->cam)
    {
        mgl_camera_get_size(par->cam,&w, &h);
    }
    mgl_parallax_layer_calc_aspect(layer,mgl_vec2d(w,h),par->cameraPlane->size);
    
    /*add it to the rest*/
    par->layers = g_list_insert(par->layers,layer,index);
}

/*eol@eof*/
