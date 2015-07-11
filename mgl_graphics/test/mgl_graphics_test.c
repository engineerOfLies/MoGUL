#include "mgl_draw.h"
#include "mgl_sprite.h"
#include "mgl_graphics.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include "mgl_logger.h"
#include "mgl_actor.h"
#include "mgl_font.h"

#include <string.h>
#include <SDL.h>
/**
 * @purpose mgl_graphics_test is meant to test the core graphics lib of MoGUL
 */

void init_all();


void draw_trinity(MglVec2D position,MglFloat r,MglVec4D color)
{
    MglVec2D top = {position.x,position.y-r};
    MglVec2D left = {position.x - (r * sin(MGL_2PI/6)),position.y + (r * cos(MGL_2PI/6))};
    MglVec2D right = {position.x + (r * sin(MGL_2PI/6)),position.y + (r * cos(MGL_2PI/6))};
    mgl_draw_bezier(top, right,left,color);
    mgl_draw_bezier(top, left,right,color);
    mgl_draw_bezier(left, top,right,color);
    mgl_draw_circle(position, r * 0.8, color);
}

void draw_candle_light(MglVec2D position)
{
  static float intensity = 0;
  intensity = ((mgl_random()+0.2)+intensity*10)/11;
  mgl_draw_solid_circle(position, 100*intensity, mgl_vec4d(255,32,0,60*intensity));
  mgl_draw_solid_circle(position, 75*intensity, mgl_vec4d(255,64,0,120*intensity));
  mgl_draw_solid_circle(position, 50*intensity, mgl_vec4d(255,128,0,220*intensity));
  mgl_draw_solid_circle(position, 25*intensity, mgl_vec4d(255,255,0,255*intensity));
}

void draw_candle(MglVec2D position)
{
  draw_candle_light(mgl_vec2d(position.x,position.y-20));
  mgl_draw_solid_rect(mgl_rect(position.x-7,position.y+10,14,180),mgl_vec4d(255,255,255,255));
  mgl_draw_solid_rect(mgl_rect(position.x-15,position.y+10,30,180),mgl_vec4d(255,255,255,128));
  mgl_draw_solid_rect(mgl_rect(position.x-22,position.y+10,44,180),mgl_vec4d(255,255,255,128));
  mgl_draw_solid_rect(mgl_rect(position.x-30,position.y+10,60,180),mgl_vec4d(255,255,255,128));
}

