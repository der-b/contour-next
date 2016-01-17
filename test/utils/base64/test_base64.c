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
#include <utils/base64.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <utils/log.h>
#include "test_header.h"

#define BUF_SIZE   (1024)
#define BUF_SIZE64 ((int)(1024 * (3.0/2.0)))

int main(int argc, char **argv) {
  uint8_t orig[BUF_SIZE];
  char base64[BUF_SIZE64];
  uint8_t recovered[BUF_SIZE];
  size_t len;
  int i, padding;

  srand(time(NULL));

  memset(orig, 0, BUF_SIZE);
  memset(base64, 0, BUF_SIZE);
  memset(recovered, 0, BUF_SIZE);

  for(i = 0; i < 64; ++i) {
    if( i != _TO_BIN(_TO_CHAR(i)) ) {
      ERROR("Macro test faild: _TO_BIN(_TO_CHAR(%d))", i);
      return UT_FAIL;
    }
  }

  for( i = 0; i < 256; ++i ) {
    if(    i != '+' 
        && i != '/' 
	&& !(i >= '0' && i <= '9') 
	&& !(i >= 'a' && i <= 'z')
	&& !(i >= 'A' && i <= 'Z') ) {

      if( 0 <= _TO_BIN(i) ) {
        ERROR("Macro test faild: _TO_BIN(%c) = %d", i, _TO_BIN(i));
        return UT_FAIL;
      }
    }
  }

  for( i = 0; i < BUF_SIZE; ++i ) {
    orig[i] = rand() % 256;
  }
  
  for( i = 0; BUF_SIZE > i; ++i ) {
    if( base64_encode(orig, BUF_SIZE - i, base64, BUF_SIZE64) ) {
      ERROR("base64_encode() faild.");
      return UT_FAIL;
    }

    len = BUF_SIZE;
    if( base64_decode(base64, recovered, &len) ) {
      ERROR("base64_encode() faild.");
      return UT_FAIL;
    }

    if( BUF_SIZE - i != len ) {
      ERROR("returnd len is wrong");
    }

    padding = (3 - (BUF_SIZE -i) % 3) % 3;

    if( 0 < padding ) {
      if( '=' != base64[strlen(base64) - 1] ) {
        ERROR("Wrong padding. %c", base64[strlen(base64) - 1]);
	return UT_FAIL;
      }
      if( 1 < padding ) {
        if( '=' != base64[strlen(base64) - 2] ) {
          ERROR("Wrong padding. %c", base64[strlen(base64) - 2]);
	  return UT_FAIL;
        }
      }
    }

    if( 0 != memcmp(orig, recovered, BUF_SIZE - i) ) {
      ERROR("orig != recovered");
      return UT_FAIL;
    }
  }
  
  return UT_SUCCESS;
}

