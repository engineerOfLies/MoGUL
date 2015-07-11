#ifndef __MGL_GRAPHICS_H__
#define __MGL_GRAPHICS_H__

/**
 * mgl_graphics
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
#include "mgl_rect.h"
#include "mgl_sprite.h"
#include "mgl_actor.h"
#include "mgl_font.h"
#include "mgl_draw.h"
#include "mgl_shape.h"
#include <SDL.h>

/**
 * @purpose mgl_graphics initializes and configures rendering pipleline and provides an interface to 
 * do basic drawing and frame timing.
 */

/**
 * @brief initializes the mgl sub systems and sets up the video system based on the parameters passed
 *
 * @param windowName the name for the main game window
 * @param viewWidth the screen width, if -1 defaults to system width 
 * @param viewHeight the screen height, if -1 defaults to system 
 * @param renderWidth the desired functional width of the game screen
 * @param renderHeight the desired functional height of the game screen
 * @param bgcolor the default background color
 * @param fullscreen if true, initialize in fullscreen mode, otherwise you get windowed mode
 * @return 0 on success, otherwise an error.  See the logs for info
 */

void mgl_graphics_init(
    char *windowName,
    MglInt viewWidth,
    MglInt viewHeight,
    MglInt renderWidth,
    MglInt renderHeight,
    MglVec4D bgcolor,
    MglBool fullscreen
);

/**
 * @brief Initialize the graphics system with the config file specified
 * Uses the config file loaded to initialize the graphics system.  If the file fails to load,
 * the graphics system will not initialize
 * 
 * @param configFile the file path to the config file to loaded.  Must be supported by mgl_config
 * @return 0 on success, otherwise an error.  See the logs for info
 */
int mgl_graphics_init_by_config(char *configFile);

/**
 * @brief return a pointer to the active main game screen rendering surface.
 * 
 * @return NULL on error, or a pointer to the sdl surface that will be copied to the screen on next render
 */
SDL_Surface *mgl_graphics_get_screen_surface();

/**
 * @brief create a new SDL surface that is compatable with the screen surface
 * It should be freed with SDL_FreeSurface
 * @param w the width for the new surface
 * @param h the height for the new surface
 * @return NULL on error or a newly created SDL Surface.
 */
SDL_Surface *mgl_graphics_create_surface(MglUint w,MglUint h);

/**
 * @brief convert the provided surface to the screen's format
 * If Successful frees the surface provided and sets your pointer to NULL
 * @param surface a pointer to your surface pointer
 * @return NULL on error, or a new SDL Surface with the screen pixel format
 */
SDL_Surface *mgl_graphics_screen_convert(SDL_Surface **surface);

/**
 * @brief get the active temp drawing buffer at least as big as the specified width and height.
 * This surface should NOT be freed when done with and should not be held onto for very long
 * @param w the desired width of the temp buffer.  
 * @param h the desired height of the temp buffer.  
 * @return a pointer to the temp drawing buffer
 */
SDL_Surface *mgl_graphics_get_temp_buffer(int w,int h);

/*
 * TODO: support multiple rendering contexts
 */
/**
 * @brief get the active rendering context
 * @return NULL on error or the active rendering context
 */
SDL_Renderer *mgl_graphics_get_renderer();

/**
 * @brief copy one surface on the current screen surface
 * @param surface the source of the copy
 * @param srcRect the clip rectangle for the source surface
 * @param dstRect the clip rectangle for the destination surface (the sceen)
 */
void mgl_graphics_blit_surface_to_screen(SDL_Surface *surface,const MglRect * srcRect,const MglRect * dstRect);

/**
 * @brief renders the provided surface to the screen renderer
 * 
 * @param surface the surface to render
 * @param srcRect the rectangle clip area to render
 * @param position the position on the screen to render to
 * @param scale the width and height to scale the image to  (1,1) is no scaling
 * @param rotation the roation point (x,y and rotation angle (z) to rotate the image
 */
