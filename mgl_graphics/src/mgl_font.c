#include "mgl_font.h"
#include "mgl_rect.h"
#include "mgl_graphics.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include <SDL_ttf.h>

struct MglFont_S
{
    TTF_Font *font;
    MglUint   pointSize;
};

static MglResourceManager * __mgl_font_resource_manager = NULL;
static MglFont * __mgl_font_default = NULL;
void mgl_font_close();
MglBool mgl_font_load_resource(char *filename,void *data);
void mgl_font_delete(void *data);

void mgl_font_init(
    MglInt maxFonts,
    MglLine defaultFontFile,
    MglUint defaultFontPoint
)
{
    if (TTF_Init()==-1) 
    {
        mgl_logger_error("failed to load TTF_Init: %s\n", TTF_GetError());
        return;
    }
    atexit(TTF_Quit);
    __mgl_font_resource_manager = mgl_resource_manager_init(
        "mgl font",
        maxFonts,
        sizeof(struct MglFont_S),
        MglFalse,
        mgl_font_delete,
        mgl_font_load_resource
    );
    atexit(mgl_font_close);
    __mgl_font_default = mgl_font_load(defaultFontFile,defaultFontPoint);
}

void mgl_font_close()
{
    mgl_resource_manager_free(&__mgl_font_resource_manager);
}

MglBool mgl_font_load_resource(char *filename,void *data)
{
    MglFont *font;
    MglLine fname;
    MglUint pointSize;
    char ** strings;
    if (!data)return MglFalse;
    font = (MglFont *)data;
    strings = g_strsplit_set (filename,
                              "|",
                              0);
    mgl_line_cpy(fname,strings[0]);
    pointSize = atoi(strings[1]);
    g_strfreev (strings);
    font->font = TTF_OpenFont(fname, pointSize);
    if (!font->font)
    {
        mgl_logger_error("failed to load font: %s, re: %s",fname,SDL_GetError());
        return MglFalse;
    }
    font->pointSize = pointSize;
    return MglTrue;
}

void mgl_font_delete(void *data)
{
    MglFont *font;
    font = (MglFont *)data;
    if (!font)return;
    if (font->font)
    {
        TTF_CloseFont(font->font);
    }
    font->font = NULL;
}

void mgl_font_free(MglFont **font)
{
    mgl_resource_free_element(__mgl_font_resource_manager,(void **)font);
}

MglFont *mgl_font_load(char *filename,MglUint pointSize)
{
    char *filenamePacked;
    MglFont *font = NULL;
    filenamePacked = g_strdup_printf (
        "%s|%i",
        filename,
        pointSize);
    font = mgl_resource_manager_load_resource(__mgl_font_resource_manager,filenamePacked);
    free(filenamePacked);
    if (!font)
    {
        mgl_logger_warn("mgl_font_load_from_image: failed to load image:%s",filename);
        return NULL;
    }    
    return font;
}

void mgl_font_draw_text_basic(
    MglVec2D position,
    MglLine text,
    MglVec4D color)
{
    SDL_Color c;
    SDL_Surface *surface;
    MglRect srcRect = {0,0,0,0};
    if (!__mgl_font_default)return;
    if (strlen(text) <= 0)return;
    c.r = color.x;
    c.g = color.y;
    c.b = color.z;
    c.a = color.w;
    
    surface = TTF_RenderUTF8_Blended(__mgl_font_default->font, text, c);
    if (!surface)
    {
        mgl_logger_error("failed to render font:%s",SDL_GetError());
        return;
    }
    srcRect.w = surface->w;
    srcRect.h = surface->h;
    mgl_graphics_render_surface_to_screen(surface,srcRect,position,mgl_vec2d(1,1),mgl_vec3d(0,0,0));
}
/*eol@eof*/
