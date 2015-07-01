#include "mgl_tilemap.h"
#include "mgl_tileset.h"
#include "mgl_graphics.h"
#include "mgl_tileset.h"
#include "mgl_config.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include <glib.h>



struct MglTileMap_S
{
    MglTileSet *tileSet;    /**<the tile set to use for this map*/
    MglUint mapWidth;       /**<number of tiles horizontally*/
    MglUint mapHeight;      /**<number of tiles vertically*/
    SDL_Surface *surface;   /**<precached surface*/
    SDL_Texture *texture;   /**<precached texture*/
    MglInt *tileMap;        /**<tile map data, 0 implies empty.  tilemap[i] -1 -> tileset index*/
};


static MglResourceManager * __mgl_tilemap_resource_manager = NULL;
void mgl_tilemap_close();
MglBool mgl_tilemap_load_resource(char *filename,void *data);
void mgl_tilemap_delete(void *data);

void mgl_tilemap_init(
    MglUint maxMaps)
{
    __mgl_tilemap_resource_manager = mgl_resource_manager_init(
        "mgl tilemap",
        maxMaps,
        sizeof(struct MglTileMap_S),
        MglFalse,
        mgl_tilemap_delete,
        mgl_tilemap_load_resource
    );
    atexit(mgl_tilemap_close);
}

void mgl_tilemap_close()
{
    mgl_resource_manager_free(&__mgl_tilemap_resource_manager);
}

void mgl_tilemap_delete(void *data)
{
    MglTileMap *map;
    if (!data)return;
    map = (MglTileMap*)data;
    if (map->tileMap)free(map->tileMap);
    if (map->surface)
    {
        SDL_FreeSurface(map->surface);
    }
    if (map->texture)
    {
        SDL_DestroyTexture(map->texture);
    }
    mgl_tileset_free(&map->tileSet);
}

MglTileMap *mgl_tilemap_load(char * filename)
{
    MglTileMap *tilemap;
    tilemap = mgl_resource_manager_load_resource(__mgl_tilemap_resource_manager,filename);
    return tilemap;
}

void mgl_tilemap_free(MglTileMap **tilemap)
{
    mgl_resource_free_element(__mgl_tilemap_resource_manager,(void **)tilemap);
}

void mgl_tilemal_render_surface(MglTileMap *tilemap)
{
    MglUint i,j;
    MglVec2D size;
    if (!tilemap)return;
    if (!tilemap->tileMap)return;
    if (!tilemap->tileSet)return;
    mgl_tileset_get_tile_size(tilemap->tileSet, &size);
    if (!tilemap->surface)
    {
        tilemap->surface = mgl_graphics_create_surface(tilemap->mapWidth,tilemap->mapHeight);
        if (!tilemap->surface)return;
    }
    if ((tilemap->surface->w != tilemap->mapWidth)||
        (tilemap->surface->h != tilemap->mapHeight))
    {
        SDL_FreeSurface(tilemap->surface);
        tilemap->surface = mgl_graphics_create_surface(tilemap->mapWidth,tilemap->mapHeight);
        if (!tilemap->surface)return;
    }
    /*clear the image*/
    mgl_draw_solid_rect_to_surface(
        tilemap->surface,
        mgl_rect(0,0,tilemap->surface->w,tilemap->surface->h),
        mgl_vec4d(0,0,0,0));
    
    for (j = 0;j < tilemap->mapHeight;j++)
    {
        for (i = 0;i < tilemap->mapWidth;i++)
        {
            if (tilemap->tileMap[(j * tilemap->mapWidth)+i] > 0)
            {
                mgl_tileset_draw_tile_to_surface(
                    tilemap->tileSet,
                    (MglUint)tilemap->tileMap[(j * tilemap->mapWidth)+i] - 1,
                    tilemap->surface,
                    mgl_vec2d(i * size.x,j*size.y));
            }
        }
    }
}

MglTileMap *mgl_tilemap_load_from_dict(MglTileMap *tilemap,MglDict *def)
{
    MglDict *map,*row;
    MglLine filename;
    MglTileSet *tileSet;
    MglUint count,i,j;
    MglUint mapWidth,mapHeight;
    MglInt *tileData;
    const char *tiles;
    if (!def)
    {
        return NULL;
    }
    if (mgl_dict_get_hash_value_as_line(filename,def,"tileSet"))
    {
        tileSet = mgl_tileset_load(filename);
        if (tileSet == NULL)
        {
            mgl_logger_warn("could not load tileSet %s! Cannot create map!",filename);
            return NULL;
        }
    }
    else
    {
        mgl_logger_warn("no tileset specified in map",filename);
        return NULL;
    }
    map = mgl_dict_get_hash_value(def,"tileMap");
    if (!map)
    {        
        mgl_logger_warn("no tileMap specified in map",filename);
        return NULL;
    }
    mgl_dict_get_hash_value_as_uint(&mapWidth, def, "mapWidth");
    mgl_dict_get_hash_value_as_uint(&mapHeight, def, "mapHeight");

    tileData = g_new(MglInt,(tilemap->mapHeight*tilemap->mapWidth));
    if (!tileData)
    {
        mgl_logger_warn("failed to allocat tile data");
        return NULL;
    }

    if (!tilemap)
    {
        tilemap = mgl_resource_new_element(__mgl_tilemap_resource_manager);
        if (!tilemap)
        {
            return NULL;
        }
    }
    tilemap->tileSet = tileSet;
    tilemap->tileMap = tileData;
    count = mgl_dict_get_list_count(map);
    if (count != tilemap->mapHeight)
    {
        mgl_logger_warn("row count does not match mapHeight");
    }
    
    for (j = 0; j < count;j++)
    {
        row = mgl_dict_get_hash_value(mgl_dict_get_list_nth(map,j),"row");
        if (!row)continue;
        tiles = mgl_dict_get_string(row);
        if (!tiles)continue;
        for (i = 0;i < strlen(tiles);i++)
        {
            sscanf(tiles,"%i",&tileData[(j*mapWidth)+i]);
            if (i < (strlen(tiles) - 1))
            {   
                tiles = strstr(tiles,",");
                tiles++;
            }
        }
    }
    return tilemap;
}

MglBool mgl_tilemap_load_resource(char *filename,void *data)
{
    MglTileMap *tilemap;
    MglConfig *conf;
    MglDict *def;
    if (!data)
    {
        return MglFalse;
    }
    tilemap = (MglTileMap *)data;
    conf = mgl_config_load(filename);
    if (!conf)
    {
        return MglFalse;
    }
    def = mgl_config_get_dictionary(conf);
    tilemap = mgl_tilemap_load_from_dict(tilemap,def);    
    mgl_config_free(&conf);
    if (!tilemap)
    {
        return MglFalse;
    }
    return MglTrue;
}

/*eol@eof*/
