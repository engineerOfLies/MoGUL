#include "mgl_logger.h"
#include "mgl_text.h"
#include <SDL_thread.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdarg.h>

typedef struct mglLogEvent_S
{
  char *message;
  MglLogLevel level;
  MglBool close;     /**<set to true to close the logging thread*/
}mglLogEvent;

/*local globals, not be accessed outside of logger*/

static MglBool      _mgl_logger_initialized = MglFalse;
static MglLogLevel  _mgl_log_threshold = MGL_LOG_DEBUG | MGL_LOG_FATAL | MGL_LOG_ERROR | MGL_LOG_WARN;
static MglBool      _mgl_logger_stdout_echo = MglFalse;
static MglLine      _mgl_logger_filename = "logger.log";
static FILE       * _mgl_logger_file = NULL;

static MglBool      _mgl_logger_enable_threading = MglFalse;
static GAsyncQueue *_mgl_logger_message_queue = NULL;
static SDL_Thread * _mgl_logger_thread = NULL;

/*local functions*/
void mgl_logger_deinit(void);
static void _mgl_logger_message(MglLogLevel level,char *msg);
static void _mgl_logger_message_write(MglLogLevel level,char *msg);
static void _mgl_logger_launch_thread();
static int _mgl_logger_thread_function(void *ptr);
static void _mgl_logger_push_message(char *message, MglLogLevel level);
static void _mgl_logger_close_thread();
static void _mgl_logger_open_log_file();

/*function definitions*/

void mgl_logger_init()
{
  if (_mgl_logger_initialized)
  {
    return;
  }
  if (strlen(_mgl_logger_filename) <= 0)
  {
    sprintf(_mgl_logger_filename,"mgl_logger.log");
  }
  _mgl_logger_open_log_file();
  _mgl_logger_initialized = MglTrue;
  atexit(mgl_logger_deinit);
}

void mgl_logger_deinit(void)
{
  if (_mgl_logger_enable_threading)
  {
    _mgl_logger_close_thread();
    if (_mgl_logger_message_queue)
    {
      g_async_queue_unref (_mgl_logger_message_queue);
    }
  }
  else if ((_mgl_logger_file != NULL)&&(_mgl_logger_file != stdout))
  {
    fclose(_mgl_logger_file);
    _mgl_logger_file = NULL;
  }
}

void mgl_logger_info(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("INFO %s",buffer);
  _mgl_logger_message(
    MGL_LOG_INFO,
    message);
  g_free(buffer);
  g_free(message);
}

void mgl_logger_trace(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("TRACE %s",buffer);
  _mgl_logger_message(
    MGL_LOG_TRACE,
    message);
  g_free(buffer);
  g_free(message);
}


void mgl_logger_warn(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("WARN %s",buffer);
  _mgl_logger_message(
    MGL_LOG_WARN,
    message);
  g_free(buffer);
  g_free(message);
}

void mgl_logger_error(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("ERROR %s",buffer);
  _mgl_logger_message(
    MGL_LOG_ERROR,
    message);
  g_free(buffer);
  g_free(message);
  
}

void mgl_logger_debug(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("DEBUG %s",buffer);
  _mgl_logger_message(
    MGL_LOG_DEBUG,
    message);
  g_free(buffer);
  g_free(message);
}

void mgl_logger_fatal(char *msg,...)
{
  char * buffer;
  char * message;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  message = g_strdup_printf("FATAL %s",buffer);
  _mgl_logger_message(
    MGL_LOG_FATAL,
    message);
  g_free(buffer);
  g_free(message);
}

void mgl_logger_message(char *msg,...)
{
  char * buffer;
  va_list ap;
  va_start(ap,msg);
  g_vasprintf (&buffer,msg,ap);
  va_end(ap);
  _mgl_logger_message(MGL_LOG_ALL,buffer);
  g_free(buffer);
}

static void _mgl_logger_message(MglLogLevel level,char *msg)
{
  if (_mgl_logger_enable_threading)
  {
    _mgl_logger_push_message(msg, level);
    return;
  }
  _mgl_logger_message_write(level,msg);
}

static void _mgl_logger_message_write(MglLogLevel level,char *msg)
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
    fprintf(stdout,"%s\n",msg);
  }
  fprintf(_mgl_logger_file,"%s\n",msg);
}

