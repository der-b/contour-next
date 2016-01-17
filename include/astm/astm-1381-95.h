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
#ifndef __ASTM_1381_95_h__
#define __ASTM_1381_95_h__

#include <astm/astm.h>

// definition of the ASTM controll character
// similar to ASCII
#define ASTM_CC_ACK '\x06' // Acknowledge
#define ASTM_CC_CR  '\x0d' // Carriage Returen
#define ASTM_CC_ENQ '\x05' // Enquiry
#define ASTM_CC_EOT '\x04' // End of Transmission
#define ASTM_CC_ETB '\x17' // End of Transmission Block
#define ASTM_CC_ETX '\x03' // End of Text
#define ASTM_CC_LF  '\x0a' // Line Feed
#define ASTM_CC_NAK '\x15' // Negative Acknowledge
#define ASTM_CC_STX '\x02' // Start of Text

/* data intermediate frame
 * <ASTM_CC_STX> fn payload <ASTM_CC_CR> <ASTM_CC_ETB> ch cl <ASTM_CC_CR> <ASTM_CC_LF>
 *
 * data end frame
 * <ASTM_CC_STX> fn payload <ASTM_CC_CR> <ASTM_CC_ETX> ch cl <ASTM_CC_CR> <ASTM_CC_LF>
 *
 * fn = frame number, one byte
 * payload 
 * ch = checksum higher byte
 * cl = checksum lower byte
 *
 * the maximum payload lenght is unknown
 */

struct _astm_1381_95_package;
struct _astm_1381_95;
typedef struct _astm_1381_95 ASTM_1381_95;
typedef struct _astm_1381_95_packet ASTM_1381_95_Packet;

enum _astm_1381_95_frametype{
	ASTM_FRAME_UNKNOWN,
	ASTM_FRAME_TEXT,
	ASTM_FRAME_EOT,
	ASTM_FRAME_ACK,
	ASTM_FRAME_NACK,
	ASTM_FRAME_ENQ
};

struct _astm_1381_95 {
};

struct _astm_1381_95_packet {
	enum _astm_1381_95_frametype type;
	size_t frame_len;

	/* The following values are only set for ASTM_FRAME_TEXT,
	 * otherwhise the values are undefined.
	 */
	struct {
		uint8_t frame_number;
		uint8_t is_end_frame;
		uint8_t checksum;
		const uint8_t *payload;
		size_t size;
	} text;
};

uint8_t astm1381_95_checksum(const uint8_t *buf, size_t length);
ASTMRet astm1381_95_init(ASTM_1381_95 *astm);
ASTMRet astm1381_95_interprete(ASTM_1381_95 *astm, const uint8_t *buf, size_t len, ASTM_1381_95_Packet *p);
ASTMRet _astm1381_95_textframe(ASTM_1381_95 *astm, const uint8_t *buf, size_t len, ASTM_1381_95_Packet *p);
ASTMRet astm1381_95_free(ASTM_1381_95 *astm);

#endif
