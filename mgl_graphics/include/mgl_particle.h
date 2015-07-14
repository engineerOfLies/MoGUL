#ifndef __MGL_PARTICLE_H__
#define __MGL_PARTICLE_H__

/**
 * mgl_particle
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

#include "mgl_vector.h"
#include "mgl_actor.h"

/**
 * @brief Type enumerates different strategies for handling a virtual 3rd dimension for particle rendering
 */
typedef enum {
    MglParticleZNone,   /**<ignore Z*/
    MglParticleZScale, /**<z scales y and x values towards center*/
    MglParticleZMatrix  /**<a matrix will be applied for all particle positions for projection*/
} MglParticleZHandler;

typedef struct
{
    MglVec3D position;      /**<position of the particle.*/
    MglVec3D velocity;      /**<velocity of the particle*/
    MglVec3D acceleration;  /**<acceleration of the particle*/
    
    MglColor color;         /**<color of the particle*/
    MglBool  useColorVec;   /**<if true, use the color vector*/
    MglColor colorVec;      /**<color vector, rate of change of the color*/
    MglBool  useColorTarget;/**<if true, use the color target*/
    MglColor colorTarget;   /**<color target, this is the color the particle should be before the particle dies*/
    
    MglActor *actor;        /**<if specified, this particle will use an actor for the graphics*/
    
    MglInt   timeToLive;    /**<How many frames this particle should live for*/
    MglUint  lifeSpan;      /**<How long this particle should live*/
}MglParticle;

/**
 * @brief initialize the particle system
 */
void mgl_particle_init(
    MglUint maxParticles,
    MglParticleZHandler strategy,
    MglVec2D *scale
);

/**
 * @brief retrieve a new particle
 * NOTE:Particles auto-cleanup and there is no free function
 * @param lifeSpan how many frames this particle should live
 * @return NULL on error or out of particles, a particle otherwise
 */
MglParticle * mgl_particle_new(MglUint lifeSpan);

/**
 * @brief updates all of the particles in the system
 */
void mgl_particle_update();

/**
 * @brief draws all of the particles in the system
 */
void mgl_particle_draw();

/**
 * @brief create a particle spray from one point
 * @param postion the starting point of the spray
 * @param direction the direction and velocity of the spray
 * @param spread how much error will be introduced to the direction of the particles
 * @param count how many particles to make
 * @param life how long the particles should live
 * @param color what color (with some error)
 * @param colorSpread error introduced to color
 */
void mgl_particle_spray(MglVec2D position,MglVec2D direction,MglVec2D spread,MglUint count,MglUint life, MglVec4D color, MglFloat colorSpread);

#endif
