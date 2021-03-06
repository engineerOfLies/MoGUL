#include "mgl_camera.h"
#include "mgl_level.h"
#include "mgl_audio.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_graphics.h"
#include "mgl_actor.h"
#include "mgl_font.h"
#include "mgl_tilemap.h"
#include <string.h>
#include <SDL.h>

/**
 * @purpose mgl_audio_test is meant to test the audio file resource manager system
 */

void init_all();

int main(int argc,char *argv[])
{
    int done = 0;
    const Uint8 *keys = NULL;
    MglCamera *cam;
    MglUint sw = 0,sh = 0;
    MglParallax *par;
    MglUint bgw,bgh;
    MglVec2D pos = {0,0};
    MglLevel *level;
    
    if ((argc == 2) && (strcmp(argv[1],"-h")==0))
    {
        fprintf(stdout,"usage:\n");
        fprintf(stdout,"%s [FILES]\n",argv[0]);
        return 0;
    }
    init_all();
    

    mgl_graphics_get_screen_resolution(&sw,&sh);

    cam = mgl_camera_new(mgl_vec2d(sw,sh));
    par = mgl_parallax_load("../test_data/images/testlevel/testbg.def",cam);
    mgl_parallax_get_size(par,&bgw,&bgh);
        mgl_logger_info("camera layer size: (%i,%i)",bgw,bgh);
    
    
    mgl_camera_set_bounds(cam,mgl_rect(0,0,bgw,bgh));
    
    mgl_level_init(5,cam);
    
    level = mgl_level_load("../test_data/maps/testmap.def");
    
    fprintf(stdout,"mgl_level_test begin\n");
    while (!done)
    {
        pos = mgl_camera_get_position(cam);
        mgl_graphics_clear_screen();

        mgl_level_draw(level);
        
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RIGHT])
        {
            pos.x += 5;
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            pos.x -= 5;
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            pos.y += 5;
        }
        if (keys[SDL_SCANCODE_UP])
        {
            pos.y -= 5;
        }
        mgl_camera_change_position(cam, pos);
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            done = 1;
        }
        mgl_grahics_next_frame();
    }
    fprintf(stdout,"mgl_level_test end\n");
}

void init_all()
{
    mgl_logger_init();
    mgl_logger_set_stdout_echo(MglTrue);
    mgl_logger_set_threshold(MGL_LOG_ALL);
    mgl_config_init();
    if (mgl_graphics_init_by_config("../test_data/graphics.def") != 0)
    {
        mgl_logger_info("failed to load graphics, exiting...");
        return;
    }
    mgl_sprite_init_from_config("../test_data/graphics.def");
    mgl_actor_init(
        1000,
        0,
        33
    );
    mgl_font_init(
        10,
        "../test_data/fonts/Exo-Regular.otf",
        16
    );

    mgl_audio_init(
        1000,
        100,
        90,
        25,
        MglTrue,
        MglTrue);
    mgl_camera_init(1);
    mgl_parallax_init(5,NULL);
    mgl_tileset_init(2);
    mgl_tilemap_init(10,MglFalse);
}
