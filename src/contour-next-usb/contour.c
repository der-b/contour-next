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
#include <utils/log.h>
#include <astm/astm.h>
#include <contour-next-usb/contour.h>
#include <linux/hiddev.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <fcntl.h>

#define BUF_SIZE 64
#define FILENAME_SIZE 256
#define READ_EVENTS 20

/*
 * open_contour()
 */
ContourRet contour_init(Contour *c)
{
	DIR *dir;
	struct dirent *d;
	struct hiddev_devinfo devinfo;
	struct hiddev_usage_ref ref;
	char filename[FILENAME_SIZE + 1];

	memset(c, 0, sizeof(*c));

	dir = opendir(CONTOUR_HID_DIR);
	if (!dir) {
		ERROR("Clould not open directory '" CONTOUR_HID_DIR "'.");
		goto err;
	}

	strncpy(filename, CONTOUR_HID_DIR, FILENAME_SIZE);
	filename[FILENAME_SIZE] = 0;

	while ((d = readdir(dir))) {
		strncpy(filename + strlen(CONTOUR_HID_DIR), d->d_name, FILENAME_SIZE - strlen(CONTOUR_HID_DIR));

		c->fd = open(filename, O_RDWR);
		if (!c->fd) {
			WARN("File '%s' does not exist.", filename);
			continue;
		}
		
		if (0 > ioctl(c->fd, HIDIOCGDEVINFO, &devinfo)) {
			close(c->fd);
			c->fd = 0;
			continue;
		}

		if (   CONTOUR_NEXT_VENDOR_ID == devinfo.vendor 
		    && CONTOUR_NEXT_PRODUCT_ID == devinfo.product ) {
		    	//INFO("Contour next found: '%s'", filename);
			break;
		}

		close(c->fd);
		c->fd = 0;
	}

	if (closedir(dir)) {
		ERROR("closedir faild.");
		goto err;
	}

	if (!c->fd) {
		goto err;
	}

	memset(&ref, 0, sizeof(ref));
	ref.report_type = HID_REPORT_TYPE_OUTPUT;
	if (0 > ioctl(c->fd, HIDIOCGUSAGE, &ref)) {
		ERROR("Faild to get usage code.");
		goto err_fd;
	}

	c->usage_code = ref.usage_code;

	// protocoll initialisation
	if (sprotocol_init(&c->sp)) {
		ERROR("Initialisation of simple protocol faild.");
		goto err_fd;
	}

	if (astm1381_95_init(&c->astm_low)) {
		ERROR("Initialisation of astm 1381-95 faild.");
		goto err_sprotocol;
	}

	if (astm1394_91_init(&c->astm_high)) {
		ERROR("Initialisation of astm 1394-91 faild.");
		goto err_astmlow;
	}

	return CONTOUR_RET_SUCCESS;

err_astmlow:
	if (astm1381_95_free(&c->astm_low)) {
		WARN("astm1381_95_free() faild.");
	}
err_sprotocol:
	if (sprotocol_free(&c->sp)) {
		WARN("sprotocol_free() faild.");
	}
err_fd:
	close(c->fd);
	c->fd = 0;
err:
	return CONTOUR_RET_UNKNOWN;
}


/*
 * write_contour()
 */
ContourRet _contour_write(Contour *c, const uint8_t *buf, size_t len)
{
	struct hiddev_usage_ref ref;
	struct hiddev_report_info info;
	int i;

	memset(&ref, 0, sizeof(ref));
	memset(&info, 0, sizeof(info));

	ref.report_id = 0;
	ref.report_type = HID_REPORT_TYPE_OUTPUT;
	ref.field_index = 0;
	ref.usage_code = c->usage_code;
	
	for (i = 0; i < len; ++i) {
		ref.usage_index = i;
		ref.value = buf[i];

		if (ioctl(c->fd, HIDIOCSUSAGE, &ref)) {
			ERROR("ioctl() faild.");
			goto err;
		}
	}

	info.report_type = HID_REPORT_TYPE_OUTPUT;
	info.report_id = 0;
	info.num_fields = 1;

	if (ioctl(c->fd, HIDIOCSREPORT, &info)) {
		ERROR("ioctl() faild.");
		goto err;
	}
	
	return CONTOUR_RET_SUCCESS;

err:
	return CONTOUR_RET_UNKNOWN;
}


/*
 * contour_read()
 */
ContourRet contour_read(Contour *c, ContourPacket *p)
{
	uint8_t tmp[BUF_SIZE];
	uint8_t *ptr;
	ASTMRet ret;
	SPPacket spp;
	ContourRet myret = CONTOUR_RET_UNKNOWN;

	do {
		if (_contour_read(c, tmp, BUF_SIZE)) {
			ERROR("_contour_read() faild.");
			goto err;
		}

		if (sprotocol_interprete(&c->sp, tmp, BUF_SIZE, &spp)) {
			ERROR("sprotocol_interpete() faild.");
			goto err;
		}

		if (!spp.size) {
			ERROR("nothing readed.");
			goto err;
		}

		p->len += spp.size;
		ptr = realloc(p->buf, p->len);
		if (!ptr) {
			ERROR("Not enought memory.");
			goto err;
		}
		p->buf = ptr;

		memcpy(p->buf + p->len - spp.size, spp.payload, spp.size);
		ret = _contour_interprete(c, p);

	} while(CONTOUR_RET_ENDNOTFOUND == ret);

	if (ret) {
		ERROR("Error while ASTM pharsing");
		goto err;
	}

	return CONTOUR_RET_SUCCESS;

err:
	free(p->buf);
	p->buf = NULL;
	return myret;
}


