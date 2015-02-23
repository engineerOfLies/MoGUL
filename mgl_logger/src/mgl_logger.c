#include "mgl_logger.h"
#include "mgl_text.h"
#include <stdarg.h>

/*local globals, not be accessed outside of logger*/

static MglBool      _mgl_logger_initialized = MglFalse;
static MglLogLevel  _mgl_log_threshold = MGL_LOG_DEBUG | MGL_LOG_FATAL | MGL_LOG_ERROR | MGL_LOG_WARN;
static MglBool      _mgl_logger_stdout_echo = MglTrue;
static MglLine      _mgl_logger_filename;
static FILE       * _mgl_logger_file = NULL;

/*local functions*/
void mgl_logger_deinit(void);
static void _mgl_logger_message(MglLogLevel level,char *msg,va_list ap);

/*function definitions*/
void mgl_logger_info(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_INFO,
    msg,
    ap);
  va_end(ap);
}

void mgl_logger_trace(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_TRACE,
    msg,
    ap);
  va_end(ap);
}


void mgl_logger_warn(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_WARN,
    msg,
    ap);
  va_end(ap);
}

void mgl_logger_error(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_ERROR,
    msg,
    ap);
  va_end(ap);
}

void mgl_logger_debug(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_DEBUG,
    msg,
    ap);
  va_end(ap);
}

void mgl_logger_fatal(char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  mgl_logger_message(
    MGL_LOG_FATAL,
    msg,
    ap);
  va_end(ap);
}

void mgl_logger_message(MglLogLevel level,char *msg,...)
{
  va_list ap;
  va_start(ap,msg);
  _mgl_logger_message(level,msg,ap);
  va_end(ap);
}

static void _mgl_logger_message(MglLogLevel level,char *msg,va_list ap)
{
  if (!(level & _mgl_log_threshold))
  {
    return;
  }
  if (_mgl_logger_file == NULL)
  {
    fprintf(stderr,"mgl_logger: no log file found! Redirecting logs to stdout\n");
    _mgl_logger_file = stdout;
    return;
  }
  if ((_mgl_logger_stdout_echo == MglTrue) &&
      (_mgl_logger_file != stdout))
  {
    vfprintf(stdout,msg,ap);
    fprintf(stdout,"\n");
  }
  vfprintf(_mgl_logger_file,msg,ap);
  fprintf(_mgl_logger_file,"\n");
}

void mgl_logger_init()
{
  if (strlen(_mgl_logger_filename) <= 0)
  {
  	sprintf(_mgl_logger_filename,"system/engineoflies.log");
  }
  _mgl_logger_file = fopen(_mgl_logger_filename, "w");
  if (_mgl_logger_file == NULL)
  {
    fprintf(stderr,"unable to open log file.");
    return;
  }
  _mgl_logger_initialized = MglTrue;
  atexit(mgl_logger_deinit);
}

void mgl_logger_deinit(void)
{
  if ((_mgl_logger_file != NULL)&&(_mgl_logger_file != stdout))
  {
    fclose(_mgl_logger_file);
    _mgl_logger_file = NULL;
  }
}

void mgl_logger_set_threshold(MglUint level)
{
  _mgl_log_threshold &= level;
}

void mgl_logger_set_log_file(const char *filepath)
{
  FILE *file;
  file = fopen(filepath,"w");
  if (file == NULL)
  {
    fprintf(stdout,"mgl_logger_set_log_file: unable to open file %s for logging!\n",filepath);
    return;
  }
  if ((_mgl_logger_file != NULL) && (_mgl_logger_file != stdout))
  {
    fclose (_mgl_logger_file);
  }
  _mgl_logger_file = file;
  strcpy(_mgl_logger_filename,filepath);
}

void mgl_logger_set_stdout_echo(MglBool enable)
{
  _mgl_logger_stdout_echo = enable;
}
/*Mgl@eof*/

