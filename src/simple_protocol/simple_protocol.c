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
#include <simple_protocol/sprotocol.h>
#include <utils/log.h>

/*
 * sprotocol_interprete()
 */
SPRet sprotocol_interprete(SProtocol *sp, const uint8_t *buf, size_t len, SPPacket *p)
{
	SPRet ret = SPROTOCOL_RET_SUCCESS;
	struct _sprotocol_header *h;

	if (!buf || !p) {
		ERROR("Invalid function argument.");
		ret = SPROTOCOL_RET_INVALIDARG;
		goto err;
	}

	if (sizeof(struct _sprotocol_header) > len) {
		ERROR("Invalid packet.");
		ret = SPROTOCOL_RET_INVALIDPKT;
		goto err;
	}

	h = (struct _sprotocol_header *)buf;
	p->size = h->size;
	p->payload = buf + sizeof(struct _sprotocol_header);

	return ret;

err:
	return ret;
}


/*
 * sprotocol_init()
 */
SPRet sprotocol_init(SProtocol *sp)
{
	return SPROTOCOL_RET_SUCCESS;
}


/*
 * sprotocol_free()
 */
SPRet sprotocol_free(SProtocol *sp)
{
	return SPROTOCOL_RET_SUCCESS;
}

