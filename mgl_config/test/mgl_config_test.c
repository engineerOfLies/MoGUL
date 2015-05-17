#include "mgl_config.h"
#include "mgl_dict.h"
#include "mgl_logger.h"
#include <string.h>

/**
 * @purpose mgl_resource_test is meant to test the abstract resource manager system
 */

void init_all();

int main(int argc,char *argv[])
{
  char *configfilename = NULL;
  init_all();
  MglConfig *config = NULL;
  if (((argc == 2) && (strcmp(argv[1],"-h")==0))||(argc < 2))
  {
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"%s [config file]\n",argv[0]);
    return 0;
  }
  mgl_logger_info("mgl_config_test begin\n");
  
  configfilename =  argv[1];
  
  mgl_config_init();
  
  config = mgl_config_load(configfilename);
  if (!config)
  {
    fprintf(stdout,"failed to load config file %s:\n",configfilename);
    return 0;
  }
  fprintf(stdout,"printing loaded dictionary:\n");
  mgl_dict_print(mgl_config_get_dictionary(config));
  
  fprintf(stdout,"\nmgl_config_test end\n");

}


void init_all()
{
  mgl_logger_init();
  mgl_logger_set_stdout_echo(MglTrue);
  mgl_logger_set_threshold(MGL_LOG_INFO);
}

/*eol@eof*/
