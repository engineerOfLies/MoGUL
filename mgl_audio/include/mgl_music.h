#ifndef __MGL_MUSIC_H__
#define __MGL_MUSIC_H__
/**
 * mgl_music
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

typedef struct MglMusic_S MglMusic;

/**
 * @brief initialize the music system / setup the music Queue
 */
void mgl_music_init(
    MglUint maxMusic
);

/**
 * @brief load the music file
 * @param filename the music file to load
 * @return NULL on error or a pointer to the loaded music file
 */
MglMusic *mgl_music_load(char *filename);

/**
 * @brief free music previously loaded
 * @param a pointer to your music pointer
 */
void mgl_music_free(MglMusic **music);

/**
 * @brief play music file at the specified volume
 * @param music the music file to play
 * @param volume the volume to play the file at.  Value should be between 0 and 1
 * @param loop the number of times to loop, -1 implies continuously
 */
void mgl_music_play(
    MglMusic *music,
    MglFloat *volume,
    MglInt loop);

/**
 * @brief stop playing any music currently played
 */
void mgl_music_stop();

/**
 * @brief enqueue a song to play in the loaded playlist
 */
void mgl_music_playlist_enqueue(
    MglMusic *music,
    MglFloat *volume,
    MglInt loop);

/**
 * @brief start playing the songs in the playlist
 */
void mgl_music_start_playlist();

/**
 * @brief stops playing the songs in the playlist
 */
void mgl_music_stop_playlist();

/**
 * @brief empty all songs in the music queue
 */
void mgl_music_purge_queue();

#endif
