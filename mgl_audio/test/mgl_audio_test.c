#include "mgl_audio.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_graphics.h"
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
    MglSound *sound;
    MglMusic *music;
    if ((argc == 2) && (strcmp(argv[1],"-h")==0))
    {
        fprintf(stdout,"usage:\n");
        fprintf(stdout,"%s [FILES]\n",argv[0]);
        return 0;
    }
    init_all();
    
    mgl_audio_init_from_config("../test_data/audio/audio.def");
    
    fprintf(stdout,"mgl_audio_test begin\n");
    sound = mgl_sound_sound_load(
        "../test_data/audio/Fuse-Burning.wav",
        NULL,
        NULL);
    music = mgl_music_load("../test_data/audio/Holst-_mars.ogg");
    
    mgl_sound_play(
        sound,
        NULL,
        NULL,
        NULL,
        0);
    
    mgl_music_play(
        music,
        NULL,
        -1);
    
    while (!done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            done = 1;
        }
    }
    fprintf(stdout,"mgl_audio_test end\n");
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

}
