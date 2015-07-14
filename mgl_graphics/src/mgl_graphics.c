#include "mgl_types.h"
#include "mgl_vector.h"
#include "mgl_config.h"
#include "mgl_logger.h"
#include "mgl_graphics.h"
#include <SDL.h>

/*static global variables*/
static SDL_Window   *   __mgl_graphics_main_window = NULL;
static SDL_Renderer *   __mgl_graphics_renderer = NULL;
static SDL_Texture  *   __mgl_graphics_texture = NULL;
static SDL_Surface  *   __mgl_graphics_surface = NULL;
static SDL_Surface  *   __mgl_graphics_temp_buffer = NULL;

/*timing*/
static MglUint __mgl_graphics_frame_delay = 30;
static MglUint __mgl_graphics_now = 0;
static MglUint __mgl_graphics_then = 0;
static MglBool __mgl_graphics_print_fps = MglFalse;
static MglFloat __mgl_graphics_fps = 0; 

/*background*/
static MglUI32 __mgl_graphics_background_color = 0;
static MglVec4D __mgl_graphics_background_color_v = {0,0,0,255};

/*color mask*/
static MglInt __mgl_bitdepth;
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
    MglVec4D bgcolor,
    MglBool fullscreen
)
{
    int a;
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
    
    __mgl_graphics_renderer = SDL_CreateRenderer(__mgl_graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
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
                                    &__mgl_bitdepth,
                                    &__mgl_rmask,
                                    &__mgl_gmask,
                                    &__mgl_bmask,
                                    &__mgl_amask);

    
    __mgl_graphics_surface = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __mgl_bitdepth,
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
    mgl_vec4d_set(__mgl_graphics_background_color_v,bgcolor.x,bgcolor.y,bgcolor.z,bgcolor.w);
    
    srand(a);
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
    if (__mgl_graphics_surface)
    {
        SDL_FreeSurface(__mgl_graphics_surface);
    }
    if (__mgl_graphics_temp_buffer)
    {
        SDL_FreeSurface(__mgl_graphics_temp_buffer);
    }
    __mgl_graphics_surface = NULL;
    __mgl_graphics_main_window = NULL;
    __mgl_graphics_renderer = NULL;
    __mgl_graphics_texture = NULL;
    __mgl_graphics_temp_buffer = NULL;
}

int mgl_graphics_init_by_config(char *configFile)
{
    MglDict *data = NULL;
    MglConfig *config = NULL;
    MglInt renderWidth = 0,renderHeight = 0;
    MglInt viewWidth = 1024,viewHeight = 768;
    MglBool fullscreen = MglTrue;
    MglVec4D bgcolor = {0,0,0,255};
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
    mgl_dict_get_hash_value_as_vec4d(&bgcolor,data,"backgroundColor");
    mgl_dict_get_hash_value_as_bool(&__mgl_graphics_print_fps, data, "printFPS");

    mgl_config_free(&config);
    
    mgl_graphics_init(windowName,viewWidth,viewHeight,renderWidth,renderHeight,bgcolor,fullscreen);
    return 0;
}

SDL_Renderer *mgl_graphics_get_renderer()
{
    return __mgl_graphics_renderer;
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

MglFloat mgl_graphics_get_frames_per_second()
{
    return __mgl_graphics_fps;
}

void mgl_graphics_frame_delay()
{
    MglUint diff;
    __mgl_graphics_then = __mgl_graphics_now;
    __mgl_graphics_now = SDL_GetTicks();
    diff = (__mgl_graphics_now - __mgl_graphics_then);
    if (diff < __mgl_graphics_frame_delay)
    {
        SDL_Delay(__mgl_graphics_frame_delay - diff);
    }
    __mgl_graphics_fps = 1000.0/MAX(SDL_GetTicks() - __mgl_graphics_then,0.001);
}

void mgl_grahics_next_frame()
{
/*    SDL_UpdateTexture(__mgl_graphics_texture, NULL, __mgl_graphics_surface->pixels, __mgl_graphics_surface->pitch);
    SDL_RenderCopy(__mgl_graphics_renderer, __mgl_graphics_texture, NULL, NULL);*/
    SDL_RenderPresent(__mgl_graphics_renderer);
    mgl_graphics_frame_delay();
}

void mgl_graphics_render_lines(MglVec2D *p1,MglVec2D *p2, MglUint lines,MglVec4D color)
{
    int i;
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    for (i = 0; i < lines;i++)
    {
        SDL_RenderDrawLine(__mgl_graphics_renderer,
                           p1[i].x,
                           p1[i].y,
                           p2[i].x,
                           p2[i].y);
    }
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           255,
                           255,
                           255,
                           255);
}

