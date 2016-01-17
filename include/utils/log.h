/* Copyright 2015 Bernd Lehmann (der-b@der-b.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __log_h__
#define __log_h__

#include "utils/log/log_config.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* set std config values */
#ifndef LOG_FD
  #define LOG_FD stderr
#endif


#ifndef LOGLEVEL_INFO
  #define LOGLEVEL_INFO 1
#endif


#ifndef LOGLEVEL_DEBUG
  #define LOGLEVEL_DEBUG 1
#endif


#ifndef LOGLEVEL_WARN
  #define LOGLEVEL_WARN 1
#endif


#ifndef LOGLEVEL_ERROR
  #define LOGLEVEL_ERROR 1
#endif


#ifndef LOGLEVEL_CRIT
  #define LOGLEVEL_CRIT 1
#endif


/* macros */
#if 1 == LOGLEVEL_INFO

  #define INFO(msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:info:   " msg "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__),\
      fflush(NULL)\
    )

  #define INFO_BUF(buf, length, msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:info:   " msg ":",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__),\
      log_buf(buf, length),\
      fprintf(\
        LOG_FD,\
	"\n"),\
      fflush(NULL)\
    )
#else
  #define INFO(msg, ...)
  #define INFO_BUF(buf, length, msg, ...)
#endif


#if 1 == LOGLEVEL_DEBUG

  #define DEBUG(msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:debug:   " msg "%s%s\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      errno = 0,\
      fflush(NULL)\
    )

  #define DEBUG_BUF(buf, length, msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:debug:   " msg "%s%s:",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      log_buf(buf, length),\
      fprintf(\
        LOG_FD,\
	"\n"),\
      errno=0,\
      fflush()\
    )
#else
  #define DEBUG(msg, ...)
  #define DEBUG_BUF(buf, length, msg, ...)
#endif


#if 1 == LOGLEVEL_WARN

  #define WARN(msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:warn:   " msg "%s%s\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      errno = 0,\
      fflush(NULL)\
    )

  #define WARN_BUF(buf, length, msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:warn:   " msg "%s%s:",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      log_buf(buf, length),\
      fprintf(\
        LOG_FD,\
	"\n"),\
      errno = 0,\
      fflush(NULL)\
    )
#else
  #define WARN(msg, ...)
  #define WARN_BUF(buf, length, msg, ...)
#endif


#if 1 == LOGLEVEL_ERROR

  #define ERROR(msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:error:   " msg "%s%s\n",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__, \
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")\
        ),\
        errno = 0\
    )

  #define ERROR_BUF(buf, length, msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:error:   " msg "%s%s:",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      log_buf(buf, length),\
      fprintf(\
        LOG_FD,\
	"\n"),\
      errno=0,\
      fflush(NULL)\
    )
#else
  #define ERROR(msg, ...)
  #define ERROR_BUF(buf, length, msg, ...)
#endif


#if 1 == LOGLEVEL_CRIT

  #define CRIT(msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:crit:   " msg "%s%s\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      fflush(NULL),\
      exit((errno)?(errno):(1))\
    )

  #define CRIT_BUF(buf, length, msg, ...)\
    (\
      fprintf(\
        LOG_FD,\
        "%s:%d:crit:   " msg "%s%s\n",\
        __FILE__,\
        __LINE__,\
	##__VA_ARGS__,\
        (errno)?(":"):(""),\
        (errno)?(strerror(errno)):("")),\
      log_buf(buf, length),\
      fprintf(\
        LOG_FD,\
	"\n"),\
      fflush(NULL),\
      exit((errno)?(errno):(1))\
    )
#else
  #define CRIT(msg, ...) (exit((errno)?(errno):(1)))
  #define CRIT_BUF(buf, length, msg, ...) (exit((errno)?(errno):(1)))
#endif

void log_buf(const void *buf, size_t length);


#endif /* __log_h__ */
