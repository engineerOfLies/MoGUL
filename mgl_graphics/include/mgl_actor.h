#ifndef __MGL_ACTOR_H__
#define __MGL_ACTOR_H__
/**
 * mgl_actor
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */

/**
 * @prupose
 * Provide an abtraction for animated actors.  Support for sprites and drawn figures.
 */

#include "mgl_types.h"
#include "mgl_text.h"
#include "mgl_sprite.h"

enum MglActionTypes {
    MglActionLoop  = 0,/**<Action is meant to loop back to the begining, ie: walk*/
    MglActionPass  = 1,/**<Action is meant to make a pass and then end, ie: attack*/
    MglActionOsci  = 2 /**<Action is meant to oscillate between begin and end, ie: idle*/
};


/**
 * @brief the purpose of an actor type is to provide a specific instance of an
 *          animated model.  Actors might be used by entities, particles or UI
 * unlike other resources, the actor is not meant to be allocated or de-allocated.
 * @note Actors have a frameRate and individual actions also have frameRates.  The Actor 
 * Frame Rate is a scaling factor for all actions, while the frame rate on all actions is for
 * timing out their own pace.  This means that if you want to speed up or slow down an actor as
 * a whole, you change the actor's frame rate.  If you want to speed up just a specific action,
 * you need to change that action's frame rate
 */

typedef struct MglActor_S MglActor;


/**
 * @brief initialize the actor resource manager
 * @param maxActors how many simulataneous actors this system will support
 * @param useTiming if animations should be synchronized by time or default to render frames
 * @param ticksPerFrame if using timing, how many ticks per frame should correlate to one frame
 */
void mgl_actor_init(
    MglUint maxActors,
    MglBool useTiming,
    MglUint ticksPerFrame
);

/**
 * @brief initialize the actor resource manager based on information in the provided config file
 * @param filename the config file to load data for.
 */
void mgl_actor_init_from_config(char *filename);

/**
 * @brief loads an actor from file using the resource manager
 * Loads any dependant resources (Sprite, etc)
 * @param the actor file to load.
 * @return NULL on error or your loaded actor on success
 */
MglActor * mgl_actor_load(char  *filename);

/**
 * @brief frees a loaded actor from memory
 * Sets your pointer to NULL
 * @param actor a pointer to your actor pointer
 */
void mgl_actor_free(MglActor **actor);

/**
 * @brief loads an actor from file and saves it into the actor data provided.
 * based on the file information, it will also load up mesh, sprite, etc data.
 *
 * @param act the actor data to load into.  It will clear any existing data.
 * @param filename the model file to load
 */
void mgl_actor_load_in_place(MglActor *act, char *filename);

/**
 * @brief clears any of the actor information at the location specified
 */
void mgl_actor_clear(MglActor *act);

/**
 * @brief allocated empty actor data;
 *
 * @return a pointer to a valid empty actor or NULL on error
 */
MglActor * mgl_actor_new();

/**
 * @brief frees all data pointed to by actor
 *
 * @param actor a pointer to an actor pointer.  Make sure data can be freed.
 */
void mgl_actor_free(MglActor **actor);

/**
 * @brief sets the actor's frame change rate.  Consider this an overall scaling
 * factor for this actor.  Ie: a rate of 1 is normal speed, < 1 is a slowdow,
 * > 1 is a speed up.
 * Note that actions have their own frame rates that will combine with the
 * actor frame rate to determine the final change in frames.
 *
 * @param act the actor whose frame rate will change
 * @param rate the amount of change.  Note that a rate of 0 means the actor
 * will not animate
 */
void mgl_actor_set_frame_rate(
    MglActor *actor,
    MglFloat rate
);

/**
 * @brief set the actor to the begining of the specified action
 *
 * @param act the actor to modify
 * @param action the name of the action to set it to.
 *        If name is not found, nothing changes.
 */
void mgl_actor_set_action(
    MglActor * actor,
    MglLine    action
);

/**
 * @brief steps an actor to the next frame in an action.  lets caller know if
 *        if the action has reached the end.
 * if the actor was already at the end, it will...
 *        if type is loop: set the frame back to the beginning.
 *        if type is osci: sets the animation direction to -1
 *        if type is pass: frame is not changed
 *
 * @param act the actor to animate
 * @return MglTrue if the action has reached the end, false otherwise.
 * looping never returns false
 * pass returns true after a single pass
 * oscillate returns true after a single pass forward and back.
 */
MglBool mgl_actor_next_frame(MglActor * actor);

/**
 * @brief retrieve the current frame of an actor
 *
 * @param act the actor to retrieve the frame from
 *
 * @return a floating point representing the frame.  Note that when drawing,
 *         it will snap to the floor.
 */
MglFloat mgl_actor_get_frame(MglActor *actor);

/**
 * @brief get the actor's width and height
 * @param actor the actor to check
 * @param w [output] optional if provided this will be populated with the actor's width
 * @param h [output] optional if provided this will be populated with the actor's height
 */
void mgl_actor_get_size(MglActor *actor,MglUint *w,MglUint *h);

/**
 * @brief draw the actor to the current rendering context
 *
 * @param act pointer to the actor to draw
 * @param position the place to draw the actor
 * @param rotation how the actor is rotated
 * @param scale how the actor is scaled
 * @param flip if the actor should be flipped along the x/y axis
 * @param color color shifting of the actor (1,1,1,1) is no change.
 */
void mgl_actor_draw(
    MglActor *actor,
    MglVec2D position,
    MglFloat rotation,
    MglVec2D *scale,
    MglVec2D *flip,
    MglVec4D *color
);


#endif
