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
#ifndef __bitswap_h__
#define __bitswap_h__

#include <bits/byteswap.h>

#define ntoh16(x)	ntohs(x)
#define hton16(x)	htons(x)

#define ntoh32(x)	ntohl(x)
#define hton32(x)	htonl(x)

#if __BYTE_ORDER == __BIG_ENDIAN
#  define ntoh64(x)	(x)
#  define hton64(x)	(x)
#else
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#    define ntoh64(x)	__bswap_64(x)
#    define hton64(x)	__bswap_64(x)
#  else
#    error "__BYTE_ORDER not set!"
#  endif
#endif 

#endif /* __bitswap_h__ */
