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
#ifndef __contour_h__
#define __contour_h__

#define CONTOUR_NEXT_VENDOR_ID  (0x1a79) 
#define CONTOUR_NEXT_PRODUCT_ID (0x7410)

#define CONTOUR_HID_DIR "/dev/usb/"
#include <simple_protocol/sprotocol.h>
#include <astm/astm-1381-95.h>

#define CONTOUR_RET_SUCCESS     ( 0)
#define CONTOUR_RET_UNKNOWN     (-1)
#define CONTOUR_RET_ENDNOTFOUND (-2)
#define CONTOUR_RET_ENDOFTRANS  (-3)

typedef ssize_t ContourRet;

struct _contour;
typedef struct _contour Contour;

struct _contour_packet;
typedef struct _contour_packet ContourPacket;

struct _contour {
	int fd;
	int usage_code;
	SProtocol sp;
	ASTM_1381_95 astm_low;
	ASTM_1394_91 astm_high;
};

struct _contour_packet {
	ASTM_1381_95_Packet astm_low;
	ASTM_1394_91_Packet content;
	size_t len;
	uint8_t *buf;
};


ContourRet contour_init(Contour *c);
ContourRet contour_free(Contour *c);
ContourRet _contour_write(Contour *c, const uint8_t *buf, size_t len);
ContourRet _contour_read(Contour *c, uint8_t *buf, size_t len);
ContourRet contour_read(Contour *c, ContourPacket *p);
ContourRet _contour_interprete(Contour *c, ContourPacket *p);
ContourRet contour_packet_init(ContourPacket *p);
ContourRet contour_packet_free(ContourPacket *p);


#endif
