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
#include "utils/urlpharser.h"
#include "utils/log.h"

// possible flags: REG_EXTENDED, REG_ICASE, REG_NOSUB, REG_NEWLINE
#define COMPTYPE (REG_EXTENDED)

// possible flags: REG_NOTBOL, REG_NOTEOL
#define EFLAGS 0

#define ERRBUFSIZE 1024
#define PMAXMATCH 255

#define _URLPHARSER_ISABSOLUTE (2)
#define _URLPHARSER_ABS_SCHEME (3)
#define _URLPHARSER_ABS_HOST (19)
#define _URLPHARSER_ABS_PORT (31)
#define _URLPHARSER_ABS_PATH (37)

#define _URLPHARSER_REL_PATH (115)

/*
 * urlpharser_init()
 */
int urlpharser_init(URLPharser *urlp) {
  int ret;
  char errbuf[ERRBUFSIZE];

  ret = regcomp(&urlp->preg, URLPHARSER_URI_REFERENCE, COMPTYPE);
  if( ret ) { 
    regerror(ret, &urlp->preg, errbuf, ERRBUFSIZE);
    ERROR("regcomp() faild (%s)\n", errbuf);
    return URLPHARSER_ERR;
  }

  return 0;
}

/*
 * urlpharser_url()
 */
URLPharserRes urlpharser_url(URLPharser *urlp, const char * url) {
  int ret;
  char errbuf[ERRBUFSIZE];
  regmatch_t match[PMAXMATCH];
  URLPharserRes res;

  memset(&res, 0, sizeof(URLPharserRes));

  ret = regexec(&urlp->preg, url, PMAXMATCH, match, EFLAGS);
  if( ret ) { 
    regerror(ret, &urlp->preg, errbuf, ERRBUFSIZE);
#ifndef NDEBUG
    fprintf(stderr, "regexec() faild (%s)\n", errbuf);
#endif
    DEBUG("Regex dosn't matched.");
    res.res = URLPHARSER_ERR;
  } else {
    res.res = URLPHARSER_SUCCESS;
    
    if( 0 <= match[_URLPHARSER_ISABSOLUTE].rm_so ) {
      res.isAbsolute = 1;

      res.scheme.start = match[_URLPHARSER_ABS_SCHEME].rm_so;
      res.scheme.end   = match[_URLPHARSER_ABS_SCHEME].rm_eo;

      res.host.start = match[_URLPHARSER_ABS_HOST].rm_so;
      res.host.end   = match[_URLPHARSER_ABS_HOST].rm_eo;

      res.port.start = match[_URLPHARSER_ABS_PORT].rm_so;
      res.port.end   = match[_URLPHARSER_ABS_PORT].rm_eo;

      res.path.start = match[_URLPHARSER_ABS_PATH].rm_so;
      res.path.end   = match[_URLPHARSER_ABS_PATH].rm_eo;

    } else {
      res.isAbsolute = 0;

      res.scheme.start = -1;
      res.host.start = -1;
      res.port.start = -1;
      res.path.start = match[_URLPHARSER_REL_PATH].rm_so;
      res.path.end   = match[_URLPHARSER_REL_PATH].rm_eo;
    }
#ifndef NDEBUG
    int j;
    char *helpbuf;
    printf("matches:\n");
    //for( j = 0; j < MAXMATCH && -1 != match[j].rm_so; j++ ) {
    for( j = 0; j < PMAXMATCH; j++ ) {
      if( 0 <= match[j].rm_so ) {
        helpbuf = malloc((match[j].rm_eo - match[j].rm_so + 1) * sizeof(char));
        if( !helpbuf ) {
          fprintf(stderr, "Not enough memory.");
          res.res = 1;
          return res;
        }
	helpbuf[match[j].rm_eo - match[j].rm_so] = 0;
        strncpy(helpbuf, &url[match[j].rm_so], match[j].rm_eo - match[j].rm_so);
        printf(" (%d) %s \n", j, helpbuf);
        free(helpbuf);
      }
    }
#endif
  }


  return res;
}


/*
 * urlpharser_free()
 */
int urlpharser_free(URLPharser *urlp) {
  regfree(&urlp->preg);

  return 0;
}

