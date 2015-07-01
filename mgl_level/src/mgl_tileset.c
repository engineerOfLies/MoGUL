#include "mgl_tileset.h"
#include "mgl_sprite.h"
#include "mgl_tileset.h"
#include "mgl_config.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include "mgl_actor.h"
#include <glib.h>

/**
 * @brief information stored for each individual tile
 */
typedef struct
{
    MglVec2D    footprint;  /**<number of tiles this tile takes up, defaults to 1,1*/ 
    MglVec2D    offset;     /**<drawing position offset for non-standard tiles*/
    MglVec2D    scale;      /**<scaling factor for drawing*/
    MglSprite * sprite;     /**<if the tile graphic is described by a sprite*/
    MglUint     frame;      /**<if the sprite is used, this is the frame we go by*/
    MglActor  * actor;      /**<if the tile graphics is described by an actor*/
    MglVec4D    color;      /**<if the tile should be color shifted*/
    MglBool     solid;      /**<MglTrue if the tile is treated as a solid wall*/
    MglDict   * info;       /**<any additional game specific data can be stored here*/
}MglTileInfo;

/**
 * @brief stores information about a set of tiles to be used in a tile map
 */
struct MglTileSet_S
{
    MglVec2D  tileSize; /**<the size of an individual tile in pixel space, all tiles will
                            be scaled to fit this block size, though some tiles can take
                            up more than one*/
    GList *tileSet;     /**<the list of tiles available for a tile set*/
    MglDict   * info;   /**<any additional game specific data can be stored here*/
};

static MglResourceManager * __mgl_tileset_resource_manager = NULL;
void mgl_tileset_close();
MglBool mgl_tileset_load_resource(char *filename,void *data);
void mgl_tileset_delete(void *data);

void mgl_tileset_init(
    MglUint maxSets)
{
    __mgl_tileset_resource_manager = mgl_resource_manager_init(
        "mgl tileset",
        maxSets,
        sizeof(struct MglTileSet_S),
        MglFalse,
        mgl_tileset_delete,
        mgl_tileset_load_resource
    );
    atexit(mgl_tileset_close);
}

void mgl_tileset_close()
{
    mgl_resource_manager_free(&__mgl_tileset_resource_manager);
}

void mgl_tileinfo_delete(MglTileInfo * tile)
{
    if (!tile)return;
    mgl_actor_free(&tile->actor);
    mgl_sprite_free(&tile->sprite);
    mgl_dict_free(&tile->info);
    free(tile);
}

void mgl_tileset_delete(void *data)
{
    GList *it;
    MglTileSet *set;
    if (!data)return;
    set = (MglTileSet*)data;
    for (it = set->tileSet;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        mgl_tileinfo_delete((MglTileInfo *) it->data);
    }
    g_list_free(set->tileSet);
    mgl_dict_free(&set->info);
}

MglTileInfo *mgl_tileset_load_info_from_dict(MglDict *def)
{
    MglTileInfo *tileInfo;
    MglLine filename;
    MglDict *info;
    if (!def)return NULL;
    tileInfo = g_new(MglTileInfo,1);
    if (!tileInfo)
    {
        mgl_logger_error("failed to allocate memory for tile info");
        return NULL;
    }
    memset(tileInfo,0,sizeof(MglTileInfo));
    if (!mgl_dict_get_hash_value_as_vec2d(&tileInfo->scale, def, "scale"))
    {
        mgl_vec2d_set(tileInfo->scale,1,1);
    }
    if (!mgl_dict_get_hash_value_as_vec2d(&tileInfo->footprint, def, "footprint"))
    {
        mgl_vec2d_set(tileInfo->footprint,1,1);
    }
    if (!mgl_dict_get_hash_value_as_vec4d(&tileInfo->color, def, "color"))
    {
        mgl_vec4d_set(tileInfo->color,255,255,255,255);
    }
    if (mgl_dict_get_hash_value_as_line(filename, def, "image"))
    {
        tileInfo->sprite = mgl_sprite_load_image(filename);
        mgl_dict_get_hash_value_as_uint(&tileInfo->frame, def, "frame");
    }
    if (mgl_dict_get_hash_value_as_line(filename, def, "sprite"))
    {
        tileInfo->sprite = mgl_sprite_load_from_def(filename);
        mgl_dict_get_hash_value_as_uint(&tileInfo->frame, def, "frame");
    }
    else if (mgl_dict_get_hash_value_as_line(filename, def, "actor"))
    {
        tileInfo->actor = mgl_actor_load(filename);
        if (mgl_dict_get_hash_value_as_line(filename, def, "action"))
        {
            mgl_actor_set_action(tileInfo->actor,filename);
        }
    }

    info = mgl_dict_get_hash_value(def,"info");
    if (info)
    {
        tileInfo->info = mgl_dict_clone(info);
    }
    return tileInfo;
}

