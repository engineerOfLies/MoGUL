#include "mgl_parallax.h"

#include "mgl_sprite.h"
#include "mgl_actor.h"

#include <glib.h>

/**
 * @brief the layer type holds information on how to draw a given layer of the parallax background
 */
typedef struct
{
    MglSprite *image; /**<the image to draw*/
    MglActor  *actor; /**<optionally an actor can be used instead*/
    MglVec2D   aspect;/**<the percent offset from the camera position this should be drawn at.*/
}MglLayer;

/**
 * @brief a parallax background context
 */
struct MglParallax_S
{
    GList *layers;      /**<the list of layers that make up this parallax background context*/
    GList *cameraPlane;  /**<the layer that is drawn 1:1 to the camera / All other planes are moved accordingly*/
};


/*eol@eof*/
