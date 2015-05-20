#include "mgl_types.h"
#include "mgl_config.h"
#include "mgl_logger.h"
#include "mgl_graphics.h"
#include <SDL.h>

/*static global variables*/
static SDL_Window   *   __mgl_graphics_main_window = NULL;
static SDL_Renderer *   __mgl_graphics_renderer = NULL;
static SDL_Texture  *   __mgl_graphics_texture = NULL;
static SDL_Surface  *   __mgl_graphics_surface = NULL;

/*timing*/
static MglUint __mgl_graphics_frame_delay = 30;
static MglUint __mgl_graphics_now = 0;
static MglUint __mgl_graphics_then = 0;

/*background*/
static MglUI32 __mgl_graphics_background_color = 0;

/*color mask*/
static MglUint __mgl_rmask;
static MglUint __mgl_gmask;
static MglUint __mgl_bmask;
static MglUint __mgl_amask;

static void mgl_graphics_close();

void mgl_graphics_init(
    char *windowName,
    MglInt viewWidth,
    MglInt viewHeight,
    MglInt renderWidth,
    MglInt renderHeight,
    MglVec3D bgcolor,
    MglBool fullscreen
)
{
    MglInt bitDepth = 32;
    MglUint flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        mgl_logger_error("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (renderWidth == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }
    __mgl_graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderWidth, renderHeight,
                             flags);

    if (!__mgl_graphics_main_window)
    {
        mgl_logger_error("failed to create main window: %s",SDL_GetError());
        mgl_graphics_close();
        return;
    }
    
    __mgl_graphics_renderer = SDL_CreateRenderer(__mgl_graphics_main_window, -1, 0);
    if (!__mgl_graphics_renderer)
    {
        mgl_logger_error("failed to create renderer: %s",SDL_GetError());
        mgl_graphics_close();
        return;
    }
    SDL_SetRenderDrawColor(__mgl_graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__mgl_graphics_renderer);
    SDL_RenderPresent(__mgl_graphics_renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(__mgl_graphics_renderer, renderWidth, renderHeight);

    __mgl_graphics_texture = SDL_CreateTexture(
        __mgl_graphics_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        renderWidth, renderHeight);
    if (!__mgl_graphics_texture)
    {
        mgl_logger_error("failed to create screen texture: %s",SDL_GetError());
        mgl_graphics_close();
        return;
    }
    
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888,
                                    &bitDepth,
                                    &__mgl_rmask,
                                    &__mgl_gmask,
                                    &__mgl_bmask,
                                    &__mgl_amask);

    
    __mgl_graphics_surface = SDL_CreateRGBSurface(0, renderWidth, renderHeight, bitDepth,
                                        __mgl_rmask,
                                    __mgl_gmask,
                                    __mgl_bmask,
                                    __mgl_amask);
    
    if (!__mgl_graphics_surface)
    {
        mgl_logger_error("failed to create screen surface: %s",SDL_GetError());
        mgl_graphics_close();
        return;
    }

    __mgl_graphics_background_color = SDL_MapRGB(__mgl_graphics_surface->format, bgcolor.x,bgcolor.y,bgcolor.z);
    
    atexit(mgl_graphics_close);
    mgl_logger_info("graphics initialized");
}

void mgl_graphics_close()
{
    if (__mgl_graphics_texture)
    {
        SDL_DestroyTexture(__mgl_graphics_texture);
    }
    if (__mgl_graphics_renderer)
    {
        SDL_DestroyRenderer(__mgl_graphics_renderer);
    }
    if (__mgl_graphics_main_window)
    {
        SDL_DestroyWindow(__mgl_graphics_main_window);
    }
    __mgl_graphics_main_window = NULL;
    __mgl_graphics_renderer = NULL;
    __mgl_graphics_texture = NULL;
}

