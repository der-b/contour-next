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
#include "test_header.h"
#include <utils/urlpharser.h>
#include <utils/log.h>
#include <regex.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <utils/log.h>

#define ERRBUFSIZE (1024)
#define MAXMATCH (10)

#define REPORT_ON_SUCCESS (0)
#define REPORT_ON_FAILURE (1)

#define LOWALPHA "abcdefghijklmnopqrstuvwxyz"
#define UPALPHA  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM      "0123456789"
#define MARK     "-_.!~*'()"
#define RESERVED ";/?:@&=+$,"
#define MARK_RESERVED (MARK RESERVED)
#define UNRESEVED (LOWALPHA UPALPHA NUM MARK)

int regex_helper(const char *regex, char *testdata, const int report);
int check_regex();

int main(void) {
  int ret;
  URLPharser urlp;
  URLPharserRes urlpres;

  ret = check_regex();
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  if( urlpharser_init(&urlp) ) {
    ERROR("urlpharser_init() faild.");
    return UT_FAIL;
  }

  #define URL ("http://miranda@www.der-b.com:8080/path/to/file/test.html?abc=def")
  urlpres = urlpharser_url(&urlp, URL);
  if( urlpres.res ) {
    ERROR("urlpharser_url() faild.");
    return UT_FAIL;
  }

  if( !urlpres.isAbsolute ) {
    ERROR("Absolute URL was not recognized as absolut URL.");
    return UT_FAIL;
  }

  if( -1 == urlpres.scheme.start || 0 != strncmp("http", &URL[urlpres.scheme.start], urlpres.scheme.end - urlpres.scheme.start) ) {
    ERROR("SCHEME was wrong pharsed.");
    return UT_FAIL;
  }

  if( -1 == urlpres.host.start || 0 != strncmp("www.der-b.com", &URL[urlpres.host.start], urlpres.host.end - urlpres.host.start) ) {
    ERROR("HOST was wrong pharsed.");
    return UT_FAIL;
  }
  
  if( -1 == urlpres.port.start || 0 != strncmp("8080", &URL[urlpres.port.start], urlpres.port.end - urlpres.port.start) ) {
    ERROR("PORT was wrong pharsed.");
    return UT_FAIL;
  }

  if( -1 == urlpres.path.start || 0 != strncmp("/path/to/file/test.html", &URL[urlpres.path.start], urlpres.path.end - urlpres.path.start) ) {
    ERROR("PATH was wrong pharsed.");
    return UT_FAIL;
  }

  #undef URL

  #define URL ("../path/to/file/test.html?abc=def")
  urlpres = urlpharser_url(&urlp, URL);
  if( urlpres.res ) {
    ERROR("urlpharser_url() faild.");
    return UT_FAIL;
  }

  if( urlpres.isAbsolute ) {
    ERROR("relative URL was not recognized as relative URL.");
    return UT_FAIL;
  }

  if( -1 != urlpres.scheme.start ) {
    ERROR("SCHEME recognized but url has no sheme.");
    return UT_FAIL;
  }

  if( -1 != urlpres.host.start ) {
    ERROR("HOST recognized but url has no host");
    return UT_FAIL;
  }
  
  if( -1 != urlpres.port.start ) {
    ERROR("PORT recognized but url has no port.");
    return UT_FAIL;
  }

  if( -1 == urlpres.path.start || 0 != strncmp("../path/to/file/test.html", &URL[urlpres.path.start], urlpres.path.end - urlpres.path.start) ) {
    ERROR("PATH was wrong pharsed.");
    return UT_FAIL;
  }

  #undef URL

  if( urlpharser_free(&urlp) ) {
    ERROR("urlpharser_free() faild.");
    return UT_FAIL;
  }

  return UT_SUCCESS;
}


/**
 * check_regex()
 * checks regex expressions, which are subparts of the regx for the 
 * uri pharser
 */
