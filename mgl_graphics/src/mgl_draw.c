#include "mgl_draw.h"
#include "mgl_graphics.h"
#include "mgl_logger.h"

void mgl_draw_pixel(MglVec2D point,MglVec4D color)
{
    SDL_Surface *surface;
    MglRect pixel;
    MglUint pixelColor;
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(1,1);
    pixelColor = mgl_graphics_vec_to_surface_color(surface,color);
    SDL_FillRect(surface,NULL,pixelColor);
    mgl_rect_set(&pixel,point.x,point.y,1,1);
    /*blit surface to the screen surface*/
    mgl_graphics_blit_surface_to_screen(surface,NULL,&pixel);
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

void mgl_draw_solid_rect(MglRect rect, MglVec4D color)
{
    SDL_Surface *surface;
    MglUint rectColor;
    MglRect copyRect;
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(rect.w,rect.h);
    rectColor = mgl_graphics_vec_to_surface_color(surface,color);
    SDL_FillRect(surface,NULL,rectColor);
    /*blit surface to the screen surface*/
    mgl_rect_set(&copyRect,0,0,rect.w,rect.h);
    mgl_graphics_blit_surface_to_screen(surface,&copyRect,&rect);
}

void mgl_draw_solid_rect_to_surface(SDL_Surface *surface,MglRect rect, MglVec4D color)
{
  MglUint drawColor = 0;
  if (!surface)
  {
    mgl_logger_warn("mgl_draw_solid_rect_to_surface: no surface provided");
    return;
  }
  drawColor = mgl_graphics_vec_to_surface_color(surface,color);
  SDL_FillRect(surface,
               &rect,
               drawColor);
}

void mgl_draw_rect(MglRect rect, MglVec4D color)
{
    SDL_Surface *surface;
    MglUint clearColor;
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(rect.w,rect.h);
    clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
    SDL_FillRect(surface,NULL,clearColor);
    /*draw circle to the new surface*/
    
    mgl_draw_rect_to_surface(surface,mgl_rect(0,0,rect.w,rect.h), color);
    /*blit surface to the screen surface*/
    mgl_graphics_blit_surface_to_screen(surface,NULL,&rect);
}

void mgl_draw_rect_to_surface(SDL_Surface *surface,MglRect rect, MglVec4D color)
{
  MglUint drawColor = 0;
  MglRect drawRect = {0,0,0,0};
  if (!surface)
  {
    mgl_logger_warn("mgl_draw_rect_to_surface: no surface provided");
    return;
  }
  drawColor = mgl_graphics_vec_to_surface_color(surface,color);
  /*top*/
  mgl_rect_set(&drawRect,rect.x,rect.y,rect.w,1);
  SDL_FillRect(surface,
               &drawRect,
               drawColor);
  /*bottom*/
  mgl_rect_set(&drawRect,rect.x,rect.y+rect.h - 1,rect.w,1);
  SDL_FillRect(surface,
               &drawRect,
               drawColor);
  /*left*/
  mgl_rect_set(&drawRect,rect.x,rect.y,1,rect.h);
  SDL_FillRect(surface,
               &drawRect,
               drawColor);
  /*right*/
  mgl_rect_set(&drawRect,rect.x+rect.w - 1,rect.y,1,rect.h);
  SDL_FillRect(surface,
               &drawRect,
               drawColor);
}

void mgl_draw_line(MglVec2D p1,MglVec2D p2,MglVec4D color)
{
    SDL_Surface *surface;
    MglRect area;
    MglUint clearColor;
    int w,h,minx,miny;
    w = abs((int)p1.x - (int)p2.x)+2;
    h = abs((int)p1.y - (int)p2.y)+2;
    minx = MIN(p1.x,p2.x);
    miny = MIN(p1.y,p2.y);
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(w,h);
    clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
    SDL_FillRect(surface,NULL,clearColor);
    /*draw circle to the new surface*/
    
    mgl_draw_line_to_surface(surface,mgl_vec2d(p1.x - minx,p1.y - miny),mgl_vec2d(p2.x - minx,p2.y - miny),color);
    mgl_rect_set(&area,minx,miny,w,h);
    /*blit surface to the screen surface*/
    mgl_graphics_blit_surface_to_screen(surface,NULL,&area);
}

void mgl_draw_line_to_surface(SDL_Surface *surface,MglVec2D p1,MglVec2D p2,MglVec4D color)
{
  int deltax,deltay;
  int x,y,curpixel;
  int den,num,numadd,numpixels;
  int xinc1,xinc2,yinc1,yinc2;
  if (!surface)
  {
    mgl_logger_warn("mgl_draw_line_to_surface: no surface provided");
    return;
  }
  deltax = fabs(p2.x - p1.x);
  deltay = fabs(p2.y - p1.y);
  x = p1.x;
  y = p1.y;
  
  if (p2.x >= p1.x)
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (p2.y >= p1.y)
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)
  {
    xinc1 = 0;
    yinc2 = 0;
    den = deltax;
    num = deltax >> 1;
    numadd = deltay;
    numpixels = deltax;
  }
  else
  {
    xinc2 = 0;
    yinc1 = 0;
    den = deltay;
    num = deltay >> 1;
    numadd = deltax;
    numpixels = deltay;
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(x,y),color);
    num += numadd;
    if (num >= den)
    {
      num -= den;
      x += xinc1;
      y += yinc1;
    }
    x += xinc2;
    y += yinc2;
  }
}

