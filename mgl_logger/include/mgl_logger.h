#ifndef __MGL_LOGGER_H__
#define __MGL_LOGGER_H__
/**
 * mgl_types
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
#include <mgl_types.h>

#define GOTHERE printf("GOT HERE: %s:%s():line:%i\n",__FILE__,__FUNCTION__,__LINE__)

typedef enum _mgl_log_levels {
  MGL_LOG_NONE  = 0,
  MGL_LOG_FATAL = 1,
  MGL_LOG_ERROR = 2,
  MGL_LOG_WARN  = 4,
  MGL_LOG_TRACE = 8,
  MGL_LOG_DEBUG = 16,
  MGL_LOG_INFO  = 32,
  MGL_LOG_ALL   = 63
}MglLogLevel;

/**
 * @brief initializes logging system
 */
void mgl_logger_init();

/**
 * @brief returns the initialization status of the logger
 *
 * @return MglTrue if mgl_logger is initialized
 */
MglBool mgl_logger_is_initialized();

/**
 * @brief sets how verbose logging will be.
 * 
 * @param level Enable a log level, can be | 'd together
 */
void mgl_logger_set_threshold(MglUint level);

/**
 * @brief enables or disables logging to terminal / console
 * This defaults to true internally.
 * @param enable set to MglTrue to enable it, MglFalse to disable it.
 */
void mgl_logger_set_stdout_echo(MglBool enable);

/**
 * @brief change which file logging is saved to.
 * Will also open the file for logging internally
 * @param filepath the path of the file to save logs to
 */
void mgl_logger_set_log_file(const char *filepath);

/**
 * @brief enable or disable thread based logging.
 * 
 * @param enable MglTrue to enable thread logging, MglFalse to disable it
 */
void mgl_logger_enable_thread_logging(MglBool enable);

/**
 * @brief log a message regardless of warning levels.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_message(...) m_mgl_logger_message(__FILE__,__LINE__,__VA_ARGS__)
void m_mgl_logger_message(char *f,int l,char *msg,...);

/**
 * @brief log a message at the info warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_info(...) _mgl_logger_info(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_info(char *f,int l,char *msg,...);

/**
 * @brief log a message at the trace warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_trace(...) _mgl_logger_trace(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_trace(char *f,int l,char *msg,...);

/**
 * @brief log a message at the "warning" warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_warn(...) _mgl_logger_warn(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_warn(char *f,int l,char *msg,...);

/**
 * @brief log a message at the error warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_error(...) _mgl_logger_error(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_error(char *f,int l,char *msg,...);

/**
 * @brief log a message at the debug warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_debug(...) _mgl_logger_debug(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_debug(char *f,int l,char *msg,...);

/**
 * @brief log a message at the fatal warning level.  It will ignore warning
 * levels below the threshold.
 *
 * @param msg the warning message to log.
 * @param .... this function uses the printf convention
 */
#define mgl_logger_fatal(...) _mgl_logger_fatal(__FILE__,__LINE__,__VA_ARGS__)
void _mgl_logger_fatal(char *f,int l,char *msg,...);

#endif