static void _mgl_logger_open_log_file()
{
  _mgl_logger_file = fopen(_mgl_logger_filename, "w");
  if (_mgl_logger_file == NULL)
  {
    fprintf(stderr,"unable to open log file.");
    return;
  }
}

void mgl_logger_set_threshold(MglUint level)
{
  _mgl_log_threshold |= level;
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

void mgl_logger_enable_thread_logging(MglBool enable)
{
  if ((enable) && (!_mgl_logger_enable_threading))
  {
    /*launch thread*/
    _mgl_logger_launch_thread();
  }else if ((!enable) && (_mgl_logger_enable_threading))
  {
    /*close thread*/
    _mgl_logger_close_thread();
    if (_mgl_logger_file == NULL)
    {
      _mgl_logger_open_log_file();
    }
  }
  _mgl_logger_enable_threading = enable;
}

static void _mgl_logger_launch_thread()
{
  if (_mgl_logger_thread  != NULL)
  {
    mgl_logger_error("_mgl_logger_launch_thread: logging thread already active.");
    return;
  }
  /*create message queue*/
  _mgl_logger_message_queue = g_async_queue_new();
  if (_mgl_logger_message_queue == NULL)
  {
    mgl_logger_error("_mgl_logger_launch_thread: unable to create messsage queue.");
    return;
  }
  _mgl_logger_thread = SDL_CreateThread(_mgl_logger_thread_function,"mgl_logger",(void *)NULL);
  SDL_DetachThread(_mgl_logger_thread);
}

static void _mgl_logger_close_thread()
{
  mglLogEvent *closeMessage;
  closeMessage = (mglLogEvent *)malloc(sizeof(mglLogEvent));
  if (!closeMessage)
  {
    mgl_logger_error("_mgl_logger_close_thread: unable to create close message.");
    return;
  }
  memset(closeMessage,0,sizeof(mglLogEvent));
  closeMessage->close = MglTrue;
  g_async_queue_push(_mgl_logger_message_queue,(gpointer)closeMessage);
}

static void _mgl_logger_push_message(char *message, MglLogLevel level)
{
  mglLogEvent *logMessage;
  logMessage = (mglLogEvent *)malloc(sizeof(mglLogEvent));
  if (!logMessage)
  {
    mgl_logger_enable_thread_logging(MglFalse);
    mgl_logger_error("_mgl_logger_close_thread: unable to create log message.");
    return;
  }
  
  logMessage->message = (char *)malloc(sizeof(char)*strlen(message));
  strcpy(logMessage->message,message);
  logMessage->level = level;
  logMessage->close = MglFalse;
  
  g_async_queue_push(_mgl_logger_message_queue,(gpointer)logMessage);
}
static void _mgl_logger_free_message(mglLogEvent *event)
{
  if (!event)
  {
    return;
  }
  if (event->message != NULL)
  {
    free(event->message);
  }
  free(event);
}

static int _mgl_logger_thread_function(void *ptr)
{
  mglLogEvent *logMessage;
  int done = 0;
  _mgl_logger_message_queue = g_async_queue_ref(_mgl_logger_message_queue);
  while(!done)
  {
    logMessage = g_async_queue_pop (_mgl_logger_message_queue);
    if (!logMessage)
    {
      SDL_Delay(5);
      continue;
    }
    if (logMessage->close)
    {
      done = 1;
      break;
    }
    _mgl_logger_message_write(logMessage->level,logMessage->message);
    _mgl_logger_free_message(logMessage);
  }
  /*empty queue*/
  while(logMessage)
  {
    _mgl_logger_message_write(logMessage->level,logMessage->message);
    _mgl_logger_free_message(logMessage);/*write all the logs that are queued up*/
    logMessage = g_async_queue_pop (_mgl_logger_message_queue);
  }
  /*unreference it for cleanup*/
  g_async_queue_unref (_mgl_logger_message_queue);
  if ((_mgl_logger_file != NULL)&&(_mgl_logger_file != stdout))
  {
    fclose(_mgl_logger_file);
    _mgl_logger_file = NULL;
  }
  return 0;
}
/*eol@eof*/

