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
#ifndef __ASTM_1394_91_H__
#define __ASTM_1394_91_H__

#include <astm/astm.h>

struct _astm_1394_91_package;
struct _astm_1394_91;
typedef struct _astm_1394_91 ASTM_1394_91;
typedef struct _astm_1394_91_packet ASTM_1394_91_Packet;

enum _astm_1394_91_packet_type {
	UNKNOWN = 0,
	HEADER,
	PATIENT,
	RESULT,
	TERMINATOR
};

struct _astm_1394_91 {
};

struct _astm_1394_91_packet {
	enum _astm_1394_91_packet_type type;
	const uint8_t *buf;
	size_t len;
};

ASTMRet astm1394_91_init(ASTM_1394_91 *astm);
ASTMRet astm1394_91_interprete(ASTM_1394_91 *astm, const uint8_t *buf, size_t len, ASTM_1394_91_Packet *p);
ASTMRet astm1394_91_free(ASTM_1394_91 *astm);

#endif
