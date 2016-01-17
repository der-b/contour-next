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
#ifndef __sprotocol_h__
#define __sprotocol_h__

#include <stdint.h>
#include <sys/types.h>

#define SPROTOCOL_RET_SUCCESS    ( 0)
#define SPROTOCOL_RET_UNKNOWN    (-1)
#define SPROTOCOL_RET_INVALIDARG (-2)
#define SPROTOCOL_RET_INVALIDPKT (-3)

typedef ssize_t SPRet;

struct _sprotocol;
typedef struct _sprotocol SProtocol;

struct _sprotocol_packet;
typedef struct _sprotocol_packet SPPacket;

struct _sprotocol {
};

struct _sprotocol_header {
	uint8_t reserved[3];
	uint8_t size;
} __attribute__((packed));

struct _sprotocol_packet {
	uint8_t size;
	const uint8_t *payload;
};

SPRet sprotocol_init(SProtocol *sp);
SPRet sprotocol_interprete(SProtocol *sp, const uint8_t *buf, size_t len, SPPacket *p);
SPRet sprotocol_free(SProtocol *sp);

#endif
