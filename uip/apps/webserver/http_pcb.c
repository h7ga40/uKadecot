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

#include "httpd.h"
#include "http_pcb.h"
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#ifndef _MSC_VER
/* strnlen() is a POSIX.2008 addition. Can't rely on it being available so
 * define it ourselves.
 */
size_t
strnlen(const char *s, size_t maxlen)
{
  const char *p;

  p = memchr(s, '\0', maxlen);
  if (p == NULL)
    return maxlen;

  return p - s;
}
#endif

size_t
strlncat(char *dst, size_t len, const char *src, size_t n)
{
  size_t slen;
  size_t dlen;
  size_t rlen;
  size_t ncpy;

  slen = strnlen(src, n);
  dlen = strnlen(dst, len);

  if (dlen < len) {
    rlen = len - dlen;
    ncpy = slen < rlen ? slen : (rlen - 1);
    memcpy(dst + dlen, src, ncpy);
    dst[dlen + ncpy] = '\0';
  }

  assert(len > slen + dlen);
  return slen + dlen;
}

int
websvr_request_url(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  strlncat(context->message.request_url,
    sizeof(context->message.request_url),
    buf,
    len);
  return 0;
}

int
websvr_header_field(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  struct message *m = &context->message;

  if (strncmp("Referer", buf, len) == 0) {
    m->num_headers = 1;
  } else if (strncmp("Host", buf, len) == 0) {
    m->num_headers = 2;
  } else if (strncmp("Upgrade", buf, len) == 0) {
    m->num_headers = 3;
  } else if (strncmp("Connection", buf, len) == 0) {
    m->num_headers = 4;
  } else if (strncmp("Sec-WebSocket-Key", buf, len) == 0) {
    m->num_headers = 5;
  } else if (strncmp("Origin", buf, len) == 0) {
    m->num_headers = 6;
  } else if (strncmp("Sec-WebSocket-Protocol", buf, len) == 0) {
    m->num_headers = 7;
  } else if (strncmp("Sec-WebSocket-Version", buf, len) == 0) {
    m->num_headers = 8;
  } else {
    m->num_headers = 0;
  }

  return 0;
}

int
websvr_header_value(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  struct message *m = &context->message;

  switch (m->num_headers) {
  case 1:
    strlncat(m->referer, sizeof(m->referer), buf, len);
    break;
  case 2:
    strlncat(m->host, sizeof(m->host), buf, len);
    break;
  case 3:
    strlncat(m->upgrade, sizeof(m->upgrade), buf, len);
    break;
  case 4:
    strlncat(m->connection, sizeof(m->connection), buf, len);
    break;
  case 5:
    strlncat(m->sec_websocket_key, sizeof(m->sec_websocket_key), buf, len);
    break;
  case 6:
    strlncat(m->origin, sizeof(m->origin), buf, len);
    break;
  case 7:
    strlncat(m->sec_websocket_protocol, sizeof(m->sec_websocket_protocol), buf, len);
    break;
  case 8:
    strlncat(m->sec_websocket_version, sizeof(m->sec_websocket_version), buf, len);
    break;
  }

  return 0;
}

void
websvr_check_body_is_final(const http_parser *p)
{
  struct httpd_state *context = get_context(p);
  if (context->message.body_is_final) {
    printf("\n\n *** Error http_body_is_final() should return 1 "
      "on last on_body callback call "
      "but it doesn't! ***\n\n");
    assert(0);
  }
  context->message.body_is_final = http_body_is_final(p);
}

int
websvr_body(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  strlncat(context->message.body,
    sizeof(context->message.body),
    buf,
    len);
  context->message.body_size += len;
  websvr_check_body_is_final(p);
  // printf("websvr_body: '%s'\n", requests[num_messages].body);
  return 0;
}

int
websvr_count_body(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  assert(buf);
  context->message.body_size += len;
  websvr_check_body_is_final(p);
  return 0;
}

int
websvr_message_begin(http_parser *p)
{
  struct httpd_state *context = get_context(p);
  context->message.message_begin_cb_called = TRUE;
  return 0;
}

int
websvr_headers_complete(http_parser *p)
{
  struct httpd_state *context = get_context(p);
  context->message.method = p->method;
  context->message.http_major = p->http_major;
  context->message.http_minor = p->http_minor;
  context->message.headers_complete_cb_called = TRUE;
  context->message.should_keep_alive = http_should_keep_alive(p);
  return 0;
}

int
websvr_message_complete(http_parser *p)
{
  struct httpd_state *context = get_context(p);
  if (context->message.should_keep_alive != http_should_keep_alive(p)) {
    printf("\n\n *** Error http_should_keep_alive() should have same "
      "value in both on_message_complete and on_headers_complete "
      "but it doesn't! ***\n\n");
    assert(0);
  }

  if (context->message.body_size &&
    http_body_is_final(p) &&
    !context->message.body_is_final) {
    printf("\n\n *** Error http_body_is_final() should return 1 "
      "on last on_body callback call "
      "but it doesn't! ***\n\n");
    assert(0);
  }

  context->message.message_complete_cb_called = TRUE;
  return 0;
}

int
websvr_response_status(http_parser *p, const char *buf, size_t len)
{
  struct httpd_state *context = get_context(p);
  strlncat(context->message.response_status,
    sizeof(context->message.response_status),
    buf,
    len);
  return 0;
}

http_parser_settings websvr_settings =
{
  websvr_message_begin,
  websvr_request_url,
  websvr_response_status,
  websvr_header_field,
  websvr_header_value,
  websvr_headers_complete,
  websvr_body,
  websvr_message_complete,
};

