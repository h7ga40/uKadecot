/*
 * Copyright (c) 2001-2005, Adam Dunkels.
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
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id$
 *
 */

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "net/ip/psock.h"
#include "httpd-fs.h"
#include "http_pcb.h"
#include "websocket.h"

struct httpd_state {
  unsigned char timer;
  struct psock sin, sout;
  struct pt outputpt;
  char inputbuf[50];
  char state;
  union{
    struct{
      int parse_pos;
      int parse_len;
      struct http_parser parser;
      struct http_parser_url handle;
      struct message message;
    };
    struct{
      char _dummy1[340];
      int drv;
      char filename[256];
      int len;
      char *query;
      struct httpd_fs_file file;
    };
    struct{
      char _dummy2[340];
      struct websocket websocket;
      int close_req;
    };
  };
};
#define get_context(p) (struct httpd_state *)((intptr_t)p - (intptr_t)&((struct httpd_state *)0)->parser)

void httpd_init(void);
void httpd_appcall(void);

void httpd_log(char *msg);
void httpd_log_file(uint16_t *requester, char *file);

#endif /* __HTTPD_H__ */
