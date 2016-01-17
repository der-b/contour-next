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
#include <astm/astm-1381-95.h>
#include <utils/log.h>

/*
 * hexdigit2int()
 */
uint8_t hexdigit2int(uint8_t c)
{
	if ('0' <= c && '9' >= c) {
		return c - '0';
	} else if ('A' <= c && 'F' >= c) {
		return c - 'A' + 10;
	} else {
		return c - 'a' + 10;
	}
}

/*
 * astm_checksum()
 */
uint8_t astm1381_95_checksum(const uint8_t *buf, size_t length)
{
	size_t i;
	uint8_t sum;

	for (i = sum = 0; i < length; ++i) {
		sum += buf[i];
	}

	return sum;
}


/*
 * _astm1381_95_textframe()
 */
ASTMRet _astm1381_95_textframe(ASTM_1381_95 *astm, const uint8_t *buf, size_t len, ASTM_1381_95_Packet *p)
{
	ASTMRet ret = ASTM_RET_SUCCESS;
	uint8_t csum;

	// check first byte
	if (ASTM_CC_STX != buf[0]) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err;
	}

	// check frame number
	p->text.frame_number = buf[1] - '0';
	if (7 < p->text.frame_number) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_fn;
	}

#warning "Potentional dangerous assumption: Assumes that the caller sends only buffer where the end of the buffer matches with the frame end."
	// check last byte
	if (ASTM_CC_LF != buf[len - 1]) {
		ret = ASTM_RET_ENDNOTFOUND;
		goto err_fn;
	}

	// check second last byte
	if (ASTM_CC_CR != buf[len - 2]) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_fn;
	}

	// check checksum low
	if (!(   ('0' <= buf[len - 3] && '9' >= buf[len - 3]) 
	      || ('A' <= buf[len - 3] && 'F' >= buf[len - 3]) 
	      || ('a' <= buf[len - 3] && 'f' >= buf[len - 3])
	     )) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_fn;
	}

	// check checksum high
	if (!(   ('0' <= buf[len - 4] && '9' >= buf[len - 4]) 
	      || ('A' <= buf[len - 4] && 'F' >= buf[len - 4]) 
	      || ('a' <= buf[len - 4] && 'f' >= buf[len - 4])
	     )) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_fn;
	}


	// pharse checksum
	p->text.checksum = (hexdigit2int(buf[len - 4]) << 4) | hexdigit2int(buf[len - 3]);
	
	// check end of frame flag
	if (ASTM_CC_ETB != buf[len - 5] && ASTM_CC_ETX != buf[len - 5]) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_cs;
	}

	p->text.is_end_frame = ASTM_CC_ETX == buf[len - 5];

	// check first byte after payload
	if (ASTM_CC_CR != buf[len - 6]) {
		ERROR("Error while pharseing astm 1381-95 frame.");
		ret = ASTM_RET_UNKNOWN;
		goto err_endframe;
	}

	p->text.payload = buf + 2;
	p->text.size = len - 8;

	// checksum
	csum = astm1381_95_checksum(buf + 1, len - 5);
	if ( csum != p->text.checksum) {
		ERROR("Error while pharseing astm 1381-95 frame. (checksum is %.2x but shoud be %.2x)", csum, p->text.checksum);
		ret = ASTM_RET_UNKNOWN;
		goto err_endframe;
	}

	p->type = ASTM_FRAME_TEXT;
	p->frame_len = len;

	return ASTM_RET_SUCCESS;

err_endframe:
	p->text.is_end_frame = 0;
err_cs:
	p->text.checksum = 0;
err_fn:
	p->text.frame_number = 0;
err:
	return ret;
}


/*
 * astm1381_95_interprete()
 */
ASTMRet astm1381_95_interprete(ASTM_1381_95 *astm, const uint8_t *buf, size_t len, ASTM_1381_95_Packet *p)
{
	ASTMRet ret = ASTM_RET_SUCCESS;

	if (!astm || !p) {
		ERROR("Invalid arg.");
		ret = ASTM_RET_UNKNOWN;
		goto err;
	}

	if (1 > len) {
		ERROR("buffer to small");
		ret = ASTM_RET_UNKNOWN;
	}


	p->type = ASTM_FRAME_UNKNOWN;
	p->frame_len = 0;

	switch (*buf) {
	case ASTM_CC_ACK: 
		p->frame_len = 1;
		p->type = ASTM_FRAME_ACK;
		break;
	case ASTM_CC_ENQ:
		p->frame_len = 1;
		p->type = ASTM_FRAME_ENQ;
		break;
	case ASTM_CC_EOT:
		p->frame_len = 1;
		p->type = ASTM_FRAME_EOT;
		break;
	case ASTM_CC_NAK:
		p->frame_len = 1;
		p->type = ASTM_FRAME_NACK;
		break;
	case ASTM_CC_STX:
		ret = _astm1381_95_textframe(astm, buf, len, p);
		break;
	default:
		ERROR("Frame type not recognized.");
		ret = ASTM_RET_UNKNOWN;
	}

	return ret;

err:
	return ret;
	
}


/*
 * astm_init()
 */
ASTMRet astm1381_95_init(ASTM_1381_95 *astm)
{
	memset(astm, 0, sizeof(*astm));
	return ASTM_RET_SUCCESS;
}


/*
 * astm_free()
 */
ASTMRet astm1381_95_free(ASTM_1381_95 *astm)
{
	return ASTM_RET_SUCCESS;
}
