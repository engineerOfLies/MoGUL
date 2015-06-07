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
static MglSpriteMode __mgl_sprite_mode = MglSpriteBoth;

struct MglSprite_S
{
    SDL_Texture *texture;
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
    MglSI64 red,green,blue,colorKey;
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
    colorKey = atoi(strings[7]);
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
    SDL_SetColorKey(image,
                    SDL_TRUE,
                    colorKey);
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
    
    if (__mgl_sprite_mode & MglSpriteTexture)
    {
        sprite->texture = SDL_CreateTextureFromSurface(mgl_graphics_get_renderer(),sprite->image);
        if (sprite->texture)
        {
            SDL_SetTextureBlendMode(sprite->texture,SDL_BLENDMODE_BLEND);        
            SDL_UpdateTexture(sprite->texture,
                            NULL,
                            sprite->image->pixels,
                            sprite->image->pitch);
            if (!(__mgl_sprite_mode & MglSpriteSurface))
            {
                SDL_FreeSurface(sprite->image);
            }
        }
    }
    return MglTrue;
}

void mgl_sprite_delete(void *data)
{
    MglSprite *sprite;
    sprite = (MglSprite *)data;
    if (sprite->image)
    {
        SDL_FreeSurface(sprite->image);
    }
    sprite->image = NULL;
    if (sprite->texture)
    {
        SDL_DestroyTexture(sprite->texture);
    }
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

MglSprite *mgl_sprite_load_from_dict(MglDict *data)
{
    MglLine spriteFilename;
    MglInt frameWidth;
    MglInt frameHeight;
    MglUint framesPerLine = __mgl_sprite_default_fpl;
    MglVec4D redSwap = {-1,-1,-1,-1};
    MglVec4D greenSwap = {-1,-1,-1,-1};
    MglVec4D blueSwap = {-1,-1,-1,-1};
    MglVec4D colorKey = {-1,-1,-1,-1};
    if (!data)
    {
        return NULL;
    }
    mgl_dict_get_hash_value_as_line(spriteFilename,data,"filename");
    mgl_dict_get_hash_value_as_int(&frameWidth,data,"frameWidth");
    mgl_dict_get_hash_value_as_int(&frameHeight,data,"frameHeight");
    mgl_dict_get_hash_value_as_uint(&framesPerLine,data,"framesPerLine");
    
    mgl_dict_get_hash_value_as_vec4d(&redSwap,data,"redSwap");
    mgl_dict_get_hash_value_as_vec4d(&greenSwap,data,"greenSwap");
    mgl_dict_get_hash_value_as_vec4d(&blueSwap,data,"blueSwap");
    mgl_dict_get_hash_value_as_vec4d(&colorKey,data,"colorKey");
    return mgl_sprite_load_from_image(
        spriteFilename,
        frameWidth,
        frameHeight,
        framesPerLine,
        redSwap.x!=-1?&redSwap:NULL,
        greenSwap.x!=-1?&greenSwap:NULL,
        blueSwap.x!=-1?&blueSwap:NULL,
        colorKey.x!=-1?&colorKey:NULL);
}

MglSprite *mgl_sprite_load_from_def(char *filename)
{
    MglConfig *config;
    MglSprite *sprite;
    MglDict *data;
    if (!filename)return NULL;
    config = mgl_config_load(filename);
    if (!config)return NULL;
    data = mgl_config_get_dictionary(config);
    sprite = mgl_sprite_load_from_dict(data);
    mgl_config_free(&config);
    return sprite;
}

MglSprite *mgl_sprite_load_from_image(
    char *filename,
    MglInt frameWidth,
    MglInt frameHeight,
    MglUint framesPerLine,
    MglVec4D *redSwap,
    MglVec4D *greenSwap,
    MglVec4D *blueSwap,
    MglVec4D *colorKey)
{
    char *filenamePacked;
    MglSprite *sprite = NULL;
    filenamePacked = g_strdup_printf (
        "%s|%i|%i|%i|%i|%i|%i|%i",
        filename,
        frameWidth,
        frameHeight,
        framesPerLine,
        redSwap?mgl_graphics_vec_to_screen_color(*redSwap):-1,
        greenSwap?mgl_graphics_vec_to_screen_color(*greenSwap):-1,
        blueSwap?mgl_graphics_vec_to_screen_color(*blueSwap):-1,
        colorKey?mgl_graphics_vec_to_screen_color(*colorKey):-1);
    sprite = mgl_resource_manager_load_resource(__mgl_sprite_resource_manager,filenamePacked);
    free(filenamePacked);
    if (!sprite)
    {
        mgl_logger_warn("mgl_sprite_load_from_image: failed to load image:%s",filename);
        return NULL;
    }
        
    return sprite;
}

void mgl_sprite_draw(
    MglSprite * sprite,
    MglVec2D position,
    MglVec2D * scale,
    MglVec2D * scaleCenter,
    MglVec3D * rotation,
    MglVec2D * flip,
    MglUint frame)
{
    MglRect cell,target;
    SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
    SDL_Point r;
    MglVec2D scaleFactor = {1,1};
    MglVec2D scaleOffset = {0,0};
    if (!sprite)
    {
        return;
    }
    
    if (scale)
    {
        mgl_vec2d_copy(scaleFactor,(*scale));
    }
    if (scaleCenter)
    {
        mgl_vec2d_copy(scaleOffset,(*scaleCenter));
    }
    if (rotation)
    {
        mgl_vec2d_copy(r,(*rotation));
        r.x *= scaleFactor.x;
        r.y *= scaleFactor.y;
    }
    if (flip)
    {
        if (flip->x)flipFlags |= SDL_FLIP_HORIZONTAL;
        if (flip->y)flipFlags |= SDL_FLIP_VERTICAL;
    }
    
    mgl_rect_set(
        &cell,
        frame%sprite->framesPerLine * sprite->frameWidth,
        frame/sprite->framesPerLine * sprite->frameHeight,
        sprite->frameWidth,
        sprite->frameHeight);
    mgl_rect_set(
        &target,
        position.x - (scaleFactor.x * scaleOffset.x),
        position.y - (scaleFactor.y * scaleOffset.y),
        sprite->frameWidth * scaleFactor.x,
        sprite->frameHeight * scaleFactor.y);
    SDL_RenderCopyEx(mgl_graphics_get_renderer(),
                     sprite->texture,
                     &cell,
                     &target,
                     rotation?rotation->z:0,
                     rotation?&r:NULL,
                     flipFlags);
}

void mgl_sprite_draw_to_surface(SDL_Surface *surface, MglSprite *sprite, MglVec2D position,MglUint frame)
{
    MglRect cell,target;
    mgl_rect_set(
        &cell,
        frame%sprite->framesPerLine * sprite->frameWidth,
        frame/sprite->framesPerLine * sprite->frameHeight,
        sprite->frameWidth,
        sprite->frameHeight);
    mgl_rect_set(
        &target,
        position.x,
        position.y,
        sprite->frameWidth,
        sprite->frameHeight);
    SDL_BlitSurface(sprite->image,&cell,surface,&target);
}

void mgl_sprite_free(MglSprite **sprite)
{
    mgl_resource_free_element(__mgl_sprite_resource_manager,(void **)sprite);
}


/*eol@eof*/
