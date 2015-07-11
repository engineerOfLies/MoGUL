#include "mgl_draw.h"
#include "mgl_dict.h"
#include "mgl_graphics.h"
#include "mgl_logger.h"

void mgl_draw_pixel(MglVec2D point,MglVec4D color)
{
    mgl_graphics_render_pixel(point,color);
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
    mgl_graphics_render_surface_to_screen(surface,copyRect,mgl_vec2d(rect.x,rect.y),mgl_vec2d(1,1),mgl_vec3d(0,0,0));
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

void mgl_draw_line_sequence(MglLines *lines,MglVec4D color)
{
    int count,i;
    MglVec2D point,lastpoint;
    if (!lines)return;
    count = mgl_shape_lines_get_count(lines);
    if (count < 2)return;
    mgl_shape_lines_get_nth_point(&lastpoint,lines, 0);
    for (i = 1; i < count;i++)
    {
        mgl_shape_lines_get_nth_point(&point,lines, i);
        mgl_draw_line(lastpoint,point,color);
        mgl_vec2d_copy(lastpoint,point);
    }
}

void mgl_draw_line_curved(MglLines *lines,MglVec4D color)
{
    int count,i;
    MglBool cleanup = MglFalse;
    MglLines *path;
    MglVec2D point,lastpoint,midpoint;
    if (!lines)return;
    count = mgl_shape_lines_get_count(lines);
    if (count < 2)return;
    
    if (count < 3)
    {
        /*draw a line*/
        return;
    }
    if (count > 3)
    {
        cleanup = MglTrue;
        /*subdivide as needed*/
        path = mgl_shape_lines_clone(lines);
        mgl_vec2d_clear(lastpoint);
        for (i = 1;i < count - 2;i+=2)
        {
            mgl_lines_subdivide_line(path,i);
            count++;
        }
        count = mgl_shape_lines_get_count(path);
    }
    else
    {
        path = lines;
    }
    /*draw curves*/
    mgl_shape_lines_get_nth_point(&point,path, 0);
    for (i = 1;i < count;i+=2)
    {
        mgl_vec2d_copy(lastpoint,point);
        mgl_shape_lines_get_nth_point(&midpoint,path, i);
        mgl_shape_lines_get_nth_point(&point,path, i+1);
        
        mgl_draw_bezier(lastpoint, midpoint,point,color);        

    }
    if (cleanup)
    {
        mgl_shape_lines_free(&path);
    }
}


void mgl_draw_line(MglVec2D p1,MglVec2D p2,MglVec4D color)
{
    mgl_graphics_render_line(p1,p2,color);
}

void mgl_draw_line_to_screen_surface(MglVec2D p1,MglVec2D p2,MglVec4D color)
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
  surface = mgl_graphics_get_temp_buffer((2*r)+1,(2*r)+1);
  clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
  SDL_FillRect(surface,NULL,clearColor);
  /*draw circle to the new surface*/
  mgl_draw_circle_to_surface(surface,mgl_vec2d(r,r), r, color);
  mgl_rect_set(&area,0,0,(2*r)+1,(2*r)+1);
  /*blit surface to the screen surface*/
  mgl_graphics_render_surface_to_screen(surface,area,mgl_vec2d(center.x-r,center.y-r),mgl_vec2d(1,1),mgl_vec3d(0,0,0));
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
  while (point.x < point.y)
  {
    point.x++;
    if (p < 0)
    {
      p += 2*point.x+1;
    }
    else
    {
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
  mgl_rect_set(&area,0,0,2*r,2*r);
  /*blit surface to the screen surface*/
  mgl_graphics_render_surface_to_screen(surface,area,mgl_vec2d(center.x-r,center.y-r),mgl_vec2d(1,1),mgl_vec3d(0,0,0));
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
  while (point.x < point.y)
  {
    point.x++;
    if (p < 0)
    {
      p += 2*point.x+1;
    }
    else
    {
      point.y--;
      p += 2*(point.x-point.y)+1;
    }
    mgl_draw_solid_circle_points(surface,center, point, color);
  }
}

void mgl_draw_bezier(MglVec2D p0, MglVec2D p1,MglVec2D p2,MglVec4D color)
{
    SDL_Surface *surface;
    MglRect area;
    MglUint clearColor;
    int w,h,minx,miny,maxx,maxy;
    minx = MIN(p0.x,MIN(p1.x,p2.x));
    miny = MIN(p0.y,MIN(p1.y,p2.y));
    maxx = MAX(p0.x,MAX(p1.x,p2.x));
    maxy = MAX(p0.y,MAX(p1.y,p2.y));
    w = abs(maxx - minx)+2;
    h = abs(maxy - miny)+2;
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(w,h);
    clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
    SDL_FillRect(surface,NULL,clearColor);
    /*draw circle to the new surface*/
    mgl_draw_bezier_to_surface(surface, mgl_vec2d(p0.x - minx,p0.y - miny), mgl_vec2d(p1.x - minx,p1.y - miny),mgl_vec2d(p2.x - minx,p2.y - miny), color);
    mgl_rect_set(&area,0,0,w,h);
    /*blit surface to the screen surface*/
    mgl_graphics_render_surface_to_screen(surface,area,mgl_vec2d(minx,miny),mgl_vec2d(1,1),mgl_vec3d(0,0,0));
}


void mgl_draw_bezier_to_surface(SDL_Surface *surface, MglVec2D p0, MglVec2D p1, MglVec2D p2, MglVec4D color)
{
    MglVec2D qp,qp2,qpv; /*approximation line starting point and vector*/
    MglVec2D p0v,p1v,temp; /*vectors from point to next point*/
    MglVec2D dp; /*draw point*/
    MglFloat t = 0;  /*time segment*/
    MglFloat tstep;
    MglFloat totalLength;
    totalLength = mgl_vec2d_magnitude(p0)+mgl_vec2d_magnitude(p1)+mgl_vec2d_magnitude(p2);
    if (totalLength == 0)
    {
        mgl_logger_warn("path provided is zero length.");
        return;
    }
    tstep = fabs(1.0/(totalLength * 0.9));
    mgl_vec2d_sub(p0v,p1,p0);
    mgl_vec2d_sub(p1v,p2,p1);
    for (t = 0; t <= 1;t += tstep)
    {
        /*calculate Q*/
        mgl_vec2d_scale(temp,p0v,t);
        mgl_vec2d_add(qp,p0,temp);
        
        mgl_vec2d_scale(temp,p1v,t);
        mgl_vec2d_add(qp2,p1,temp);
        
        mgl_vec2d_sub(qpv,qp2,qp);
        
        mgl_vec2d_scale(temp,qpv,t);
        mgl_vec2d_add(dp,qp,temp);
        
        mgl_draw_pixel_to_surface(surface,dp,color);
    }
}  

void mgl_draw_triangle(MglVec2D p1,MglVec2D p2,MglVec2D p3,MglColor color)
{
    mgl_draw_line(p1,p2,color);
    mgl_draw_line(p2,p3,color);
    mgl_draw_line(p3,p1,color);
}


void mgl_draw_triangle_top(MglVec2D v1, MglVec2D v2, MglVec2D v3,MglColor color,SDL_Surface *surface)
{
    int scanlineY;
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);
    
    float curx1 = v3.x;
    float curx2 = v3.x;
    
    for (scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        curx1 -= invslope1;
        curx2 -= invslope2;
        mgl_draw_line_to_surface(surface,mgl_vec2d(curx1, scanlineY),mgl_vec2d(curx2, scanlineY),color);
    }
}

void mgl_draw_triangle_bottom(MglVec2D v1, MglVec2D v2, MglVec2D v3,MglColor color, SDL_Surface *surface)
{
    int scanlineY;
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);
    
    float curx1 = v1.x;
    float curx2 = v1.x;
    
    for (scanlineY = (int)v1.y; scanlineY <= (int)v2.y; scanlineY++)
    {
        mgl_draw_line_to_surface(surface,mgl_vec2d(curx1, scanlineY),mgl_vec2d(curx2, scanlineY),color);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

gint mgl_vec2d_ysort (gconstpointer a, gconstpointer b)
{
    MglVec2D *A,*B;
    A = (MglVec2D *)a;
    B = (MglVec2D *)b;
    if (A->y < B->y)
    {
        return -1;
    }
    if (A->y < B->y)
    {
        return 1;
    }
    return 0;
}

void mgl_draw_triangle_solid(MglVec2D p1,MglVec2D p2,MglVec2D p3,MglColor color)
{
    SDL_Surface *surface;
    MglRect area;
    MglUint clearColor;
    GList *vertices = NULL;
    MglUint minx,miny,maxx,maxy;
    MglVec2D *v1,*v2,*v3;
    MglVec2D v4;
    
    minx = MIN(MIN(p1.x,p2.x),p3.x);
    miny = MIN(MIN(p1.y,p2.y),p3.y);
    maxx = MAX(MAX(p1.x,p2.x),p3.x);
    maxy = MAX(MAX(p1.y,p2.y),p3.y);
    vertices = g_list_append (vertices,&p1);
    vertices = g_list_insert_sorted (vertices,&p2,mgl_vec2d_ysort);
    vertices = g_list_insert_sorted (vertices,&p3,mgl_vec2d_ysort);
    v1 = g_list_nth_data(vertices,2);
    v2 = g_list_nth_data(vertices,1);
    v3 = g_list_nth_data(vertices,0);
    
    
    /*make an empty surface*/
    surface = mgl_graphics_get_temp_buffer(maxx - minx,maxy - miny);
    clearColor = mgl_graphics_vec_to_surface_color(surface,mgl_vec4d(0,0,0,0));
    SDL_FillRect(surface,NULL,clearColor);
    
    if (v2->y == v3->y)
    {
        mgl_draw_triangle_bottom(
            mgl_vec2d(v1->x - minx,v1->y-miny),
            mgl_vec2d(v2->x - minx,v2->y-miny),
            mgl_vec2d(v3->x - minx,v3->y-miny),
            color,
            surface);
    }
    /* check for trivial case of top-flat triangle */
    else if (v1->y == v2->y)
    {
        mgl_draw_triangle_top(
            mgl_vec2d(v1->x - minx,v1->y-miny),
            mgl_vec2d(v2->x - minx,v2->y-miny),
            mgl_vec2d(v3->x - minx,v3->y-miny),
            color,
            surface);
    }
    else
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
    
        mgl_vec2d_set(v4,(v1->x + ((float)(v2->y - v1->y) / (float)(v3->y - v1->y)) * (v3->x - v1->x)), v2->y);
        mgl_draw_triangle_bottom(
            mgl_vec2d(v1->x - minx,v1->y-miny),
            mgl_vec2d(v2->x - minx,v2->y-miny),
            mgl_vec2d(v4.x - minx,v4.y-miny),
            color,
            surface);
        mgl_draw_triangle_top(
            mgl_vec2d(v2->x - minx,v2->y-miny),
            mgl_vec2d(v4.x - minx,v4.y-miny),
            mgl_vec2d(v3->x - minx,v3->y-miny),
            color,
            surface);
    }
    g_list_free(vertices);
    /*draw circle to the new surface*/
    mgl_rect_set(&area,0,0,maxx - minx,maxy - miny);
    /*blit surface to the screen surface*/
    mgl_graphics_render_surface_to_screen(surface,area,mgl_vec2d(minx,miny),mgl_vec2d(1,1),mgl_vec3d(0,0,0));
}

/*eol@eof*/
