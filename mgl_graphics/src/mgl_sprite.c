#include "mgl_sprite.h"
#include "mgl_dict.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_resource.h"
#include "mgl_graphics.h"

#include <SDL.h>
#include <SDL_image.h>

static MglResourceManager * __mgl_sprite_resource_manager = NULL;
static MglUint __mgl_sprite_default_fpl = 16;

struct MglSprite_S
{
    SDL_Surface *image;
    
    MglUint frameWidth;
    MglUint frameHeight;
    MglUint framesPerLine;
    
    MglSI64 redSwap;
    MglSI64 greenSwap;
    MglSI64 blueSwap;
};

#define SPRITE(data) (MglSprite*)data

void mgl_sprite_close();
MglBool mgl_sprite_load_resource(char *filename,void *data);
static void mgl_sprite_swap_colors(MglSprite *sprite);
void mgl_sprite_delete(void *data);

void mgl_sprite_init_from_config(char * configFile)
{
    MglUint maxSprites = 100,defaultFPL;
    MglDict *data = NULL;
    MglConfig *config = NULL;
    
    if (!configFile)
    {
        mgl_logger_error("mgl_sprite_init_from_config: failed to provide config file to load");
        return;
    }
    config = mgl_config_load(configFile);
    if (!config)
    {
        mgl_logger_error("mgl_sprite_init_from_config: failed to load config file: %s",configFile);
        return;
    }
    data = mgl_config_get_dictionary(config);
    
    mgl_dict_get_hash_value_as_uint(&maxSprites, data, "maxSprites");
    mgl_dict_get_hash_value_as_uint(&defaultFPL, data, "defaultFramesPerLine");
    mgl_config_free(&config);
    mgl_sprite_init(
        maxSprites,
        defaultFPL);
}