/*
 * credit circle drawing algorith:
 * http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
 */

static void mgl_draw_circle_points(SDL_Surface *surface,MglVec2D center, MglVec2D point, MglVec4D color)
{  
  if (point.x == 0)
  {
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x, center.y + point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x, center.y - point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.y, center.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.y, center.y),color);
  }
  else if (point.x == point.y)
  {
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.x, center.y + point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.x, center.y + point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.x, center.y - point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.x, center.y - point.y),color);
  }
  else if (point.x < point.y)
  {
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.x, center.y + point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.x, center.y + point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.x, center.y - point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.x, center.y - point.y),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.y, center.y + point.x),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.y, center.y + point.x),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x + point.y, center.y - point.x),color);
    mgl_draw_pixel_to_surface(surface,mgl_vec2d(center.x - point.y, center.y - point.x),color);
  }
}

void mgl_draw_circle(MglVec2D center, int r, MglVec4D color)
{
  SDL_Surface *surface;
  MglRect area;
  MglUint clearColor;
  /*make an empty surface*/
  surface = mgl_graphics_get_temp_buffer(2*r,2*r);
  clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
  SDL_FillRect(surface,NULL,clearColor);
  /*draw circle to the new surface*/
  mgl_draw_circle_to_surface(surface,mgl_vec2d(r,r), r, color);
  mgl_rect_set(&area,center.x-r,center.y-r,2*r,2*r);
  /*blit surface to the screen surface*/
  mgl_graphics_blit_surface_to_screen(surface,NULL,&area);
}

void mgl_draw_circle_to_surface(SDL_Surface *surface,MglVec2D center, int radius, MglVec4D color)
{
  MglVec2D point = {0,0};
  int p = (5 - radius*4)/4;
  point.y = radius;
  if (!surface)
  {
    mgl_logger_warn("mgl_draw_circle: no surface provided");
    return;
  }
  mgl_draw_circle_points(surface,center, point, color);
  while (point.x < point.y) {
    point.x++;
    if (p < 0) {
      p += 2*point.x+1;
    } else {
      point.y--;
      p += 2*(point.x-point.y)+1;
    }
    mgl_draw_circle_points(surface,center, point, color);
  }
}

static void mgl_draw_solid_circle_points(SDL_Surface *surface,MglVec2D center, MglVec2D point, MglVec4D color)
{
  MglUint drawColor;
  MglRect drawRect;
  drawColor = mgl_graphics_vec_to_surface_color(surface,color);
  if (point.x == 0)
  {
    mgl_rect_set(&drawRect,center.x,center.y - point.y,1, 2 * point.y);
    SDL_FillRect(surface,
                 &drawRect,
                 drawColor);

    mgl_rect_set(&drawRect,center.x - point.y, center.y,2 * point.y, 1);
    SDL_FillRect(surface,
                 &drawRect,
                 drawColor);
  }
  else if (point.x == point.y)
  {
    mgl_rect_set(&drawRect,center.x - point.x,center.y - point.y,2 * point.x, 2 * point.y);
    SDL_FillRect(surface,
                 &drawRect,
                 drawColor);
    
  }
  else if (point.x < point.y)
  {
    mgl_rect_set(&drawRect,center.x - point.x,center.y - point.y,2 * point.x, 2 * point.y);
    SDL_FillRect(surface,
                 &drawRect,
                 drawColor);

    mgl_rect_set(&drawRect,center.x - point.y,center.y - point.x,2 * point.y, 2 * point.x);
    SDL_FillRect(surface,
                 &drawRect,
                 drawColor);
  }
}

void mgl_draw_solid_circle(MglVec2D center, int r, MglVec4D color)
{
  SDL_Surface *surface;
  MglRect area;
  MglUint clearColor;
  /*make an empty surface*/
  surface = mgl_graphics_get_temp_buffer(2*r,2*r);
  clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
  SDL_FillRect(surface,NULL,clearColor);
  /*draw circle to the new surface*/
  mgl_draw_solid_circle_to_surface(surface,mgl_vec2d(r,r), r, color);
  mgl_rect_set(&area,center.x-r,center.y-r,2*r,2*r);
  /*blit surface to the screen surface*/
  mgl_graphics_blit_surface_to_screen(surface,NULL,&area);
}

void mgl_draw_solid_circle_to_surface(SDL_Surface *surface,MglVec2D center, int radius, MglVec4D color)
{
  MglVec2D point = {0,0};
  int p = (5 - radius*4)/4;
  point.y = radius;
  if (!surface)
  {
    mgl_logger_warn("mgl_draw_circle: no surface provided");
    return;
  }
  mgl_draw_solid_circle_points(surface,center, point, color);
  while (point.x < point.y) {
    point.x++;
    if (p < 0) {
      p += 2*point.x+1;
    } else {
      point.y--;
      p += 2*(point.x-point.y)+1;
    }
    mgl_draw_solid_circle_points(surface,center, point, color);
  }
}

/*eol@eof*/