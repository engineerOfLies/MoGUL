#include "mgl_shapes.h"
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

int main(int argc,char *argv[])
{
  init_all();
  int done = 0;
  const Uint8 *keys = NULL;
  char *confFile = NULL;
  MglUint sw,sh;
  MglInt dx = 1 ,dy = 1;
  MglVec4D color = {0,255,0,255};
  MglVec2D position = {0,0};
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
    
    position.x += dx * 0.3;
    position.y += dy * 0.3;
    if (position.x > sw)dx = -1;
    if (position.x <= 0)dx = 1;
    if (position.y > sh)dy = -1;
    if (position.y <= 0)dy = 1;
    mgl_draw_pixel(position,color);

    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE])done = 1;
    mgl_grahics_next_frame();
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
