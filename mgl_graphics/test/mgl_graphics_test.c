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
  MglVec3D bgcolor = {0,0,0};
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

  while (!done)
  {
    bgcolor.x = ((int)bgcolor.x + 1)%256;
    bgcolor.y = ((int)bgcolor.y + 1)%256;
    bgcolor.z = ((int)bgcolor.z + 1)%256;
    mgl_graphics_set_bgcolor(bgcolor);
    mgl_graphics_clear_screen();
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
