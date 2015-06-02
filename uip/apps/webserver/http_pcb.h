/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __HTTP_PCB_H__
#define __HTTP_PCB_H__

#include "http_parser.h"

#define MAX_ELEMENT_SIZE 256

struct message
{
	enum http_method method;
	unsigned short http_major;
	unsigned short http_minor;
	char request_url[MAX_ELEMENT_SIZE];
	char response_status[64];
	int num_headers;
	char host[MAX_ELEMENT_SIZE];
	char referer[MAX_ELEMENT_SIZE];
	char upgrade[32];
	char connection[32];
	char sec_websocket_key[64];
	char origin[MAX_ELEMENT_SIZE];
	char sec_websocket_protocol[64];
	char sec_websocket_version[4];
	char response_key[86];
	size_t body_size;
	char body[MAX_ELEMENT_SIZE];
	int should_keep_alive;
	int headers_complete_cb_called;
	int message_begin_cb_called;
	int message_complete_cb_called;
	int body_is_final;
};

#ifndef _MSC_VER
size_t strnlen(const char *s, size_t maxlen);
#endif
size_t strlncat(char *dst, size_t len, const char *src, size_t n);
extern http_parser_settings websvr_settings;

#endif /* __HTTP_PCB_H__ */
