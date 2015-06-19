#include "mgl_audio.h"
#include "mgl_logger.h"
#include <SDL.h>
#include <SDL_mixer.h>

void mgl_audio_close();

void mgl_audio_init(
    MglUint maxSounds,
    MglUint channels,
    MglUint channelGroups,
    MglBool enableMP3,
    MglBool enableOgg)
{
    int flags = 0;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
    {
        printf("Failed to open audio: %s\n", SDL_GetError());
    }
    atexit(Mix_CloseAudio);
    
    if (enableMP3)
    {
        flags |= MIX_INIT_MP3;
    }
    if (enableOgg)
    {
        flags |= MIX_INIT_OGG;
    }
    if (!(Mix_Init(flags) & flags))
    {
        mgl_logger_error("failed to initialize some audio support: %s",SDL_GetError());
    }
    atexit(Mix_Quit);
    atexit(mgl_audio_close);
    mgl_sound_init(
        maxSounds,
        channels,
        channelGroups);
}

void mgl_audio_close()
{
}
/*eol@eof*/
