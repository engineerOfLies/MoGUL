#include "mgl_logger.h"
#include <string.h>

/**
 * @purpose mgl_logger_test is meant to test the logging system
 */


int main(int argc,char *argv[])
{
  int i;
  mgl_logger_init();
  if ((argc == 2) && (strcmp(argv[1],"-h")==0))
  {
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"%s -f <log file> -l <log level number> -[tc]\n",argv[0]);
    fprintf(stdout,"  t - enable thread based logging\n");
    fprintf(stdout,"  c - enable logging to console / terminal\n");
    return 0;
  }
  fprintf(stdout,"mgl_logger_test begin\n");
  for (i = 1;i < argc;++i)
  {
    if (strcmp(argv[i],"-f")==0)
    {
      fprintf(stdout,"using log file %s\n",argv[++i]);
      mgl_logger_set_log_file(argv[i]);
    }
    else if (strcmp(argv[i],"-l")==0)
    {
      mgl_logger_set_threshold(atoi(argv[++i]));
      fprintf(stdout,"using log level %s\n",argv[i]);
    }
    else if (strcmp(argv[i],"-t")==0)
    {
      fprintf(stdout,"using threaded logger\n");
      mgl_logger_enable_thread_logging(MglTrue);
    }
    else if (strcmp(argv[i],"-c")==0)
    {
      fprintf(stdout,"echoing to console\n");
      mgl_logger_set_stdout_echo(MglTrue);
    }
  }
  mgl_logger_info("This is %s %s level log","an","INFO");
  mgl_logger_trace("This is %s %s level log","a","TRACE");
  mgl_logger_debug("This is %s %s level log","a","DEBUG");
  mgl_logger_warn("This is %s %s level log","a","WARNING");
  mgl_logger_error("This is %s %s level log","an","ERROR");
  mgl_logger_fatal("This is %s %s level log","a","FATAL");
  mgl_logger_message("This is a message without logging level");
  fprintf(stdout,"mgl_logger_test end\n");
}