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
#include <astm/astm-1394-91.h>
#include <string.h>
#include <utils/log.h>

ASTMRet astm1394_91_init(ASTM_1394_91 *astm)
{
	memset(astm, 0, sizeof(*astm));
	return ASTM_RET_SUCCESS;
}

ASTMRet astm1394_91_interprete(ASTM_1394_91 *astm, const uint8_t *buf, size_t len, ASTM_1394_91_Packet *p)
{
	if (!buf) {
		ERROR("Inavalid argument");
		goto err;
	}

	if (1 > len) {
		ERROR("Inavalid argument");
		goto err;
	}

	memset(p, 0, sizeof(*p));

	p->buf = buf;
	p->len = len;

	switch(*buf) {
	case 'H': p->type = HEADER; break;
	case 'P': p->type = PATIENT; break;
	case 'R': p->type = RESULT; break;
	case 'L': p->type = TERMINATOR; break;
	}

	return ASTM_RET_SUCCESS;

err:
	return ASTM_RET_UNKNOWN;
}

ASTMRet astm1394_91_free(ASTM_1394_91 *astm)
{
	return ASTM_RET_SUCCESS;
}