int main(int argc,char *argv[])
{
  init_all();
  int done = 0;
  const Uint8 *keys = NULL;
  char *confFile = NULL;
  int frame = 0;
  MglSprite *bgimage;
  MglSprite *sprite;
  MglActor *actor;
  MglUint sw,sh;
  MglInt swoosh = 0;
  MglInt dir = 1;
  MglLines *lines;
  MglVec3D rotation = {16,3,0};
  MglVec2D scale = {1,1};
  MglVec2D scaleCenter = {16,3};
  MglVec4D colorKey = {0,0,0,255};
  MglVec4D color = {0,255,0,128};
  MglVec2D flip = {1,0};
  MglVec2D patrol = {0,0};
  MglVec2D mechaScale = {2,2};
  MglInt    pDir = 1;
  if (((argc == 2) && (strcmp(argv[1],"-h")==0))||(argc < 2))
  {
      fprintf(stdout,"usage:\n");
      fprintf(stdout,"%s [graphics config file]\n",argv[0]);
      return 0;
  }
  confFile = argv[1];
  mgl_logger_message("mgl_graphics_test begin\n");

  if (mgl_graphics_init_by_config(confFile) != 0)
  {
      mgl_logger_info("failed to load graphics, exiting...");
      return 0;
  }
  mgl_sprite_init_from_config(confFile);
  mgl_actor_init(
      1000,
      0,
      33
  );
  mgl_font_init(
      10,
      "test/fonts/Exo-Regular.otf",
      16
  );
  
  sprite = mgl_sprite_load_from_image(
      "test/rotationscale.png",
      32,
      64,
      16,
      NULL,
      NULL,
      NULL,
      &colorKey);
  bgimage = mgl_sprite_load_image("test/images/linux_desktop.png");
  actor = mgl_actor_load("test/actors/mecha.actor");

  mgl_graphics_get_screen_resolution(&sw,&sh);

  lines = mgl_shape_lines_new();
  
  mgl_shape_lines_append_point(lines,mgl_vec2d(100,100));
  mgl_shape_lines_append_point(lines,mgl_vec2d(200,100));
  mgl_shape_lines_append_point(lines,mgl_vec2d(200,200));
  mgl_shape_lines_append_point(lines,mgl_vec2d(100,200));
  mgl_shape_lines_append_point(lines,mgl_vec2d(300,100));
  mgl_shape_lines_append_point(lines,mgl_vec2d(300,300));
  mgl_shape_lines_append_point(lines,mgl_vec2d(100,300));
  
  mgl_actor_set_action(
      actor,
      "walk_right"
  );
  
  patrol.y = sh/2-24;
  patrol.x = sw/2-24;
  while (!done)
  {
    mgl_graphics_clear_screen();
    mgl_sprite_draw_image(bgimage,mgl_vec2d(0,0));
    
    swoosh += dir;
    if (swoosh >= sw)dir = -1;
    if (swoosh <= 0)dir = 1;

    /*circles*/
    draw_candle(mgl_vec2d(sw - sw/5,sh/4+10));
    /*mgl_draw_bezier(mgl_vec2d(500,sh), mgl_vec2d(swoosh,1),mgl_vec2d(sw-100,sh),mgl_vec4d(255,128,64,255));*/

    /*draw_trinity(mgl_vec2d(sw/2,sh/2),100,mgl_vec4d(0,255,0,255));
    draw_trinity(mgl_vec2d(sw/2,sh/2),120,mgl_vec4d(0,255,0,255));*/
    /*mgl_draw_line_sequence(lines,mgl_vec4d(255,0,0,255));*/
    rotation.z+=0.5;
    scale.x = scale.y = (scale.x + 0.01);
    if (scale.x >= 4)
    {
        scale.x = scale.y = 0.1;
    }
    mgl_draw_circle(mgl_vec2d(sw/2,sh/4), 64, mgl_vec4d(255,32,0,255));
    mgl_sprite_draw(
        sprite,
        mgl_vec2d(sw/2,sh/4),
        &scale,
        &scaleCenter,
        &rotation,
        &flip,
        &color,
        frame);
    mgl_draw_pixel(mgl_vec2d(sw/2,sh/4),mgl_vec4d(255,0,255,255));
    
    //mgl_draw_triangle_solid(mgl_vec2d(sw/2,sh/4),mgl_vec2d(sw/3,sh/3),mgl_vec2d(swoosh,sh/2),mgl_vec4d(255,0,255,64));
    
    //mgl_draw_triangle_solid(mgl_vec2d(sw/4,sh/4),mgl_vec2d(sw/3,sh/4),mgl_vec2d(swoosh/2,sh/2),mgl_vec4d(255,0,255,64));
    
    mgl_draw_triangle_solid(mgl_vec2d(sw/4,swoosh/3),mgl_vec2d(sw/5,sh * scale.x * 0.25),mgl_vec2d(sw/2,sh/4),mgl_vec4d(255,0,255,64));

//    mgl_draw_triangle_solid(mgl_vec2d(sw/2,sh/2),mgl_vec2d(sw/3,sh/4),mgl_vec2d(swoosh/2,sh/2),mgl_vec4d(255,0,255,64));
    /*mgl_draw_triangle(mgl_vec2d(sw/2,sh/4),mgl_vec2d(sw/3,sh/3),mgl_vec2d(sw,sh/2),mgl_vec4d(255,128,128,255));*/
    
    mgl_draw_solid_rect(mgl_rect(0,sh/2-10,sw,48),mgl_vec4d(255,255,255,128));
    mgl_actor_next_frame(actor);
    patrol.x += (pDir * 2);
    if (patrol.x > (sw - sw/4))
    {
        pDir = -1;
        mgl_actor_set_action(
            actor,
            "walk_left"
        );
    }
    if (patrol.x < sw/4)
    {
        pDir = 1;
        mgl_actor_set_action(
            actor,
            "walk_right"
        );
    }
    mgl_actor_draw(
        actor,
        patrol,
        0,
        &mechaScale,
        NULL,
        NULL
    );
    
    mgl_font_draw_text_basic(
        mgl_vec2d(50,50),
        "THIS IS TEXT",
        mgl_vec4d(0,0,255,255));

    mgl_font_draw_text_wrap(
        "THIS is even longer text that will end up wrapping around due to how long it is and the bounds provided",
        mgl_rect(350,100,100,300),
        mgl_vec4d(255,0,255,255),
        mgl_font_default()
    );
    
    mgl_draw_line_curved(lines,mgl_vec4d(255,0,255,255));
    
    
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE])
    {
      done = 1;
    }
    mgl_grahics_next_frame();
    printf("fps:%f\n",mgl_graphics_get_frames_per_second());
  }
  mgl_shape_lines_free(&lines);
  mgl_logger_message("mgl_graphics_test end\n");

}


void init_all()
{
    mgl_logger_init();
    mgl_logger_set_stdout_echo(MglTrue);
    mgl_logger_set_threshold(MGL_LOG_ALL);
    mgl_config_init();
}

/*eol@eof*/
