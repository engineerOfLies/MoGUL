#include "mgl_tileset.h"
#include "mgl_sprite.h"
#include "mgl_actor.h"
#include <glib.h>

/**
 * @brief information stored for each individual tile
 */
typedef struct
{
    MglVec2D    size;   /**<number of tiles this tile takes up, defaults to 1,1*/ 
    MglSprite * sprite; /**<if the tile graphic is described by a sprite*/
    MglUint     frame;  /**<if the sprite is used, this is the frame we go by*/
    MglActor  * actor;  /**<if the tile graphics is described by an actor*/
    MglDict   * info;   /**<any additional game specific data can be stored here*/
    MglVec4D    color;  /**<if the tile should be color shifted*/
}MglTileInfo;

/**
 * @brief stores information about a set of tiles to be used in a tile map
 */
struct MglTileSet_S
{
    MglVec2D  tileSize; /**<the size of an individual tile in pixel space, all tiles will
                            be scaled to fit this block size, though some tiles can take
                            up more than one*/
    GList *set;         /**<the list of tiles available for a tile set*/
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
    set = (MglTileSet
    for (it = set->set;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        mgl_tileinfo_delete((MglTileInfo *) it->data);
    }
    g_list_free(set);
}

MglTileInfo *mgl_tileset_load_info_from_dict(MglDict *def)
{
    MglTileInfo *tileInfo;
    if (!def)return NULL;
    tileInfo = g_new(MglTileInfo,1);
    if (!tileInfo)
    {
        mgl_logger_error("failed to allocate memory for tile info");
        return NULL;
    }
    memset(tileInfo,0,sizeof(MglTileInfo));
}

MglTileSet *mgl_tileset_load_from_dict(MglTileSet *tileset,MglDict *def)
{
    MglDict *list;
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
    list = mgl_dict_get_hash_value(def,"set");
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
    mgl_config_free(&sconf);
    if (!tileset)
    {
        return MglFalse;
    }
    return MglTrue;
}
/*eol@eof*/
