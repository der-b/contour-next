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
#include <utils/log.h>


/*
 * base64_encode()
 */
int base64_encode(const uint8_t *bytes, const size_t len_bytes, char *base64, size_t len_base64) {
  size_t i, j, rest, padding;
  uint8_t tmp;

  for( tmp = rest = i = j = 0; i < len_base64 && j < len_bytes; ++i ) {
    if( 6 == rest ) {
      base64[i] = _TO_CHAR(0x3f & tmp);
      rest = 0;
      continue;
    }
    base64[i] = _TO_CHAR(0x3f & (tmp<< (6 - rest) | bytes[j] >> (8 - (6 - rest))));
    tmp = bytes[j];
    rest += 2;
    ++j;
  }

  if( j != len_bytes ) {
    return BASE64_BUFFERTOSMALL;
  }

  if( i < len_base64 ) {
    if( 0 != rest ) {
      base64[i] = _TO_CHAR(0x3f & (tmp<< (6 - rest) ));
      ++i;
    }
  } else {
    return BASE64_BUFFERTOSMALL;
  }

  padding = (3 - len_bytes % 3) % 3;

  if( padding + i + 1 /* '\0' */ > len_base64 ) {
    return BASE64_BUFFERTOSMALL;
  }

  if( 0 < padding ) {
    base64[i] = '=';
    ++i;
    if( 1 < padding ) {
      base64[i] = '=';
      ++i;
    }
  }

  base64[i] = 0;

  return BASE64_SUCCESS;
}


/*
 * base64_decode()
 */
int base64_decode(const char *base64, uint8_t *bytes, size_t *len_bytes) {
  size_t i, j, restB, rest64;
  size_t len_b64 = strlen(base64);
  int16_t tmp;
  uint8_t padding = 0;

  if( 0 < len_b64 && '=' == base64[len_b64-1] ) {
    --len_b64;
    ++padding;
    if( 0 < len_b64 && '=' == base64[len_b64-1] ) {
      --len_b64;
      ++padding;
    }
  }

  for( restB = rest64 = j = i = 0; i < *len_bytes && j < len_b64; ++i, ++j ) {
    if( !rest64 ) {
      tmp = _TO_BIN(base64[j]);
      if( 0 > tmp ) {
        return BASE64_WRONGLITERAL;
      }
      bytes[i] = tmp << 2;
      restB = 2;
      ++j;
      if( j >= len_b64 ) {
        continue;
      }
    } else {
      bytes[i] = tmp << (2 + 6 - rest64);
      restB = 8 - rest64;
    }
    tmp = _TO_BIN(base64[j]);
    if( 0 > tmp ) {
      return BASE64_WRONGLITERAL;
    }
    bytes[i] = bytes[i] | (tmp >> (6 - restB));
    rest64 = 6 - restB;
  }

  if( j < len_b64 ) {
    return BASE64_BUFFERTOSMALL;
  }

  if( rest64 != (padding * 2) ) {
    return BASE64_WRONGPADDING;
  }

  *len_bytes = i;

  return BASE64_SUCCESS;
}

