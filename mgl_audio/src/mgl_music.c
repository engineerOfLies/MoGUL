#include "mgl_music.h"
#include "mgl_logger.h"
#include "mgl_resource.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include <glib/glist.h>
#include <glib.h>
#include <SDL.h>
#include <SDL_mixer.h>

struct MglMusic_S
{
    Mix_Music *music;
    MglFloat volume;
    MglInt loop;
};

static MglResourceManager * __mgl_music_resource_manager = NULL;
static MglFloat __mgl_music_volume = 1;/*global music volume*/

static GList * __mgl_music_playlist = NULL;
static GList * __mgl_music_playlist_current = NULL;
static MglBool __mgl_music_playlist_play = MglFalse;

void mgl_music_close();
MglBool mgl_music_load_resource(char *filename,void *data);
void mgl_music_delete(void *data);
void mgl_music_playlist_play_next();

void mgl_music_init(
    MglUint maxMusic)
{
    __mgl_music_resource_manager = mgl_resource_manager_init(
        "mgl music",
        maxMusic,
        sizeof(struct MglMusic_S),
        MglFalse,
        mgl_music_delete,
        mgl_music_load_resource
    );
    Mix_HookMusicFinished(mgl_music_playlist_play_next);
}

void mgl_music_close()
{
    mgl_resource_manager_free(&__mgl_music_resource_manager);
}

MglBool mgl_music_load_resource(char *filename,void *data)
{
    MglMusic *music;
    if (!filename)return MglFalse;
    if (!data)return MglFalse;
    music = (MglMusic *)data;
    music->music = Mix_LoadMUS(filename);
    return MglTrue;
}

void mgl_music_delete(void *data)
{
    MglMusic *music;
    if (!data)return;
    music = (MglMusic *)data;
    Mix_FreeMusic(music->music);
}

MglMusic *mgl_music_load(char *filename)
{
    MglMusic *music;
    music = mgl_resource_manager_load_resource(__mgl_music_resource_manager,filename);
    if (!music)return NULL;
    music->volume = 1;
    return music;
}

void mgl_music_free(MglMusic **music)
{
    if (!music)return;
    mgl_resource_free_element(__mgl_music_resource_manager,(void **)music);
}

void mgl_music_play(
    MglMusic *music,
    MglFloat *volume,
    MglInt loop)
{
    if (!music)return;
    if (volume)
    {
        Mix_VolumeMusic((int)(*volume * MIX_MAX_VOLUME * __mgl_music_volume));    
    }
    else
    {
        Mix_VolumeMusic((int)(music->volume * MIX_MAX_VOLUME * __mgl_music_volume));    
    }
    Mix_PlayMusic(music->music, loop);
}

void mgl_music_stop()
{
    __mgl_music_playlist_play = MglFalse;
    Mix_HaltMusic();
}

void mgl_music_stop_playlist()
{
    mgl_music_stop();
}

void mgl_music_playlist_enqueue(
    MglMusic *music,
    MglFloat *volume,
    MglInt loop)
{
    if (!music)return;
    if (volume)
    {
        music->volume = *volume;
    }
    music->loop = loop;
    __mgl_music_playlist = g_list_append(__mgl_music_playlist,music);
}

void mgl_music_playlist_play_next()
{
    MglMusic *music;
    if (__mgl_music_playlist == NULL)
    {
        return;
    }
    if (!__mgl_music_playlist_play)return;
    if (__mgl_music_playlist_current == NULL)
    {
        __mgl_music_playlist_current = __mgl_music_playlist;
    }
    else
    {
        /*loop*/
        __mgl_music_playlist_current = __mgl_music_playlist_current->next;
        if (__mgl_music_playlist_current == NULL)
        {
            __mgl_music_playlist_current = __mgl_music_playlist;
        }
    }
    music = (MglMusic *)__mgl_music_playlist_current->data;
    mgl_music_play(
        music,
        &music->volume,
        music->loop);
}

void mgl_music_start_playlist()
{
    if (__mgl_music_playlist_current == NULL)
    {
        __mgl_music_playlist_current = __mgl_music_playlist;
    }
    __mgl_music_playlist_play = MglTrue;
}

void mgl_music_purge_queue()
{
    GList *it = NULL;
    MglMusic *music;
    mgl_music_stop_playlist();
    it = __mgl_music_playlist;
    for (;it != NULL;it = it->next)
    {
        if (!it->data)continue;
        music = it->data;
        mgl_music_free(&music);
        it->data = NULL;
    }
    g_list_free(__mgl_music_playlist);
    __mgl_music_playlist = NULL;
    __mgl_music_playlist_current = NULL;
}

/*eol@eof*/
