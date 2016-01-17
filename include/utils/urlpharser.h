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
#ifndef __urlpharser_h__
#define __urlpharser_h__

#include <sys/types.h>
#include <regex.h>

#define URLPHARSER_LOWALPHA      "[[:lower:]]"
#define URLPHARSER_UPALPHA       "[[:upper:]]"
#define URLPHARSER_ALPHA         "[[:alpha:]]"
#define URLPHARSER_DIGIT         "[[:digit:]]"
#define URLPHARSER_ALPHANUM      "[[:alnum:]]"
#define URLPHARSER_MARK          "[-_\\.!~\\*'\\(\\)]"
#define URLPHARSER_RESERVED      "[;/\\?:@&=\\+\\$,]"
#define URLPHARSER_UNRESERVED    "(" URLPHARSER_ALPHANUM "|" URLPHARSER_MARK ")"
#define URLPHARSER_HEX           "(" URLPHARSER_DIGIT "|[A-Fa-f])"
#define URLPHARSER_ESCAPED       "(%%" URLPHARSER_HEX URLPHARSER_HEX ")"
#define URLPHARSER_URIC          "(" URLPHARSER_RESERVED "|" URLPHARSER_UNRESERVED "|" URLPHARSER_ESCAPED ")"
#define URLPHARSER_SCHEME        "(" URLPHARSER_ALPHA "(" URLPHARSER_ALPHANUM "|[\\+\\.-])*)"
#define URLPHARSER_PCHAR         "(" URLPHARSER_UNRESERVED "|" URLPHARSER_ESCAPED "|[:@&=\\+\\$,])"
#define URLPHARSER_PARAM         "(" URLPHARSER_PCHAR"*)"
#define URLPHARSER_SEGMENT       "(" URLPHARSER_PCHAR "*(;|" URLPHARSER_PARAM ")*)"
#define URLPHARSER_PATH_SEGMENTS "(" URLPHARSER_SEGMENT "(/" URLPHARSER_SEGMENT ")*)"
#define URLPHARSER_URIC_NO_SLASH "(" URLPHARSER_UNRESERVED "|" URLPHARSER_ESCAPED "|[;\\?:@&=\\+\\$,])"
#define URLPHARSER_OPAQUE_PART   "(" URLPHARSER_URIC_NO_SLASH URLPHARSER_URIC "*)"
#define URLPHARSER_PATH          "(" URLPHARSER_ABS_PATH "|" URLPHARSER_OPAQUE_PART ")"
#define URLPHARSER_ABS_PATH      "(/" URLPHARSER_PATH_SEGMENTS ")"
#define URLPHARSER_DOMAINLABEL   "(" URLPHARSER_ALPHANUM "|(" URLPHARSER_ALPHANUM "(" URLPHARSER_ALPHANUM "|-)*" URLPHARSER_ALPHANUM "))"
#define URLPHARSER_TOPLABEL      "(" URLPHARSER_ALPHA "|(" URLPHARSER_ALPHA "(" URLPHARSER_ALPHANUM "|-)*" URLPHARSER_ALPHANUM  "))"
#define URLPHARSER_HOSTNAME      "((" URLPHARSER_DOMAINLABEL "\\.)*" URLPHARSER_TOPLABEL "(\\.)?)"
#define URLPHARSER_IPV4ADDRESS   "(" URLPHARSER_DIGIT "*\\." URLPHARSER_DIGIT "*\\." URLPHARSER_DIGIT "*\\." URLPHARSER_DIGIT "*)"
#define URLPHARSER_HOST          "(" URLPHARSER_HOSTNAME "|" URLPHARSER_IPV4ADDRESS ")"
#define URLPHARSER_PORT          "("URLPHARSER_DIGIT "*)"
#define URLPHARSER_HOSTPORT      "(" URLPHARSER_HOST "(:" URLPHARSER_PORT ")?)"
#define URLPHARSER_USERINFO      "((" URLPHARSER_UNRESERVED "|" URLPHARSER_ESCAPED "|[;:&=\\+\\$,])*)" // TODO: check whether "*" makes sense
#define URLPHARSER_SERVER        "((" URLPHARSER_USERINFO "@)?" URLPHARSER_HOSTPORT ")"
#define URLPHARSER_REG_NAME      "((" URLPHARSER_RESERVED "|" URLPHARSER_ESCAPED "|[;:@&=\\+\\$,])+)" // TODO: check whether "1*" in rfc2396 can be translated to "+"
#define URLPHARSER_AUTHORITY     "(" URLPHARSER_SERVER "|" URLPHARSER_REG_NAME ")"
#define URLPHARSER_NET_PATH      "(//" URLPHARSER_AUTHORITY "(" URLPHARSER_ABS_PATH "))"
#define URLPHARSER_QUERY         "(" URLPHARSER_URIC "*)"
#define URLPHARSER_HIER_PART     "((" URLPHARSER_NET_PATH "|" URLPHARSER_ABS_PATH ")(\\?" URLPHARSER_QUERY ")?)"
#define URLPHARSER_ABSOLUTEURI   "(" URLPHARSER_SCHEME ":(" URLPHARSER_HIER_PART "|" URLPHARSER_OPAQUE_PART "))"
#define URLPHARSER_FRAGMENT      "(" URLPHARSER_URIC "*)"
#define URLPHARSER_REL_SEGMENT   "((" URLPHARSER_UNRESERVED "|" URLPHARSER_ESCAPED "|[;@&=\\+\\$,])+)"
#define URLPHARSER_REL_PATH      "(" URLPHARSER_REL_SEGMENT URLPHARSER_ABS_PATH"+)"
#define URLPHARSER_RELATIVEURI   "((" URLPHARSER_NET_PATH "|" URLPHARSER_ABS_PATH "|" URLPHARSER_REL_PATH ")" URLPHARSER_QUERY "?)"
#define URLPHARSER_URI_REFERENCE "^(" URLPHARSER_ABSOLUTEURI "|" URLPHARSER_RELATIVEURI ")(#" URLPHARSER_FRAGMENT ")?$"

#define URLPHARSER_SUCCESS  ( 0)
#define URLPHARSER_ERR      (-1)

struct urlpharser;
struct urlpharser_result;
typedef struct urlpharser URLPharser;
typedef struct urlpharser_result URLPharserRes;

struct urlpharser {
  regex_t preg;
};

struct urlpharser_result {
  int res;
  int isAbsolute;

  struct {
    ssize_t start;
    ssize_t end;
  } scheme;

  struct {
    ssize_t start;
    ssize_t end;
  } host;

  struct {
    ssize_t start;
    ssize_t end;
  } port;

  struct {
    ssize_t start;
    ssize_t end;
  } path;
};

int urlpharser_init(URLPharser *urlp);
URLPharserRes urlpharser_url(URLPharser *urlp, const char * url);
int urlpharser_free(URLPharser *urlp);

#endif