void mgl_sprite_init(
    MglUint maxSprites,
    MglUint defaultFramesPerLine)
{
    if (!(IMG_Init( IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        mgl_logger_error("mgl_sprite_init: failed to init image: %s",SDL_GetError());
    }
    atexit(IMG_Quit);
    __mgl_sprite_resource_manager = mgl_resource_manager_init(
        "mgl sprite",
        maxSprites,
        sizeof(struct MglSprite_S),
        MglFalse,
        mgl_sprite_delete,
        mgl_sprite_load_resource
    );
    __mgl_sprite_default_fpl = defaultFramesPerLine;
    atexit(mgl_sprite_close);
}

void mgl_sprite_close()
{
    mgl_resource_manager_free(&__mgl_sprite_resource_manager);
}

MglBool mgl_sprite_load_resource(char *filename,void *data)
{
    SDL_Surface *image;
    MglSprite *sprite;
    char ** strings;
    MglLine fname;
    MglUint fw,fh,fpl;
    MglSI64 red,green,blue;
    sprite = SPRITE(data);
    if (!sprite)
    {
        mgl_logger_error("mgl_sprite_load_resource: NULL data provided for sprite %s",filename);
        return MglFalse;
    }
    /*parse parameters packed with sprite*/
    strings = g_strsplit_set (filename,
                    "|",
                    0);
    mgl_line_cpy(fname,strings[0]);
    fw = atoi(strings[1]);
    fh = atoi(strings[2]);
    fpl = atoi(strings[3]);
    red = atoi(strings[4]);
    green = atoi(strings[5]);
    blue = atoi(strings[6]);
    g_strfreev (strings);
    
    sprite->frameWidth = fw;
    sprite->frameHeight = fh;
    sprite->framesPerLine = fpl;
    
    sprite->redSwap = red;
    sprite->greenSwap = green;
    sprite->blueSwap = blue;
    
    image = IMG_Load(fname);
    if (!image)
    {
        mgl_logger_warn("mgl_sprite_load_resource:failed to load sprite image file: %s, re: %s",fname, SDL_GetError());
        return MglFalse;
    }
    sprite->image = mgl_graphics_screen_convert(&image);
    if (!sprite->image)
    {
        return MglFalse;
    }
    /*set the rest of the data*/
    if ((red != -1)||
        (green != -1)||
        (blue != -1))
    {
        mgl_sprite_swap_colors(sprite);
    }
    
    return MglTrue;
}

void mgl_sprite_delete(void *data)
{
    MglSprite *sprite;
    sprite = (MglSprite *)data;
    SDL_FreeSurface(sprite->image);
    sprite->image = NULL;
}


static void mgl_sprite_swap_colors(MglSprite *sprite)
{
    int i,j;
    Uint8 r,g,b,a;
    MglUint color;
    MglFloat intensity;
    MglVec4D redShift,greenShift,blueShift;
    if (SDL_MUSTLOCK(sprite->image))
    {
        SDL_LockSurface(sprite->image);
    }
    SDL_GetRGBA(sprite->redSwap,
                sprite->image->format,
                &r,
                &g,
                &b,
                &a);
    mgl_vec4d_set(redShift,r,g,b,a);

    SDL_GetRGBA(sprite->greenSwap,
                sprite->image->format,
                &r,
                &g,
                &b,
                &a);
    mgl_vec4d_set(greenShift,r,g,b,a);
    
    SDL_GetRGBA(sprite->blueSwap,
                sprite->image->format,
                &r,
                &g,
                &b,
                &a);
    mgl_vec4d_set(blueShift,r,g,b,a);
    
    for (j = 0;j < sprite->image->h;j++)
    {
        for (i = 0;i < sprite->image->w;i++)
        {
            color = mgl_graphics_get_surface_pixel(sprite->image,mgl_vec2d(i,j));
            SDL_GetRGBA(color,
                        sprite->image->format,
                        &r,
                        &g,
                        &b,
                        &a);
            if ((r != 0)&&
                (g == 0)&&
                (b == 0)&&
                (sprite->redSwap != -1))
            {/*pure red*/
                intensity = (MglFloat)r / 255;
                r = (Uint8)intensity * redShift.x;
                g = (Uint8)intensity * redShift.y;
                b = (Uint8)intensity * redShift.z;
            }
            if ((r == 0)&&
                (g != 0)&&
                (b == 0)&&
                (sprite->blueSwap != -1))
            {/*pure red*/
                intensity = (MglFloat)g / 255;
                r = (Uint8)intensity * greenShift.x;
                g = (Uint8)intensity * greenShift.y;
                b = (Uint8)intensity * greenShift.z;
            }
            if ((r == 0)&&
                (g == 0)&&
                (b != 0)&&
                (sprite->blueSwap != -1))
            {/*pure red*/
                intensity = (MglFloat)b / 255;
                r = (Uint8)intensity * blueShift.x;
                g = (Uint8)intensity * blueShift.y;
                b = (Uint8)intensity * blueShift.z;
            }
            color = SDL_MapRGBA(sprite->image->format,r,g,b,a);
            mgl_graphics_set_surface_pixel(sprite->image,mgl_vec2d(i,j),color);
        }
    }
    if (SDL_MUSTLOCK(sprite->image))
    {
        SDL_UnlockSurface(sprite->image);
    }
}

MglSprite *mgl_sprite_load_from_image(
    char *filename,
    MglInt frameWidth,
    MglInt frameHeight,
    MglUint framesPerLine,
    MglVec4D *redSwap,
    MglVec4D *greenSwap,
    MglVec4D *blueSwap)
{
    char *filenamePacked;
    MglSprite *sprite = NULL;
    filenamePacked = g_strdup_printf (
        "%s|%i|%i|%i|%i|%i|%i",
        filename,
        frameWidth,
        frameHeight,
        framesPerLine,
        redSwap?mgl_graphics_vec_to_screen_color(*redSwap):-1,
        greenSwap?mgl_graphics_vec_to_screen_color(*greenSwap):-1,
        blueSwap?mgl_graphics_vec_to_screen_color(*blueSwap):-1);
    sprite = mgl_resource_manager_load_resource(__mgl_sprite_resource_manager,filenamePacked);
    free(filenamePacked);
    if (!sprite)
    {
        mgl_logger_warn("mgl_sprite_load_from_image: failed to load image:%s",filename);
        return NULL;
    }
        
    return sprite;
}

void mgl_sprite_draw(MglSprite *sprite, MglVec2D position,MglUint frame)
{
    MglRect cell;
    if (!sprite)
    {
        return;
    }
    mgl_logger_warn("frame: %i",frame);
    mgl_rect_set(
        &cell,
        frame%sprite->framesPerLine * sprite->frameWidth,
        frame/sprite->framesPerLine * sprite->frameHeight,
        sprite->frameWidth,
        sprite->frameHeight);
    mgl_graphics_render_surface_to_screen(sprite->image,cell,position,mgl_vec2d(1,1),mgl_vec3d(0,0,0));
}

void mgl_sprite_draw_to_surface(SDL_Surface *surface, MglSprite *sprite, MglVec2D position,MglUint frame)
{
    
}

/*eol@eof*/