void mgl_graphics_render_surface_to_screen(SDL_Surface *surface,MglRect srcRect,MglVec2D position,MglVec2D scale,MglVec3D rotation);

/**
 * @brief renders the contents of the screen buffer to the physical screen.  Internally waits to 
 * ensure a steady frame rate based on the frameDelay configured
 */
void mgl_grahics_next_frame();

/**
 * @brief set the internal frame delay to wait between rendering frames.  This is to ensure a steady frame rate.
 */
void mgl_graphics_set_frame_delay(MglUint frameDelay);

/**
 * @brief get the rendering time for the current game timestamp
 * 
 * @return the number of ticks that have passed since the start of the program and the last rendered frame.
 */
MglUint mgl_graphics_get_render_time();

/**
 * @brief clears the rendering screen with the default color or background image if it is set
 */
void mgl_graphics_clear_screen();

/**
 * @brief set the default background color when clearing the screen
 * 
 * @param bgcolor the color vector (r,g,b)
 */
void mgl_graphics_set_bgcolor(MglVec3D bgcolor);

/**
 * @brief convert a component color vector (r,g,b,a) to the SDL color expected by the surface provided
 * 
 * @param surface the surface that the color will be made compatable for
 * @param color the component vector where x=>r, y=>g, z=>b, w=>a.
 * @return an SDL color that will work with the surface provided
 */
MglUint mgl_graphics_vec_to_surface_color(SDL_Surface *surface,MglVec4D color);

/**
 * @brief convert a component color vector (r,g,b,a) to the SDL color expected by the loaded screen surface
 * 
 * @param color the component vector where x=>r, y=>g, z=>b, w=>a.
 * @return an SDL color that will work with the screen surface
 */
MglUint mgl_graphics_vec_to_screen_color(MglVec4D color);

/**
 * @brief returns the color information for the specified surface pixel
 * Surface should be locked prior to calling this function.
 * @param surface the surface to query
 * @param position the x and y coordinates of the pixel to check (rounded to integer space)
 * 
 * @return 0 if color is black or there is a problem.  Errors are logged
 */
MglUint mgl_graphics_get_surface_pixel(SDL_Surface *surface,MglVec2D position);

/**
 * @brief sets the pixel of the surface to the specified color
 * Surface should be locked priot to calling this function.  This should not be considered a drawing tool.
 * @param surface the surface to change
 * @param position the x and y coordinates of the pixel to set
 * @param color the color to set the pixel to, assumed to be in the same format as the surface
 */
void mgl_graphics_set_surface_pixel(SDL_Surface *surface,MglVec2D position,MglUint color);

/**
 * @brief draws a pixel to the active rendering context
 * @param pixel the coordinates of the pixel to draw
 * @param color the color to set the pixel too.
 */
void mgl_graphics_render_pixel(MglVec2D pixel,MglVec4D color);

/**
 * @brief draws a line to the active rendering context
 * @param p1 end point 1 of the line
 * @param p2 end point 2 of the line
 * @param color the color to draw
 */
void mgl_graphics_render_line(MglVec2D p1,MglVec2D p2, MglVec4D color);

/**
 * @brief draws a series lines to the active rendering context
 * @param p1 list of end point 1's of the lines
 * @param p2 list of end point 2's of the lines
 * @param color the color to draw
 */
void mgl_graphics_render_lines(MglVec2D *p1,MglVec2D *p2, MglUint lines,MglVec4D color);

/**
 * @brief get the currect render screen surface resolution
 * 
 * @param w [output] if provided, this will be populated with the screen's width
 * @param h [output] if provided, this will be populated with the screen's height
 * 
 * @return 0 on success, other on error
 */
MglInt mgl_graphics_get_screen_resolution(MglUint *w,MglUint *h);

/**
 * @brief get the current calculated frames per second
 * @return the fps in float
 */
MglFloat mgl_graphics_get_frames_per_second();

#endif
