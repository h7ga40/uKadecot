/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup httpd Web server
 * @{
 * The uIP web server is a very simplistic implementation of an HTTP
 * server. It can serve web pages and files from a read-only ROM
 * filesystem, and provides a very small scripting language.

 */

/**
 * \file
 *         Web server
 * \author
 *         Adam Dunkels <adam@sics.se>
 */


/*
 * Copyright (c) 2004, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id$
 */

#include "net/ip/uip.h"
#include "httpd.h"
#include "httpd-fs.h"
#include "http-strings.h"
#include "base64.h"
#include "sha1.h"
#include "kadecot_names.h"
#include "uip_adpt.h"

#include <string.h>

#define STATE_WAITING 0
#define STATE_OUTPUT  1
#define STATE_WS_OUTPUT  2

#define ISO_nl      0x0a
#define ISO_space   0x20
#define ISO_bang    0x21
#define ISO_percent 0x25
#define ISO_period  0x2e
#define ISO_slash   0x2f
#define ISO_colon   0x3a

#ifndef _MSC_VER
#ifndef strcpy_s
#define strcpy_s(s1, s1m, s2) strcpy(s1, s2)
#endif

#ifndef strncpy_s
#define strncpy_s(dst, dsz, src, sz) strncpy(dst, src, sz)
#endif
#endif

#define MAX(a, b) ((a > b) ? a : b)

union temp_type_t {
	char binary[sizeof(http_content_type_binary)];
	char html[sizeof(http_content_type_html)];
	char css[sizeof(http_content_type_css)];
	char js[sizeof(http_content_type_js)];
	char json[sizeof(http_content_type_json)];
	char png[sizeof(http_content_type_png)];
	char gif[sizeof(http_content_type_gif)];
	char jpg[sizeof(http_content_type_jpg)];
	char svg[sizeof(http_content_type_svg)];
	char plain[sizeof(http_content_type_plain)];
};

struct temp_buf_t{
	char headers[MAX(sizeof(http_header_404), sizeof(http_header_200))
		+ sizeof(http_content_encoding_gzip)
		+ sizeof(http_location) + sizeof(((struct httpd_state *)0)->filename) + 2
		+ sizeof(union temp_type_t)
	];
	char ws_headers[sizeof(http_header_101)
		+ sizeof(http_upgrade) + sizeof(((struct httpd_state *)0)->message.upgrade) + sizeof(http_crnl)
		+ sizeof(http_connection) + sizeof(((struct httpd_state *)0)->message.connection) + sizeof(http_crnl)
		+ sizeof(http_sec_websocket_accept) + sizeof(((struct httpd_state *)0)->message.response_key) + sizeof(http_crnl)
		+ sizeof(http_sec_websocket_protocol) + sizeof(((struct httpd_state *)0)->message.sec_websocket_protocol) + sizeof(http_crnl)
		+ sizeof(http_crnl)
	];
};
static char temp_buf[sizeof(struct temp_buf_t)];

int httpd_strnicmp(const char *s1, const char *s2, size_t n)
{
	int i;
	char c1, c2;

	for(i = 0; i < n; i++, s1++, s2++){
		c1 = *s1;
		c2 = *s2;
		if(c1 == '\0' && c2 == '\0')
			return 0;

		if(c1 >= 'a' && c1 <= 'z')
			c1 += 'A' - 'a';

		if(c2 >= 'a' && c2 <= 'z')
			c2 += 'A' - 'a';

		if(c1 < c2)
			return -1;

		if(c1 > c2)
			return 1;
	}

	return 0;
}

struct websocket *websocket_getws(ID wbsid)
{
	struct uip_conn *conn = uip_getconn(wbsid);
	if (conn == NULL)
		return NULL;

	return &conn->appstate.websocket;
}

/*---------------------------------------------------------------------------*/
static unsigned short
generate_part_of_file(void *state)
{
  struct httpd_state *s = (struct httpd_state *)state;
  int len;

  if (s->file.len > uip_mss()) {
    len = uip_mss();
  } else {
    len = s->file.len;
  }
  s->len = httpd_fs_read(&s->file, uip_appdata, len);

  return s->len;
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_file(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sout);

  do {
    PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_file, s);
    s->file.len -= s->len;
    s->file.pos += s->len;
  } while (s->file.len > 0);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_headers(struct httpd_state *s, const char *statushdr))
{
  char *pos = temp_buf;
  int len;
  char *ptr;

  PSOCK_BEGIN(&s->sout);

  len = strlen(statushdr);
  memcpy(pos, statushdr, len); pos += len;

  if (s->drv == 0) {
    len = strlen(http_content_encoding_gzip);
    memcpy(pos, http_content_encoding_gzip, len); pos += len;
  }

  if (s->file.redirect) {
    len = strlen(http_location);
    memcpy(pos, http_location, len); pos += len;
    if (s->drv == 1) {
      len = 2;
      memcpy(pos, "/~", len); pos += len;
    }
    len = strlen(s->filename);
    memcpy(pos, s->filename, len); pos += len;
    if (s->query != NULL) {
      pos[0] = '?'; pos++;
      len = strlen(s->query);
      memcpy(pos, s->query, len); pos += len;
    }
    len = 2;
    memcpy(pos, "\r\n", len); pos += len;
  }

  ptr = strrchr(s->filename, ISO_period);
  if (ptr == NULL) {
    len = strlen(http_content_type_binary);
    memcpy(pos, http_content_type_binary, len); pos += len;
  } else if (strncmp(http_html, ptr, 5) == 0 ||
    strncmp(http_shtml, ptr, 6) == 0) {
    len = strlen(http_content_type_html);
    memcpy(pos, http_content_type_html, len); pos += len;
  } else if (strncmp(http_css, ptr, 4) == 0) {
    len = strlen(http_content_type_css);
    memcpy(pos, http_content_type_css, len); pos += len;
  } else if (strncmp(http_js, ptr, 3) == 0) {
    len = strlen(http_content_type_js);
    memcpy(pos, http_content_type_js, len); pos += len;
  } else if (strncmp(http_json, ptr, 5) == 0) {
    len = strlen(http_content_type_json);
    memcpy(pos, http_content_type_json, len); pos += len;
  } else if (strncmp(http_png, ptr, 4) == 0) {
    len = strlen(http_content_type_png);
    memcpy(pos, http_content_type_png, len); pos += len;
  } else if (strncmp(http_gif, ptr, 4) == 0) {
    len = strlen(http_content_type_gif);
    memcpy(pos, http_content_type_gif, len); pos += len;
  } else if (strncmp(http_jpg, ptr, 4) == 0) {
    len = strlen(http_content_type_jpg);
    memcpy(pos, http_content_type_jpg, len); pos += len;
  } else if (strncmp(http_svg, ptr, 4) == 0) {
    len = strlen(http_content_type_svg);
    memcpy(pos, http_content_type_svg, len); pos += len;
  } else {
    len = strlen(http_content_type_plain);
    memcpy(pos, http_content_type_plain, len); pos += len;
  }
  *pos = '\0';

  PSOCK_SEND(&s->sout, temp_buf, pos - temp_buf);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_output(struct httpd_state *s))
{
  PT_BEGIN(&s->outputpt);

  if (!httpd_fs_open(s->drv, s->filename, sizeof(s->filename), &s->file)) {
    s->drv = 0;
    strcpy_s(s->filename, sizeof(s->filename), http_404_html);
    httpd_fs_open(s->drv, s->filename, sizeof(s->filename), &s->file);
    PT_WAIT_THREAD(&s->outputpt,
      send_headers(s,
      http_header_404));
    PT_WAIT_THREAD(&s->outputpt, send_file(s));
  } else {
    PT_WAIT_THREAD(&s->outputpt,
      send_headers(s,
      s->file.redirect ? http_header_301 : http_header_200));
    PT_WAIT_THREAD(&s->outputpt, send_file(s));
  }

  PSOCK_CLOSE(&s->sout);
  PT_END(&s->outputpt);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_ws_headers(struct httpd_state *s, const char *statushdr))
{
  char *pos = temp_buf;
  int len;

  PSOCK_BEGIN(&s->sout);

  len = strlen(statushdr);
  memcpy(pos, statushdr, len); pos += len;

  len = strlen(http_upgrade);
  memcpy(pos, http_upgrade, len); pos += len;
  len = strlen(s->message.upgrade);
  memcpy(pos, s->message.upgrade, len); pos += len;
  len = strlen(http_crnl);
  memcpy(pos, http_crnl, len); pos += len;

  len = strlen(http_connection);
  memcpy(pos, http_connection, len); pos += len;
  len = strlen(s->message.connection);
  memcpy(pos, s->message.connection, len); pos += len;
  len = strlen(http_crnl);
  memcpy(pos, http_crnl, len); pos += len;

  len = strlen(http_sec_websocket_accept);
  memcpy(pos, http_sec_websocket_accept, len); pos += len;
  len = strlen(s->message.response_key);
  memcpy(pos, s->message.response_key, len); pos += len;
  len = strlen(http_crnl);
  memcpy(pos, http_crnl, len); pos += len;

  len = strlen(http_sec_websocket_protocol);
  memcpy(pos, http_sec_websocket_protocol, len); pos += len;
  len = strlen(s->message.sec_websocket_protocol);
  memcpy(pos, s->message.sec_websocket_protocol, len); pos += len;
  len = strlen(http_crnl);
  memcpy(pos, http_crnl, len); pos += len;

  len = strlen(http_crnl);
  memcpy(pos, http_crnl, len); pos += len;
  *pos = '\0';

  PSOCK_SEND(&s->sout, temp_buf, pos - temp_buf);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_part_of_ws_data(void *state)
{
  struct httpd_state *s = (struct httpd_state *)state;

  s->len = websocket_output(&s->websocket, uip_appdata, uip_mss());

  return s->len;
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_ws_data(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_ws_data, s);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_ws_output(struct httpd_state *s))
{
  char shaHash[20];
  SHA_CTX sha1;
  int len;

  PT_BEGIN(&s->outputpt);

  strlncat(s->message.response_key, sizeof(s->message.response_key),
    s->message.sec_websocket_key, sizeof(s->message.sec_websocket_key));
  len = strlncat(s->message.response_key, sizeof(s->message.response_key),
    http_websocket_guid, sizeof(http_websocket_guid));
  memset(shaHash, 0, sizeof(shaHash));
  SHA1_Init(&sha1);
  SHA1_Update(&sha1, (sha1_byte *)s->message.response_key, len);
  SHA1_Final((sha1_byte *)shaHash, &sha1);
  base64_encode((unsigned char *)s->message.response_key,
    sizeof(s->message.response_key), (unsigned char *)shaHash, sizeof(shaHash));

  PT_WAIT_THREAD(&s->outputpt, send_ws_headers(s, http_header_101));

  s->message.response_key[0] = '\0';

  do {
    while(!websocket_newdata(&s->websocket))
      PT_YIELD(&s->outputpt);

    PT_WAIT_THREAD(&s->outputpt, send_ws_data(s));
  } while ((s->state == STATE_WS_OUTPUT) && (!s->close_req));
  s->state = STATE_WAITING;
  websocket_destroy(&s->websocket);
  s->close_req = 0;

  PSOCK_CLOSE(&s->sout);

  PT_END(&s->outputpt);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_input(struct httpd_state *s))
{
  size_t done;
  const char *data;
  char *ptr;

  PSOCK_BEGIN(&s->sin);

  memset(&s->message, 0, sizeof(s->message));
  http_parser_init(&s->parser, HTTP_REQUEST);

  for (;;) {
    PSOCK_WAIT_UNTIL(&s->sin, psock_newdata(&s->sin));

    s->parse_pos = 0;
    s->parse_len = uip_datalen();
    data = &((const char *)uip_appdata)[s->parse_pos];
    done = http_parser_execute(&s->parser, &websvr_settings, data, s->parse_len);
    if (done != 0) {
      s->parse_pos += done;
      s->parse_len -= done;
      break;
    }
  }

  if (!s->message.message_complete_cb_called
    && s->message.method != HTTP_GET) {
    PSOCK_CLOSE_EXIT(&s->sin);
  }

  ptr = strrchr(s->message.request_url, '?');
  if (ptr != NULL){
    ptr[0] = '\0';
    s->query = &ptr[1];
  }
  else
    s->query = NULL;

  /* ""か"/"なら"index.html"に変更 */
  if ((s->message.request_url[0] == '\0') || ((s->message.request_url[0] == '/') && (s->message.request_url[1] == '\0'))) {
    s->drv = 0;
    strcpy_s(s->filename, sizeof(s->filename), http_index_html);
    s->file.redirect = 1;
  }
  /* "/~/"ならSDカードから読み込み */
  else if ((s->message.request_url[0] == '/') && (s->message.request_url[1] == '~') && (s->message.request_url[2] == '/')) {
    s->drv = 1;
    strcpy_s(s->filename, sizeof(s->filename), &s->message.request_url[2]);
  }
  else {
    s->drv = 0;
    strcpy_s(s->filename, sizeof(s->filename), s->message.request_url);
  }

  /*  httpd_log_file(uip_conn->ripaddr, s->message.request_url);*/
  /*  httpd_log(s->message.referer);*/

  if (httpd_strnicmp(http_websocket, s->message.upgrade, sizeof(s->message.upgrade)) == 0) {
    s->state = STATE_WS_OUTPUT;

    s->close_req = 0;
    websocket_init(&s->websocket, uip_getid((struct uip_conn *)((intptr_t)s - offsetof(struct uip_conn, appstate))));
    for (;;) {
      if(s->parse_len <= 0){
        PT_YIELD(&s->sin.pt);
        PSOCK_WAIT_UNTIL(&s->sin, psock_newdata(&s->sin));

        s->parse_pos = 0;
        s->parse_len = uip_datalen();
      }
      data = &((const char *)uip_appdata)[s->parse_pos];
      done = websocket_input(&s->websocket, (void *)data, s->parse_len);
      if ((done != 0) || (s->websocket.rstate.opecode == connection_close)) {
        s->close_req = 1;
        PSOCK_CLOSE_EXIT(&s->sin);
        break;
      }
      s->parse_pos = s->parse_len;
      s->parse_len = 0;
    }
  }
  else {
    s->state = STATE_OUTPUT;

    while (1) {
      PSOCK_READTO(&s->sin, ISO_nl);
    }
  }

  PSOCK_END(&s->sin);
}
/*---------------------------------------------------------------------------*/
static void
handle_connection(struct httpd_state *s)
{
  handle_input(s);
  switch (s->state) {
  case STATE_OUTPUT:
    handle_output(s);
    break;
  case STATE_WS_OUTPUT:
    handle_ws_output(s);
    break;
  }
}
/*---------------------------------------------------------------------------*/
void
httpd_appcall(void)
{
  struct httpd_state *s = (struct httpd_state *)&(uip_conn->appstate);

  if (uip_closed() || uip_aborted() || uip_timedout()) {
  } else if (uip_connected()) {
    if(s->state != STATE_WS_OUTPUT){
      PSOCK_INIT(&s->sin, s->inputbuf, sizeof(s->inputbuf) - 1);
      PSOCK_INIT(&s->sout, s->inputbuf, sizeof(s->inputbuf) - 1);
      PT_INIT(&s->outputpt);
      s->state = STATE_WAITING;
      /*    timer_set(&s->timer, CLOCK_SECOND * 100);*/
      s->timer = 0;
      handle_connection(s);
    }
    else {
      handle_connection(s);
    }
  } else if (s != NULL) {
    if(s->state != STATE_WS_OUTPUT){
      if (uip_poll()) {
        ++s->timer;
        if (s->timer >= 20) {
          uip_abort();
        }
      } else {
        s->timer = 0;
      }
    }
    handle_connection(s);
  } else {
    uip_abort();
  }
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Initialize the web server
 *
 *             This function initializes the web server and should be
 *             called at system boot-up.
 */
void
httpd_init(void)
{
  uip_listen(UIP_HTONS(41314));

  httpd_fs_init();
  kadecot_names_init();
}
/*---------------------------------------------------------------------------*/
/** @} */
