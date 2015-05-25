#include "mgl_draw.h"
#include "mgl_graphics.h"
#include "mgl_config.h"
#include "mgl_dict.h"
#include "mgl_logger.h"
#include <string.h>
#include <SDL.h>
/**
 * @purpose mgl_graphics_test is meant to test the core graphics lib of MoGUL
 */

void init_all();

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
  MglUint sw,sh;
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

  mgl_graphics_get_screen_resolution(&sw,&sh);
  while (!done)
  {
    mgl_graphics_clear_screen();
    
    /*circles*/
    draw_candle(mgl_vec2d(sw/2,sh/2+10));
    draw_candle(mgl_vec2d(sw/2 - 90,sh/2+10));
    draw_candle(mgl_vec2d(sw/2 + 90,sh/2+10));
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE])
    {
      done = 1;
    }
    mgl_grahics_next_frame();
    printf("fps:%f\n",mgl_graphics_get_frames_per_second());
  }
  
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