int check_regex() {
  int ret, i;
  char buf[1024];

  // check URLPHARSER_LOWALPHA
  buf[1] = 0;
  ret = regex_helper(URLPHARSER_LOWALPHA"*", LOWALPHA, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( buf[0] = 'A'; buf[0] <= 'Z'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_LOWALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( buf[0] = '0'; buf[0] <= '9'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_LOWALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_LOWALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_UPALPHA
  ret = regex_helper(URLPHARSER_UPALPHA"*", UPALPHA, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( buf[0] = 'a'; buf[0] <= 'z'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_UPALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( buf[0] = '0'; buf[0] <= '9'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_UPALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_UPALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_ALPHA
  ret = regex_helper(URLPHARSER_ALPHA"*", UPALPHA LOWALPHA, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( buf[0] = '0'; buf[0] <= '9'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_ALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_ALPHA, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_DIGIT
  ret = regex_helper(URLPHARSER_DIGIT"*", NUM, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( buf[0] = 'a'; buf[0] <= 'z'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_DIGIT, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( buf[0] = 'A'; buf[0] <= 'Z'; buf[0]++ ) {
    ret = regex_helper(URLPHARSER_DIGIT, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_DIGIT, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_ALPHANUM
  ret = regex_helper(URLPHARSER_ALPHANUM"*", LOWALPHA UPALPHA NUM, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_ALPHANUM, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_MARK
  ret = regex_helper(URLPHARSER_MARK"*", MARK, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( i = 0; i < strlen(RESERVED); i++) {
    buf[0] = RESERVED[i];
    ret = regex_helper(URLPHARSER_MARK, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_RESERVED
  ret = regex_helper(URLPHARSER_RESERVED "*", RESERVED, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK); i++) {
    buf[0] = MARK[i];
    ret = regex_helper(URLPHARSER_RESERVED, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_HEX
  ret = regex_helper(URLPHARSER_HEX"*", NUM "abcdefABCDEF", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  buf[1] = 0;
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_HEX, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  // check URLPHARSER_ESCAPED
  ret = regex_helper(URLPHARSER_ESCAPED, "%%1f", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ESCAPED, "%%a5", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ESCAPED, "a5", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  ret = regex_helper(URLPHARSER_ESCAPED, "%%a5d", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  ret = regex_helper(URLPHARSER_ESCAPED, "%%4g", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_URIC
  ret = regex_helper(URLPHARSER_URIC"*", NUM LOWALPHA "%%4a" UPALPHA MARK RESERVED, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  // check URLPHARSER_SCHEME
  ret = regex_helper(URLPHARSER_SCHEME, "http", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_SCHEME, "https", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_SCHEME, "ht-t.p+s", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_SCHEME, "h012t-t.p+s", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  strcpy(buf, " https");
  for( i = 0; i < strlen(MARK_RESERVED); i++) {
    buf[0] = MARK_RESERVED[i];
    ret = regex_helper(URLPHARSER_SCHEME, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }

  strcpy(buf, " https");
  for( i = 0; i < strlen(NUM); i++) {
    buf[0] = NUM[i];
    ret = regex_helper(URLPHARSER_SCHEME, buf, REPORT_ON_SUCCESS);
    if( UT_SUCCESS == ret ) {
      return UT_FAIL;
    }
  }
  
  // check URLPHARSER_PCHAR
  ret = regex_helper(URLPHARSER_PCHAR"*", NUM LOWALPHA "%%4a" UPALPHA MARK, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_PCHAR"*", NUM LOWALPHA "%%4a" UPALPHA MARK RESERVED, REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_PARAM
  ret = regex_helper(URLPHARSER_PARAM, NUM LOWALPHA "%%4a" UPALPHA MARK, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_PARAM, NUM LOWALPHA "%%4a" UPALPHA MARK RESERVED, REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_SEGMENT
  ret = regex_helper(URLPHARSER_SEGMENT, LOWALPHA NUM "%%4a" UPALPHA MARK, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_SEGMENT, ";" NUM LOWALPHA "%%4a" UPALPHA MARK RESERVED, REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_PATH_SEGMENTS
  ret = regex_helper(URLPHARSER_PATH_SEGMENTS, "This/is/a/Path", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_PATH_SEGMENTS, "/This/is/a/Path", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_PATH_SEGMENTS, ";/This/is/a/Path", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_PATH_SEGMENTS, "/This/is/a/Path?", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_URIC_NO_SLASH
  // TODO: Hier weiter

  // check URLPHARSER_ABSOLUTEURI
  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "http://der-b.com", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "https://der-b.com/test/path/index.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "https://der-b.com/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "https://127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "https://miranda@127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_ABSOLUTEURI, "://miranda@127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_FRAGMENT
  ret = regex_helper(URLPHARSER_FRAGMENT, NUM LOWALPHA "%%4a" UPALPHA MARK RESERVED, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  // check URLPHARSER_REL_SEGMENT
  ret = regex_helper(URLPHARSER_REL_SEGMENT, NUM LOWALPHA "%%4a" UPALPHA MARK, REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_REL_SEGMENT, "", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARER_REL_PATH
  ret = regex_helper(URLPHARSER_REL_PATH, "../test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_REL_PATH, "/test.html", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  // check URLPHARSER_RELATIVEURI
  ret = regex_helper(URLPHARSER_RELATIVEURI, "../test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_RELATIVEURI, "/test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_RELATIVEURI, "//test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_RELATIVEURI, "//test/bla.html?abc=def", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_RELATIVEURI, "//test/bla.html?abc=def&xyz=miau", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  // check URLPHARSER_URI_REFERENCE
  ret = regex_helper(URLPHARSER_URI_REFERENCE, "../test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "/test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "//test/bla.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "//test/bla.html#wooohooooo", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "//test/bla.html?abc=def", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "//test/bla.html?abc=def&xyz=miau", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "http://der-b.com", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://der-b.com/test/path/index.html", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://der-b.com/test/path/index.html#miau", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://der-b.com/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://miranda@127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "https://miranda@127.0.0.1:2356/test/path/index.html?abcdef=12353#test", REPORT_ON_FAILURE);
  if( UT_SUCCESS != ret ) {
    return ret;
  }

  ret = regex_helper(URLPHARSER_URI_REFERENCE, "://miranda@127.0.0.1:2356/test/path/index.html?abcdef=12353", REPORT_ON_SUCCESS);
  if( UT_SUCCESS == ret ) {
    return UT_FAIL;
  }

  return UT_SUCCESS;
}


/*
 * regex_helper()
 */
int regex_helper(const char *regex, char *testdata, const int report) {
  regex_t preg;
  char errbuf[ERRBUFSIZE];
  regmatch_t match[MAXMATCH];
  int ret;
  char *helpbuf;

  ret = regcomp(&preg, regex, REG_EXTENDED);
  if( ret ) { 
    regerror(ret, &preg, errbuf, ERRBUFSIZE);
    fprintf(stderr, "regcomp() faild (%s, \"%s\")\n", errbuf, regex);
    regfree(&preg);
    return UT_FAIL;
  }

  ret = regexec(&preg, testdata, MAXMATCH, match, 0);

  if( ret ) { 
    if( REPORT_ON_FAILURE == report ) {
      regerror(ret, &preg, errbuf, ERRBUFSIZE);
      fprintf(stderr, "regex \"%s\" faild on testdata \"%s\": no match.\n", regex, testdata);
    }
    regfree(&preg);
    return UT_FAIL;

  } else if( 0 != match[0].rm_so || strlen(testdata) != match[0].rm_eo ) {
    //fprintf(stderr, "so %d, eo %d, strlend %lu\n", match[0].rm_so, match[0].rm_eo, strlen(testdata));
    if( REPORT_ON_FAILURE == report ) {
      fprintf(stderr, "regex \"%s\" faild on testdata \"%s\": wrong match: ", regex, testdata);
    }

    if( -1 != match[0].rm_so ) {
      if( REPORT_ON_FAILURE == report ) {
        helpbuf = malloc((match[0].rm_eo - match[0].rm_so + 1) * sizeof(char));
        if( !helpbuf ) {
          fprintf(stderr, "Not enough memory.");
          regfree(&preg);
          return UT_FAIL;
        }
	helpbuf[match[0].rm_eo - match[0].rm_so] = 0;
        strncpy(helpbuf, &testdata[match[0].rm_so], match[0].rm_eo - match[0].rm_so);
        fprintf(stderr, "%s\n", helpbuf);
        free(helpbuf);
      }
    } else {
      if( REPORT_ON_FAILURE == report ) {
        fprintf(stderr, "match[0].rm_so is %d\n", match[0].rm_so);
      }
    }

    regfree(&preg);
    return UT_FAIL;
  }

  if( REPORT_ON_SUCCESS == report ) {
    fprintf(stderr, "regex \"%s\" was successfully matcht on testdata \"%s\"", regex, testdata);
  }
  regfree(&preg);
  return UT_SUCCESS;
}

