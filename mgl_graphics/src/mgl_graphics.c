#include "mgl_types.h"
#include "mgl_config.h"
#include "mgl_logger.h"
#include "mgl_graphics.h"
#include <SDL.h>

/*static global variables*/
static SDL_Window *__mgl_graphics_main_window = NULL;

void mgl_graphics_init(
    char *windowName,
    MglInt width,
    MglInt height,
    MglBool fullscreen
)
{
    MglUint flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        mgl_logger_error("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (width == -1)
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
                             width, height,
                             flags);

    mgl_logger_info("graphics initialized");
}

int mgl_graphics_init_by_config(char *configFile)
{
    MglDict *data = NULL;
    MglConfig *config = NULL;
    MglInt width = 0,height = 0;
    MglBool fullscreen = MglFalse;
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
    
    mgl_dict_get_hash_value_as_int(&width, data, "width");
    mgl_dict_get_hash_value_as_int(&height, data, "height");
    mgl_dict_get_hash_value_as_bool(&fullscreen, data, "fullscreen");
    mgl_dict_get_hash_value_as_line(windowName, data, "windowName");
    
    mgl_config_free(&config);
    
    mgl_graphics_init(windowName,width,height,fullscreen);
    return 0;
}

/*eol@eof*/
