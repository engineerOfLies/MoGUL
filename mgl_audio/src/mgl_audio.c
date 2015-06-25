#include "mgl_audio.h"
#include "mgl_logger.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include <SDL.h>
#include <SDL_mixer.h>

void mgl_audio_close();

void mgl_audio_init(
    MglUint maxSounds,
    MglUint channels,
    MglUint channelGroups,
    MglUint maxMusic,
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
    mgl_music_init(maxMusic);
    
}

void mgl_audio_init_from_config(char *filename)
{
    MglConfig *config;
    MglDict *data;
    
    MglUint maxSounds = 1000;
    MglUint channels = 100;
    MglUint channelGroups = 90;
    MglUint maxMusic = 25;
    MglBool enableMP3 = MglTrue;
    MglBool enableOgg = MglTrue;
    
    config = mgl_config_load(filename);
    if (!config)return;
    data = mgl_config_get_dictionary(config);
    
    mgl_dict_get_hash_value_as_uint(&maxSounds, data, "maxSounds");
    mgl_dict_get_hash_value_as_uint(&channels, data, "channels");
    mgl_dict_get_hash_value_as_uint(&channelGroups, data, "channelGroups");
    mgl_dict_get_hash_value_as_uint(&maxMusic, data, "maxMusic");
    mgl_dict_get_hash_value_as_bool(&enableMP3, data, "enableMP3");
    mgl_dict_get_hash_value_as_bool(&enableOgg, data, "enableOgg");
    
    mgl_audio_init(
        maxSounds,
        channels,
        channelGroups,
        maxMusic,
        enableMP3,
        enableOgg);
    
    if (mgl_dict_get_hash_value(data,"groups")!= NULL)
    {
        mgl_sound_setup_groups_by_config(filename);
    }
    
    mgl_config_free(&config);
}

void mgl_audio_close()
{
}
/*eol@eof*/