/*
 * _contour_read();
 */
ContourRet _contour_read(Contour *c, uint8_t *buf, size_t len) 
{
	struct hiddev_event event;
	int ret;
	int i;

	memset(&event, 0, sizeof(event));

	for (i = 0; i < len; ++i) {
		ret = read(c->fd, &event, sizeof(event));
		if( 0 > ret ) {
			ERROR("read() faild.");
			goto err;
		}

		buf[i] = event.value;
	}

	return CONTOUR_RET_SUCCESS;

err:
	return CONTOUR_RET_UNKNOWN;
}


/*
 * _contour_interprete()
 */
ContourRet _contour_interprete(Contour *c, ContourPacket *p)
{
	ASTMRet ret;

	ret = astm1381_95_interprete(&c->astm_low, p->buf, p->len, &p->astm_low);
	if (ASTM_RET_ENDNOTFOUND == ret) {
		return CONTOUR_RET_ENDNOTFOUND;
	}

	if (ret) {
		ERROR("astm1381_95_interprete() faild.");
		goto err;
	}

	if (ASTM_FRAME_TEXT == p->astm_low.type) {
		if (astm1394_91_interprete(&c->astm_high, p->astm_low.text.payload, p->astm_low.text.size, &p->content)) {
			ERROR("astm1394_91_interprete() faild.");
			goto err;
		}
	}

	return CONTOUR_RET_SUCCESS;

err:
	return CONTOUR_RET_UNKNOWN;
}


/*
 * contour_free()
 */
ContourRet contour_free(Contour *c)
{
	
	if (astm1394_91_init(&c->astm_high)) {
		WARN("freeing astm 1294-91 faild.");
	}

	if (astm1381_95_free(&c->astm_low)) {
		WARN("freeing astm 1281-95 faild.");
	}

	if (sprotocol_free(&c->sp)) {
		WARN("freeing sprotocol faild.");
	}

	if (close(c->fd)) {
		WARN("closing usb device faild");
	}

	c->fd = 0;
	return CONTOUR_RET_SUCCESS;
}


/*
 * contour_packet_init()
 */
ContourRet contour_packet_init(ContourPacket *p)
{
	memset(p, 0, sizeof(*p));
	return CONTOUR_RET_SUCCESS;
}


/*
 * contour_packet_free()
 */
ContourRet contour_packet_free(ContourPacket *p)
{
	free(p->buf);
	p->buf = NULL;
	return CONTOUR_RET_SUCCESS;
}


/*
 * main()
 */
int main(int argc, char **argv)
{
	Contour _c;
	Contour *c = &_c;
	uint8_t sbuf[BUF_SIZE];
	ContourPacket cp;
	struct _sprotocol_header *h;
	ContourRet ret;

	memset(sbuf, 0, BUF_SIZE);
	memset(&cp, 0, sizeof(cp));

	if (contour_init(c)) {
		ERROR("Device not found.");
		goto err;
	}


	if (contour_packet_init(&cp)) {
		ERROR("initializing contour packet faild.");
		goto err_c;
	}
	
	/* Constructing an ASTM 1381-95 ACK packet.
	 * After sendign this the device enters Data transmission mode.
	 * Each received ASTM 1394-91 packet have to be answered with
	 * an ASTM 1381-95 ACK packet.
	 * When an ASTM 1381-95 EOT packet was received, then all data
	 * from the device was transmitted.
	 */
	h = (struct _sprotocol_header *)sbuf;
	h->size = 1;
	sbuf[sizeof(*h)] = ASTM_CC_ACK;

	do {
		if (contour_packet_free(&cp)) {
			ERROR("freeing contour packet faild.");
			goto err_c;
		}

		if (contour_packet_init(&cp)) {
			ERROR("initializing contour packet faild.");
			goto err_c;
		}

		if (_contour_write(c, sbuf, BUF_SIZE)) {
			ERROR("contour_write() faild.");
			goto err_cp;
		}

		ret = contour_read(c, &cp);
		if (ASTM_FRAME_TEXT != cp.astm_low.type) {
			break;
		}

		if (ret) {
			ERROR("read_contour() faild.");
			goto err_cp;
		}

		if (cp.content.len != write(STDOUT_FILENO, cp.content.buf, cp.content.len)) {
			ERROR("write() to STDOUT_FILENO faild.");
			goto err_cp;
		}

		if (strlen("\n") != write(STDOUT_FILENO, "\n", strlen("\n"))) {
			ERROR("write() to STDOUT_FILENO faild.");
			goto err_cp;
		}
	} while(1);
	
	
	if (contour_free(c)) {
		ERROR("contour_free() faild.");
		goto err;
	}

	return 0;


err_cp:
	if (contour_packet_free(&cp)) {
		WARN("freeing contour packet faild.");
	}
err_c:
	if (contour_free(c)) {
		WARN("contour_free(c) faild.");
	}
err:
	return 1;
}

