#include "mgl_font.h"
#include "mgl_rect.h"
#include "mgl_text.h"
#include "mgl_graphics.h"
#include "mgl_resource.h"
#include "mgl_logger.h"
#include <SDL.h>
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
void mgl_font_chomp(char *text,int length,int strl);
char * mgl_font_clean_control_characters(char *in);

static MglBool mgl_font_initialized();

MglFont *mgl_font_default()
{
    return __mgl_font_default;
}

void mgl_font_init(
    MglInt maxFonts,
    MglLine defaultFontFile,
    MglUint defaultFontPoint
)
{
    if (TTF_Init()==-1) 
    {
        mgl_logger_error("failed to load TTF_Init: %s", TTF_GetError());
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

static MglBool mgl_font_initialized()
{
    if (!__mgl_font_resource_manager)return MglFalse;
    return MglTrue;
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
        mgl_logger_warn("failed to load image:%s",filename);
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
    SDL_FreeSurface(surface);
}

MglRect mgl_font_get_text_bounds(
    char    * text,
    MglFont * font
)
{
    MglRect r = {0,0,0,0};
    if (!mgl_font_initialized())return r;
    if ((text == NULL)||(text[0] == '\0'))
    {
        mgl_logger_error(
            "no text provided for draw.\n");
        return r;
    }
    if (font == NULL)
    {
        mgl_logger_error(
            "no font provided for draw.");
        return r;
    }
    TTF_SizeText(font->font,text, (int *)&r.w, (int *)&r.h);
    return r;
}

MglRect mgl_font_get_text_wrap_bounds(
    char    * thetext,
    MglFont * font,
    MglUint   w,
    MglUint   h
)
{
    MglRect r = {0,0,0,0};
    MglText textline;
    MglText temptextline;
    MglText text;
    MglLine word;
    MglBool whitespace;
    int tw = 0, th = 0;
    int drawheight = 0;
    int i = 0;
    int space = 0;
    int lindex = 0;
    if (!mgl_font_initialized())return r;
    if((thetext == NULL)||(thetext[0] == '\0'))
    {
        return r;
    }
    if (font == NULL)
    {
        mgl_logger_error(
            "no font provided for draw.");
        return r;
    }
    
    strncpy(text,thetext,MGLTEXTLEN);
    temptextline[0] = '\0';
    do
    {
        space = 0;
        i = 0;
        whitespace = MglFalse;
        do
        {
            if(sscanf(&text[i],"%c",&word[0]) == EOF)break;
            if(word[0] == ' ')
            {
                space++;
                whitespace = MglTrue;
            }
            if(word[0] == '\t')
            {
                space+=2;
                whitespace = MglTrue;
            }
            i++;
        }while(whitespace);
        
        if(sscanf(text,"%s",word) == EOF)
        {
            break;
        }
        mgl_font_chomp(text,strlen(word) + 1,MGLTEXTLEN);
        strncpy(textline,temptextline,MGLTEXTLEN);/*keep the last line that worked*/
        for(i = 0;i < (space - 1);i++)
        {
            sprintf(temptextline,"%s%c",temptextline,' '); /*add spaces*/
        }
        sprintf(temptextline,"%s %s",temptextline,word); /*add a word*/
        TTF_SizeText(font->font, temptextline, &tw, &th); /*see how big it is now*/
        lindex += strlen(word);
        if(tw > w)         /*see if we have gone over*/
        {
            drawheight += th;
            if (h != 0)
            {
                if ((drawheight + th) > h)
                {
                    break;
                }
            }
            sprintf(temptextline,"%s",word); /*add a word*/
        }
        else if (tw > r.w)
        {
            r.w = tw;
        }
    }while(1);
    r.h = drawheight + th;
    return r;
}

MglUint mgl_font_get_text_height_average(MglFont *font)
{
    MglRect r;
    r = mgl_font_get_text_bounds(
        "gL",
        font
    );
    return r.h;
}

MglUint mgl_font_justify_from_string(char * justify)
{
    if ((mgl_word_cmp(justify,"LEFT") == 0) ||
        (mgl_word_cmp(justify,"left") == 0) ||
        (mgl_word_cmp(justify,"Left") == 0))
        return MglJustifyLeft;
    if ((mgl_word_cmp(justify,"CENTER") == 0) ||
        (mgl_word_cmp(justify,"center") == 0) ||
        (mgl_word_cmp(justify,"Center") == 0))
        return MglJustifyCenter;
    if ((mgl_word_cmp(justify,"RIGHT") == 0) ||
        (mgl_word_cmp(justify,"right") == 0) ||
        (mgl_word_cmp(justify,"Right") == 0))
        return MglJustifyRight;
    return MglJustifyNone;
}

void mgl_font_draw_text(
    char    *text,
    MglVec2D position,
    MglVec4D color,
    MglFont *font
)
{
    char *renderText = NULL;
    SDL_Surface *temp = NULL;
    SDL_Color colortype;
    MglRect srcRect = {0,0,0,0};
    if (!mgl_font_initialized())return;
    if((text == NULL) || 
        (strlen(text) <= 0))
    {
        mgl_logger_error("passed empty string for text draw.");
        return;
    }
    if (font == NULL)
    {
        mgl_logger_error("no font provided for draw.");
        return;
    }
    if (font->font == NULL)
    {
        mgl_logger_error("bad MglFont provided for draw.");
        return;
    }
    colortype.r = color.x;
    colortype.g = color.y;
    colortype.b = color.z;
    colortype.a = color.w;
    renderText = mgl_font_clean_control_characters(text);
    if (!renderText)
    {
        temp = TTF_RenderText_Blended(font->font, text,colortype);
    }
    else
    {
        temp = TTF_RenderText_Blended(font->font, renderText,colortype);
        free(renderText);
    }
    if (temp == NULL)
    {
        mgl_logger_error("unable to render text from SDL_ttf");
        return;
    }
    /*Creates an opengl compatable RGBA surface*/
    srcRect.w = temp->w;
    srcRect.h = temp->h;
    mgl_graphics_render_surface_to_screen(temp,srcRect,position,mgl_vec2d(1,1),mgl_vec3d(0,0,0));    
    SDL_FreeSurface(temp);
}

void mgl_font_draw_text_wrap(
    char   * thetext,
    MglRect  block,
    MglVec4D color,
    MglFont *font
)
{
    MglText textline;
    MglText temptextline;
    MglText text;
    MglLine word;
    MglBool whitespace;
    int drawheight = block.y;
    int w,h;
    int i;
    int space;
    int lindex = 0;
    if (!mgl_font_initialized())return;
    if ((thetext == NULL)||(thetext[0] == '\0'))
    {
        mgl_logger_error(
            "no text provided for draw.");
        return;
    }
    if (font == NULL)
    {
        mgl_logger_error(
            "no font provided for draw.");
        return;
    }
    if (font->font == NULL)
    {
        mgl_logger_error(
            "bad MglFont provided for draw.");
        return;
    }
    strncpy(text,thetext,MGLTEXTLEN);
    temptextline[0] = '\0';
    do
    {
        space = 0;
        i = 0;
        do
        {
            whitespace = MglFalse;
            if(sscanf(&text[i],"%c",&word[0]) == EOF)break;
            if(word[0] == ' ')
            {
                space++;
                whitespace = MglTrue;
            }
            if(word[0] == '\t')
            {
                space+=2;
                whitespace = MglTrue;
            }
            i++;
        }while (whitespace);
        if (sscanf(text,"%s",word) == EOF)
        {
            block.y=drawheight+h;
            mgl_font_draw_text(temptextline,mgl_vec2d(block.x,block.y), color, font);
            return;
        }
        
        mgl_font_chomp(text,strlen(word) + space,MGLTEXTLEN);
        strncpy(textline,temptextline,MGLTEXTLEN);/*keep the last line that worked*/
        for (i = 0;i < (space - 1);i++)
        {
            sprintf(temptextline,"%s%c",temptextline,' '); /*add spaces*/
        }
        sprintf(temptextline,"%s %s",temptextline,word); /*add a word*/
        TTF_SizeText(font->font, temptextline, &w, &h); /*see how big it is now*/
        lindex += strlen(word);
        if(w > block.w)         /*see if we have gone over*/
        {
            block.y=drawheight+h;
            mgl_font_draw_text(textline,mgl_vec2d(block.x,block.y), color, font);
            
            /*draw the line and get ready for the next line*/
            drawheight += h;
            if (block.h != 0)
            {
                if ((drawheight + h) > block.h)
                {
                    break;
                }
            }
            sprintf(temptextline,"%s",word); /*add a word*/
        }
    }while(1);
}

void mgl_font_chomp(char *text,int length,int strl)
{
    int i;
    if (!text)return;
    for(i = 0;i < strl - length;i++)
    {
        text[i] = text[i + length];
    }
    if (i > 0)
    {
        text[i - 1] = '\0';/*null terminate in case its overwritten*/
    }
    else
    {
        text[0] = '\0';
    }
}

char * mgl_font_clean_control_characters(char *in)
{
    char *out;
    int outIndex = 0;
    int inIndex = 0;
    out = malloc(strlen(in)*2);
    if (!out)return NULL;
    for (inIndex = 0;inIndex < strlen(in);inIndex++,outIndex++)
    {
        if (in[inIndex] == '\t')
        {
            out[outIndex++] = ' ';
            out[outIndex] = ' ';
            continue;
        }
        if (in[inIndex] == '\r')
        {
            out[outIndex] = ' ';
            continue;
        }
        out[outIndex] = in[inIndex];
    }
    out[outIndex] = '\0';
    return out;
}

/*eol@eof*/
