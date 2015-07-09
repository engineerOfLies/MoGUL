#include "mgl_layer.h"
#include "mgl_logger.h"
#include "mgl_dict.h"

void mgl_layer_free(MglLayer **layer)
{
    if ((!layer)||(!*layer))return;
    switch((*layer)->selection)
    {
        case MglLayerNone:
            break;
        case MglLayerTiles:
            mgl_tilemap_free(&(*layer)->layer.map);
            break;
        case MglLayerParallax:
            /*parallax context is owned by the level*/
            break;
        case MglLayerImage:
            mgl_sprite_free(&(*layer)->layer.image);
            break;
        case MglLayerCollision:
            /*free the collision mask*/
            break;
    }
}

MglLayer *mgl_layer_new()
{
    MglLayer *layer;
    layer = (MglLayer*)malloc(sizeof(MglLayer));
    if (!layer)
    {
        mgl_logger_error("failed to allocate a new level layer!");
        return NULL;
    }
    memset(layer,0,sizeof(MglLayer));
    return layer;
}

MglLayer *mgl_layer_load_from_def(MglDict *def)
{
    MglLine layerType;
    MglParallax *par;
    MglTileMap *map;
    MglUint     selection = MglLayerNone;
    if (!def)return NULL;
    if (!mgl_dict_get_hash_value_as_line(layerType,def,"layerType"))
    {
        return NULL;
    }
    if (mgl_line_cmp(layerType,"parallax")==0)
    {
        par = mgl_parallax_load_from_def(mgl_dict_get_hash_value(def,"parallax"));
        if (!par)
        {
            return NULL;
        }
        selection = MglLayerParallax;
    }
    else if (mgl_line_cmp(layerType,"tilemap")==0)
    {
        map = mgl_tilemap_load_from_def(mgl_dict_get_hash_value(def,"tilemap"));
        if (!map)
        {
            return NULL;
        }
        selection = MglLayerTiles;
    }
    else if (mgl_line_cmp(layerType,"image")==0)
    {
        selection = MglLayerImage;
    }
    else if (mgl_line_cmp(layerType,"collision")==0)
    {
        selection = MglLayerCollision;
    }
    return NULL;
}

MglLayer *mgl_layer_new_tile_layer(
    MglTileMap *map,
    MglBool useParallax,
    MglUint bglayer
)
{
    MglLayer *layer;
    layer = mgl_layer_new();
    if (!layer)return NULL;
    layer->selection = MglLayerTiles;
    layer->layer.map = map;
    layer->useParallax = useParallax;
    layer->bglayer = bglayer;
    return layer;
}

MglLayer *mgl_layer_new_parallax_layer(
    MglParallax *par,
    MglBool useParallax,
    MglUint bglayer
)
{
    MglLayer *layer;
    layer = mgl_layer_new();
    if (!layer)return NULL;
    layer->selection = MglLayerParallax;
    layer->layer.par = par;
    layer->useParallax = useParallax;
    layer->bglayer = bglayer;
    return layer;
}

MglLayer *mgl_layer_new_image_layer(
    MglSprite *image,
    MglBool useParallax,
    MglUint bglayer
)
{
    MglLayer *layer;
    layer = mgl_layer_new();
    if (!layer)return NULL;
    layer->selection = MglLayerImage;
    layer->layer.image = image;
    layer->useParallax = useParallax;
    layer->bglayer = bglayer;
    return layer;
}

void mgl_layer_draw(MglLayer *layer,MglParallax *par,MglCamera *cam,MglVec2D position)
{
    if (!layer)return;
    switch(layer->selection)
    {
        case MglLayerNone:
            return;
        case MglLayerTiles:
            if ((layer->useParallax) && (par != NULL))
            {
                mgl_tilemap_draw(
                    layer->layer.map, 
                    mgl_parallax_layer_adjust_position(
                        par,
                        layer->bglayer,
                        mgl_camera_get_adjusted_position(cam,position))
                );
            }
            else
            {
                mgl_tilemap_draw(layer->layer.map, mgl_camera_get_adjusted_position(cam,position));
            }
            return;
        case MglLayerParallax:
            if (par)
            {
                mgl_parallax_draw_all_layers(
                    layer->layer.par,
                    mgl_parallax_layer_adjust_position(
                        par,
                        layer->bglayer,
                        mgl_camera_get_adjusted_position(cam,position)));
            }
            else
            {
                mgl_parallax_draw_layer(
                    layer->layer.par,
                    layer->bglayer,
                    mgl_camera_get_adjusted_position(cam,position),NULL);
            }
            return;
        case MglLayerImage:
            if ((layer->useParallax) && (par != NULL))
            {
                mgl_sprite_draw(
                    layer->layer.image,
                    mgl_parallax_layer_adjust_position(
                        par,
                        layer->bglayer,
                        mgl_camera_get_adjusted_position(cam,position)),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            else
            {
                mgl_sprite_draw(
                    layer->layer.image,
                    mgl_camera_get_adjusted_position(cam,position),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
            }
            return;
        case MglLayerCollision:
            return;
    }
}

/*eol@eof*/
