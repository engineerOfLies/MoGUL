#include "mgl_config.h"
#include "mgl_dict.h"
#include <string.h>

/**
 * @purpose mgl_resource_test is meant to test the abstract resource manager system
 */

int main(int argc,char *argv[])
{
  int i;
  char *configfilename = NULL;
  MglConfig *config = NULL;
  if ((argc == 2) && (strcmp(argv[1],"-h")==0))
  {
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"%s [config file]\n",argv[0]);
    return 0;
  }
  fprintf(stdout,"mgl_resource_test begin\n");
  fprintf(stdout,"freeing resource manager...\n");
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
  
  fprintf(stdout,"\nmgl_resource_test end\n");
}