int mgl_graphics_init_by_config(char *configFile)
{
    MglDict *data = NULL;
    MglConfig *config = NULL;
    MglInt renderWidth = 0,renderHeight = 0;
    MglInt viewWidth = 1024,viewHeight = 768;
    MglBool fullscreen = MglTrue;
    MglVec3D bgcolor = {0,0,0};
    MglLine windowName = "--==MoGUL==--";
    if (!configFile)
    {
        mgl_logger_error("mgl_graphics_init_by_config: failed to provide config file to load");
        return -1;
    }
    config = mgl_config_load(configFile);
    if (!config)
    {
        mgl_logger_error("mgl_graphics_init_by_config: failed to load config file");
        return -1;
    }
    data = mgl_config_get_dictionary(config);
    
    mgl_dict_get_hash_value_as_int(&renderWidth, data, "renderWidth");
    mgl_dict_get_hash_value_as_int(&renderHeight, data, "renderHeight");
    mgl_dict_get_hash_value_as_int(&viewWidth, data, "viewWidth");
    mgl_dict_get_hash_value_as_int(&viewHeight, data, "viewHeight");
    mgl_dict_get_hash_value_as_bool(&fullscreen, data, "fullscreen");
    mgl_dict_get_hash_value_as_line(windowName, data, "windowName");
    mgl_dict_get_hash_value_as_uint(&__mgl_graphics_frame_delay, data, "frameDelay");
    mgl_dict_get_hash_value_as_vec3d(&bgcolor,data,"backgroundColor");
    
    mgl_config_free(&config);
    
    mgl_graphics_init(windowName,viewWidth,viewHeight,renderWidth,renderHeight,bgcolor,fullscreen);
    return 0;
}

SDL_Texture *mgl_graphics_get_screen_texture()
{
    return __mgl_graphics_texture;
}

SDL_Surface *mgl_graphics_get_screen_surface()
{
    return __mgl_graphics_surface;
}

void mgl_graphics_set_frame_delay(MglUint frameDelay)
{
    __mgl_graphics_frame_delay = frameDelay;
}

void mgl_graphics_frame_delay()
{
    MglUint diff;
    __mgl_graphics_now = SDL_GetTicks();
    diff = (__mgl_graphics_now - __mgl_graphics_then);
    if (diff < __mgl_graphics_frame_delay)
    {
        SDL_Delay(__mgl_graphics_frame_delay - diff);
    }
}

void mgl_grahics_next_frame()
{
    SDL_UpdateTexture(__mgl_graphics_texture, NULL, __mgl_graphics_surface->pixels, __mgl_graphics_surface->pitch);
    SDL_RenderClear(__mgl_graphics_renderer);
    SDL_RenderCopy(__mgl_graphics_renderer, __mgl_graphics_texture, NULL, NULL);
    SDL_RenderPresent(__mgl_graphics_renderer);
    mgl_graphics_frame_delay();
}

MglUint mgl_graphics_get_render_time()
{
    return __mgl_graphics_now;
}

void mgl_graphics_clear_screen()
{
    if (!__mgl_graphics_surface)
    {
        return;
    }
    SDL_FillRect(__mgl_graphics_surface,NULL,__mgl_graphics_background_color);
}

void mgl_graphics_set_bgcolor(MglVec3D bgcolor)
{
    if (!__mgl_graphics_surface)
    {
        return;
    }
    __mgl_graphics_background_color = SDL_MapRGB(__mgl_graphics_surface->format, bgcolor.x,bgcolor.y,bgcolor.z);
}

MglUint mgl_graphics_vec_to_screen_color(MglVec4D color)
{
    if (!__mgl_graphics_surface)
    {
        mgl_logger_warn("mgl_graphics_vec_to_screen_color: no screen surface loaded");
        return 0;
    }
    return mgl_graphics_vec_to_surface_color(__mgl_graphics_surface,color);
}

MglUint mgl_graphics_vec_to_surface_color(SDL_Surface *surface,MglVec4D color)
{
    if (!surface)
    {
        mgl_logger_warn("mgl_graphics_vec_to_surface_color: no surface provided");
        return 0;
    }
    return SDL_MapRGBA(surface->format, color.x,color.y,color.z,color.w);
}

MglInt mgl_graphics_get_screen_resolution(MglUint *w,MglUint *h)
{
    if (!__mgl_graphics_surface)
    {
        mgl_logger_warn("mgl_graphics_get_screen_resolution: no screen surface loaded");
        return -1;
    }
    if (w)*w = __mgl_graphics_surface->w;
    if (h)*h = __mgl_graphics_surface->h;
    return 0;
}

/*eol@eof*/