MglTileSet *mgl_tileset_load_from_dict(MglTileSet *tileset,MglDict *def)
{
    MglDict *list,*info;
    MglTileInfo *tileInfo;
    MglUint i,count;
    if (!def)
    {
        mgl_logger_error("no dictionary provided");
        return NULL;
    }
    if (!tileset)
    {
        tileset = (MglTileSet*)mgl_resource_new_element(__mgl_tileset_resource_manager);
        if (!tileset)
        {
            return NULL;
        }
    }
    mgl_dict_get_hash_value_as_vec2d(&tileset->tileSize, def, "tileSize");
    list = mgl_dict_get_hash_value(def,"tileSet");
    count = mgl_dict_get_list_count(list);
    for (i = 0;i < count;i++)
    {
        tileInfo = mgl_tileset_load_info_from_dict(mgl_dict_get_list_nth(list,i));
        if (!tileInfo)continue;
        tileset->tileSet = g_list_append(tileset->tileSet,tileInfo);
    }
    info = mgl_dict_get_hash_value(def,"info");
    if (info)
    {
        tileset->info = mgl_dict_clone(info);
    }

    return tileset;
}

MglBool mgl_tileset_load_resource(char *filename,void *data)
{
    MglTileSet *tileset;
    MglConfig *conf;
    MglDict *def;
    if (!data)
    {
        return MglFalse;
    }
    tileset = (MglTileSet *)data;
    conf = mgl_config_load(filename);
    if (!conf)
    {
        return MglFalse;
    }
    def = mgl_config_get_dictionary(conf);
    tileset = mgl_tileset_load_from_dict(tileset,def);    
    mgl_config_free(&conf);
    if (!tileset)
    {
        return MglFalse;
    }
    return MglTrue;
}

MglTileSet *mgl_tileset_load(char * filename)
{
    MglTileSet *tileset;
    tileset = mgl_resource_manager_load_resource(__mgl_tileset_resource_manager,filename);
    return tileset;
}

void mgl_tileset_free(MglTileSet **tileset)
{
    mgl_resource_free_element(__mgl_tileset_resource_manager,(void **)tileset);
}

MglTileInfo *mgl_tileset_get_tileinfo(MglTileSet *tileset,MglUint index)
{
    if (!tileset)return NULL;
    return (MglTileInfo*)g_list_nth_data(tileset->tileSet,index);
}

void mgl_tileset_draw_tile(MglTileSet *tileset,MglUint index,MglVec2D position)
{
    MglTileInfo *tileInfo;
    tileInfo = mgl_tileset_get_tileinfo(tileset,index);
    if (!tileInfo)return;
    if (tileInfo->sprite)
    {
        mgl_sprite_draw(
            tileInfo->sprite,
            position,
            &tileInfo->scale,
            NULL,
            NULL,
            NULL,
            &tileInfo->color,
            tileInfo->frame);
    }
    else if (tileInfo->actor)
    {
        mgl_actor_draw(
            tileInfo->actor,
            position,
            0,
            &tileInfo->scale,
            NULL,
            &tileInfo->color);
    }
}

void mgl_tileset_draw_tile_to_surface(MglTileSet *tileset,MglUint index,SDL_Surface *surface,MglVec2D position)
{
    MglTileInfo *tileInfo;
    tileInfo = mgl_tileset_get_tileinfo(tileset,index);
    if (!tileInfo)return;
    if (tileInfo->sprite)
    {
        mgl_sprite_draw_to_surface(
            surface,
            tileInfo->sprite,
            position,
            &tileInfo->scale,
            NULL,
            &tileInfo->color,
            tileInfo->frame);
    }
    else if (tileInfo->actor)
    {
        mgl_actor_draw_to_surface(
            tileInfo->actor,
            position,
            &tileInfo->scale,
            &tileInfo->color,
            surface);
    }
}

MglDict *mgl_tileset_get_custom_info(MglTileSet *set)
{
    if (!set)return NULL;
    return set->info;
}

MglDict *mgl_tileset_get_tile_custom_info(MglTileSet *set,MglUint index)
{
    MglTileInfo *tileInfo;
    tileInfo = mgl_tileset_get_tileinfo(set,index);
    if (!tileInfo)return NULL;
    return tileInfo->info;
}

MglBool mgl_tileset_tile_solid(MglTileSet *set,MglUint index)
{
    MglTileInfo *tileInfo;
    tileInfo = mgl_tileset_get_tileinfo(set,index);
    if (!tileInfo)
    {
        mgl_logger_warn("tile %s not found in set",index);
        return MglFalse;
    }
    return tileInfo->solid;
}
/*eol@eof*/