void mgl_graphics_render_line(MglVec2D p1,MglVec2D p2, MglVec4D color)
{
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderDrawLine(__mgl_graphics_renderer,
                       p1.x,
                       p1.y,
                       p2.x,
                       p2.y);
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           255,
                           255,
                           255,
                           255);
}

void mgl_graphics_render_rect(MglRect rect,MglVec4D color)
{
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderDrawRect(__mgl_graphics_renderer,(const struct SDL_Rect *)&rect);
}

void mgl_graphics_render_rects(MglRect *rects,MglUint count,MglVec4D color)
{
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderDrawRects(__mgl_graphics_renderer,rects,count);
}

void mgl_graphics_render_pixel(MglVec2D pixel,MglVec4D color)
{
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderDrawPoint(__mgl_graphics_renderer,
                        pixel.x,
                        pixel.y);
}

void mgl_graphics_render_pixel_list(SDL_Point * pixels,MglUint count,MglVec4D color)
{
    SDL_SetRenderDrawColor(__mgl_graphics_renderer,
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_SetRenderDrawBlendMode(__mgl_graphics_renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderDrawPoints(__mgl_graphics_renderer,
                        pixels,
                        count);
}

MglUint mgl_graphics_get_render_time()
{
    return __mgl_graphics_now;
}

void mgl_graphics_render_surface_to_screen(SDL_Surface *surface,MglRect srcRect,MglVec2D position,MglVec2D scale,MglVec3D rotation)
{
    MglRect dstRect;
    SDL_Point point;
    int w,h;
    if (!__mgl_graphics_texture)
    {
        mgl_logger_warn("mgl_graphics_render_surface_to_screen: no texture available");
        return;
    }
    if (!surface)
    {
        mgl_logger_warn("mgl_graphics_render_surface_to_screen: no surface provided");
        return;
    }
    SDL_QueryTexture(__mgl_graphics_texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed*/
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(__mgl_graphics_texture);
        __mgl_graphics_texture = SDL_CreateTexture(__mgl_graphics_renderer,
                          __mgl_graphics_surface->format->format,
                          SDL_TEXTUREACCESS_STREAMING, 
                          MAX(w,surface->w),
                          MAX(h,surface->h));
        if (!__mgl_graphics_texture)
        {
            mgl_logger_warn("mgl_graphics_render_surface_to_screen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(__mgl_graphics_texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(__mgl_graphics_texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    mgl_vec2d_set(point,rotation.x,rotation.y);
    mgl_rect_set(&dstRect,position.x,position.y,scale.x*srcRect.w,scale.y*srcRect.h);
    SDL_RenderCopyEx(__mgl_graphics_renderer,
                     __mgl_graphics_texture,
                     &srcRect,
                     &dstRect,
                     rotation.z,
                     &point,
                     SDL_FLIP_NONE);
}

void mgl_graphics_clear_screen()
{
    if (!__mgl_graphics_surface)
    {
        return;
    }
    SDL_SetRenderDrawColor(
        __mgl_graphics_renderer,
        __mgl_graphics_background_color_v.x,
        __mgl_graphics_background_color_v.y,
        __mgl_graphics_background_color_v.z,
        __mgl_graphics_background_color_v.w);
    SDL_FillRect(__mgl_graphics_surface,NULL,__mgl_graphics_background_color);
    SDL_RenderClear(__mgl_graphics_renderer);
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

SDL_Surface *mgl_graphics_create_surface(MglUint w,MglUint h)
{
    SDL_Surface *surface;
    surface = SDL_CreateRGBSurface(
        0,w, h,
        __mgl_bitdepth,
        __mgl_rmask,
        __mgl_gmask,
        __mgl_bmask,
        __mgl_amask);
    return surface;
}

void mgl_graphics_blit_surface_to_screen(SDL_Surface *surface,const MglRect * srcRect,const MglRect * dstRect)
{
    if (!surface)return;
    if (!__mgl_graphics_surface)
    {
        mgl_logger_warn("mgl_graphis_blit_surface_to_screen: no screen surface loaded");
        return;
    }
    SDL_BlitSurface(surface,
                    srcRect,
                    __mgl_graphics_surface,
                    (SDL_Rect *)dstRect);
}

SDL_Surface *mgl_graphics_get_temp_buffer(int w,int h)
{
    /*if no temp buffer exists, create one*/
    if (!__mgl_graphics_temp_buffer)
    {
        __mgl_graphics_temp_buffer = mgl_graphics_create_surface(w,h);
        if (!__mgl_graphics_temp_buffer)
        {
            /*error already triggered*/
            return NULL;
        }
        SDL_FillRect(__mgl_graphics_temp_buffer,NULL,mgl_graphics_vec_to_surface_color(__mgl_graphics_surface,mgl_vec4d(0,0,0,0)));
        return __mgl_graphics_temp_buffer;
    }
    /*change the size of the temp buffer*/
    if ((__mgl_graphics_temp_buffer->w < w)||
        (__mgl_graphics_temp_buffer->h < h))
    {
        w = MAX(__mgl_graphics_temp_buffer->w,w);
        h = MAX(__mgl_graphics_temp_buffer->h,h);
        mgl_logger_info("mgl_graphics_get_temp_buffer:resizing buffer to %ix%i",w,h);
        SDL_FreeSurface(__mgl_graphics_temp_buffer);
        __mgl_graphics_temp_buffer = mgl_graphics_create_surface(w,h);
        if (!__mgl_graphics_temp_buffer)
        {
            /*error already triggered*/
            return NULL;
        }
        SDL_FillRect(__mgl_graphics_temp_buffer,NULL,mgl_graphics_vec_to_surface_color(__mgl_graphics_surface,mgl_vec4d(0,0,0,0)));
        return __mgl_graphics_temp_buffer;
    }
    /*otherwise return the existing one*/
    SDL_FillRect(__mgl_graphics_temp_buffer,NULL,mgl_graphics_vec_to_surface_color(__mgl_graphics_surface,mgl_vec4d(0,0,0,0)));
    return __mgl_graphics_temp_buffer;
}

SDL_Surface *mgl_graphics_screen_convert(SDL_Surface **surface)
{
    SDL_Surface *convert;
    if (!(*surface))
    {
        mgl_logger_warn("mgl_graphics_screen_convert: surface provided was NULL");
        return NULL;
    }
    if (!__mgl_graphics_surface)
    {
        mgl_logger_warn("mgl_graphics_screen_convert: graphics not yet initialized");
        return NULL;
    }
    convert = SDL_ConvertSurface(*surface,
                       __mgl_graphics_surface->format,
                       0);
    if (!convert)
    {
        mgl_logger_warn("mgl_graphics_screen_convert: failed to convert surface: %s",SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(*surface);
    *surface = NULL;
    return convert;
}

MglUint mgl_graphics_get_surface_pixel(SDL_Surface *surface,MglVec2D position)
{
    MglUint bpp;/*bytes per pixel*/
    char *pixels;
    if (!surface)
    {
        mgl_logger_warn("mgl_graphics_get_surface_pixel: surface is not provided");
        return 0;
    }
    if ((SDL_MUSTLOCK(surface))&&
        (!surface->locked))
    {
        mgl_logger_warn("mgl_graphics_get_surface_pixel: surface must be locked before use");
        return 0;
    }
    
    pixels = (char *)surface->pixels;
    bpp = surface->pitch / surface->w;
    return *(pixels + ((MglUint)position.y * surface->pitch)+((MglUint)position.x * bpp));
}

void mgl_graphics_set_surface_pixel(SDL_Surface *surface,MglVec2D position,MglUint color)
{
    MglUint bpp;/*bytes per pixel*/
    char *pixels;
    if (!surface)
    {
        mgl_logger_warn("mgl_graphics_set_surface_pixel: surface is not provided");
        return;
    }
    if ((SDL_MUSTLOCK(surface))&&
        (!surface->locked))
    {
        mgl_logger_warn("mgl_graphics_set_surface_pixel: surface must be locked before use");
        return;
    }
    
    pixels = (char *)surface->pixels;
    bpp = surface->pitch / surface->w;
    pixels[((MglUint)position.y * surface->pitch)+((MglUint)position.x * bpp)] = color;
}

/*eol@eof*/
