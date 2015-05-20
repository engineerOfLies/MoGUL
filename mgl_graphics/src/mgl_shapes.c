#include "mgl_shapes.h"
#include "mgl_graphics.h"
#include "mgl_logger.h"

void mgl_draw_pixel(MglVec2D point,MglVec4D color)
{
    SDL_Surface *surface;
    surface = mgl_graphics_get_screen_surface();
    mgl_draw_pixel_to_surface(surface,point,color);
}

void mgl_draw_pixel_to_surface(SDL_Surface *surface,MglVec2D point,MglVec4D color)
{
    MglUint drawColor = 0;
    MglRect pixel = {0,0,1,1};
    if (!surface)
    {
        mgl_logger_warn("mgl_draw_pixel_to_surface: no surface provided");
        return;
    }
    pixel.x = point.x;
    pixel.y = point.y;
    drawColor = mgl_graphics_vec_to_surface_color(surface,color);
    SDL_FillRect(surface,
                 &pixel,
                 drawColor);
}

/*eol@eof*/
