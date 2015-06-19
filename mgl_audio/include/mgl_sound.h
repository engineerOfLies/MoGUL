#ifndef __MGL_SOUND_H__
#define __MGL_SOUND_H__
/**
 * mgl_sound
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
#include "mgl_types.h"


/**
 * @prupose the the MglSound keeps track of sound bites.  Useful for audio effects and not music
 */
typedef struct MglSound_S MglSound;

/**
 * @brief initializes the sound system based on the passed in parameters
 * @param maxSounds the maximum number of sounds that can be loaded into memory at once
 * @param channels the number of allocated audio channels (excluding music channel)
 * @param channelGroups the number of channels to be reserved for groups to be set up
 */
void mgl_sound_init(
    MglUint maxSounds,
    MglUint channels,
    MglUint channelGroups);

/**
 * @brief loads a supported audio file from file.
 * @param filename the sound file to load
 * @param volume [optional] if specified the volume will be set to this.
 * @param defaultChannel [optional] if specified this will be the channel to play on unless otherwise stated
 */
MglSound *mgl_sound_sound_load(
    char * filename,
    MglUint *volume,
    MglUint *defaultChannel);

/**
 * @brief play a loaded sound file
 * @param sound the sound to play
 * @param volume [optional] if provided this will change the volume of the sound to play.  Value is expected to be between 0 and 1.
 * @param channel [optional] if provided this will be the channel to play the sound on
 * @param group [optional] if provided, this will be the channel group to play the sound on
 * @param loops the number of times to loop this sound.  -1 implies indefinitely
 */
void mgl_sound_play(
    MglSound * sound,
    MglFloat * volume,
    MglInt   * channel,
    char     * group,
    MglInt     loops);

/**
 * @brief sets the playing volume for the specified sound file
 * @param sound the sound file to set
 * @param volume the sound level to set it to.  Expects a value between 0 and 1
 */
void mgl_sound_set_volume(MglSound *sound,MglFloat volume);

/**
 * @brief free the sound file
 * @param a pointer to your sound pointer
 */
void mgl_sound_free(MglSound **sound);

/*
 
 Sound Groups
 
 */
/**
 * @brief lookup the group id for the specified group name
 * @param group the name to lookup
 * @param id [optional] output.  If specified it will be populated with the group id or -1 if not found
 * @param volume [optional] output.  If specified it will be populated with the group volume or 1 if not found
 */
void mgl_sound_get_group_data(
    char * group,
    MglInt *id,
    MglFloat *volume);

/**
 * @brief allocate a group of sound channels
 * @param group the name of the group to create
 * @param channels how many channels to allocate
 * @param volume the volume to play the channel at.  Expects a value between 0 and 1
 */
void mgl_sound_set_group(
    char * group,
    MglUint channels,
    MglFloat volume
);

/**
 * @brief setup audio groups based on config file
 * @param filename the config file to use
 */
void mgl_sound_setup_groups_by_config(char * filename);

#endif
