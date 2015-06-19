#include "mgl_sound.h"
#include "mgl_logger.h"
#include "mgl_resource.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include <SDL.h>
#include <SDL_mixer.h>

static MglResourceManager * __mgl_sound_resource_manager = NULL;
static MglUint __mgl_sound_allocated_channels = 0;
static MglUint __mgl_sound_reserved_channels = 0;
static MglUint __mgl_sound_group_id = 0;
static MglUint __mgl_sound_group_channel = 0;
static MglDict * __mgl_sound_groups = NULL;

struct MglSound_S
{
    Mix_Chunk *chunk;
    MglFloat   volume; /**<sound level to play the sample at, if not specified*/
    MglUint    defaultChannel;/**<the channel to play the sound on, if not specified*/
};

void mgl_sound_close();
MglBool mgl_sound_load_resource(char *filename,void *data);
void mgl_sound_delete(void *data);

void mgl_sound_init(
    MglUint maxSounds,
    MglUint channels,
    MglUint groupChannels)
{
    __mgl_sound_resource_manager = mgl_resource_manager_init(
        "mgl audio",
        maxSounds,
        sizeof(struct MglSound_S),
        MglFalse,
        mgl_sound_delete,
        mgl_sound_load_resource
    );
    __mgl_sound_groups = mgl_dict_new_hash();
    __mgl_sound_allocated_channels = Mix_AllocateChannels(channels);
    __mgl_sound_reserved_channels = Mix_ReserveChannels(groupChannels);
    atexit(mgl_sound_close);    
}

void mgl_sound_init_from_config(char * filename)
{
    
}

void mgl_sound_close()
{
    mgl_dict_free(&__mgl_sound_groups);
    mgl_resource_manager_free(&__mgl_sound_resource_manager);
}

MglBool mgl_sound_load_resource(char *filename,void *data)
{
    MglSound *sound;
    if (!data)
    {
        return MglFalse;
    }
    sound = (MglSound *)data;
    
    sound->chunk = Mix_LoadWAV(filename);
    if (!sound->chunk)
    {
        mgl_logger_error("failed to load sound chunk: %s re:%s",filename,SDL_GetError());
    }
    sound->volume = 1;
    sound->defaultChannel = -1;
    return MglTrue;
}

void mgl_sound_delete(void *data)
{
    MglSound *sound;
    if (!data)
    {
        return;
    }
    sound = (MglSound *)data;
    Mix_FreeChunk(sound->chunk);
    sound->chunk = NULL;
}

void mgl_sound_set_volume(MglSound *sound,MglFloat volume)
{
    if (!sound)return;
    sound->volume = volume;
    Mix_VolumeChunk(sound->chunk, (int)(volume * MIX_MAX_VOLUME));
}

MglSound *mgl_sound_sound_load(
    char * filename,
    MglUint *volume,
    MglUint *defaultChannel
)
{
    MglSound *sound;
    sound = mgl_resource_manager_load_resource(__mgl_sound_resource_manager,filename);
    if (!sound)
    {
        return NULL;
    }
    if (volume)
    {
        mgl_sound_set_volume(sound,*volume);
    }
    if (defaultChannel)
    {
        sound->defaultChannel = *defaultChannel;
    }
    return sound;
}

void mgl_sound_get_group_data(
    char * group,
    MglInt *id,
    MglFloat *volume)
{
    MglDict *groupData;
    if (!__mgl_sound_groups)
    {
        mgl_logger_warn("no audio group setup");
        return;
    }
    groupData = mgl_dict_get_hash_value(__mgl_sound_groups, group);
    if (!groupData)
    {
        mgl_logger_warn("failed to find audio group %s",group);
        return;
    }
    if (id)
    {
        *id = -1;
        if (!mgl_dict_get_hash_value_as_int(id, groupData, "id"))
        {
            mgl_logger_warn("failed to find audio group id for %s",group);
        }
    }
    if (volume)
    {
        *volume = 1;
        if (!mgl_dict_get_hash_value_as_float(volume, groupData,"volume"))
        {
            mgl_logger_warn("failed to find audio group volume for %s",group);
        }
    }
}

void mgl_sound_set_group(
    char * group,
    MglUint channels,
    MglFloat volume
)
{
    MglDict *groupData;
    if (!group)
    {
        mgl_logger_warn("cannot allocate a channel group with no name!");
        return;
    }
    if (!channels)
    {
        mgl_logger_warn("cannot allocate a channel with no channels for group %s",group);
        return;
    }
    if ((__mgl_sound_group_channel + channels) >= __mgl_sound_reserved_channels)
    {
        mgl_logger_warn("cannot allocate %i more channels for group %s",channels,group);
        return;
    }
    Mix_GroupChannels(__mgl_sound_group_channel, __mgl_sound_group_channel + channels-1, ++__mgl_sound_group_id);
    __mgl_sound_group_channel += channels;
    groupData = mgl_dict_new_hash();
    mgl_dict_hash_insert(groupData,"id",mgl_dict_new_int(__mgl_sound_group_id));
    mgl_dict_hash_insert(groupData,"volume",mgl_dict_new_float(volume));
    mgl_dict_hash_insert(__mgl_sound_groups,group,groupData);
}

void mgl_sound_play(
    MglSound * sound,
    MglFloat * volume,
    MglInt   * channel,
    char     * group,
    MglInt     loops)
{
    int chan;
    MglInt id;
    MglFloat netVolume = 1,gVolume = 1;
    if (!sound)return;
    if (volume)
    {
        netVolume = *volume;
    }
    if (group)
    {
        mgl_sound_get_group_data(
            group,
            &id,
            &gVolume);   
        chan = Mix_GroupAvailable(id);
        netVolume *= gVolume;
    }
    else if (channel)
    {
        chan = *channel;
    }
    else
    {
        chan = sound->defaultChannel;
    }
    Mix_VolumeChunk(sound->chunk, (int)(netVolume * MIX_MAX_VOLUME));
    Mix_PlayChannel(chan, sound->chunk, loops);
}

/*eol@eof*/
