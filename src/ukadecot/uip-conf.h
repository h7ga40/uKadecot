/**
 * \addtogroup uipopt
 * @{
 */

/**
 * \name Project-specific configuration options
 * @{
 *
 * uIP has a number of configuration options that can be overridden
 * for each project. These are kept in a project-specific uip-conf.h
 * file and all configuration names have the prefix UIP_CONF.
 */

/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * This file is part of the uIP TCP/IP stack
 *
 * $Id$
 */

/**
 * \file
 *         An example uIP configuration file
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

/**
 * 8 bit datatype
 *
 * This typedef defines the 8-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef unsigned char u8_t;

/**
 * 16 bit datatype
 *
 * This typedef defines the 16-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef unsigned short u16_t;

/**
 * Statistics datatype
 *
 * This typedef defines the dataype used for keeping statistics in
 * uIP.
 *
 * \hideinitializer
 */
typedef unsigned short uip_stats_t;

/**
 * Maximum number of TCP connections.
 *
 * \hideinitializer
 */
#define UIP_CONF_MAX_CONNECTIONS 20

/**
 * Maximum number of listening TCP ports.
 *
 * \hideinitializer
 */
#define UIP_CONF_MAX_LISTENPORTS 10

/**
 * uIP buffer size.
 *
 * \hideinitializer
 */
//#define UIP_CONF_BUFFER_SIZE     420
#define UIP_CONF_BUFFER_SIZE     1514

/**
 * Use uIP stack buffer or external driver buffer.
 *
 * \hideinitializer
 */
#define UIP_CONF_EXTERNAL_BUFFER 1


/**
 * CPU byte order.
 *
 * \hideinitializer
 */
// #define UIP_CONF_BYTE_ORDER      LITTLE_ENDIAN
#ifdef __LIT
#define UIP_CONF_BYTE_ORDER      UIP_LITTLE_ENDIAN
#else
#define UIP_CONF_BYTE_ORDER      UIP_BIG_ENDIAN
#endif

/**
 * Logging on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_LOGGING         1

/**
 * Enable IPv6.
 *
 * \hideinitializer
 */
#define UIP_CONF_IPV6            0

/**
 * UDP support on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP             1

/**
 * UDP checksums on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP_CHECKSUMS   1

/**
 * uIP statistics on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_STATISTICS      1

/**
 * uIP UDP Broadcast on or off
 *
 * \hideinitializer
 */
// Enable UDP Brodcast packet processing
#define UIP_CONF_BROADCAST       UIP_CONF_UDP

#ifndef printf
#include <stdio.h>

extern void syslog(unsigned int prio, const char *format, ...);
#define printf(...) syslog(7/*LOG_DEBUG*/, __VA_ARGS__)
#endif

/* Here we include the header file for the application(s) we use in
   our project. */
/*#include "smtp.h"*/
/*#include "hello-world.h"*/
/*#include "telnetd.h"*/
#include "webserver.h"
#include "dhcpc.h"
/*#include "resolv.h"*/
/*#include "webclient.h"*/
/*#include "httpd.h"*/

#include "timer.h"
#include "pt.h"

struct ecnl_state {
	struct pt pt;
	struct uip_timer timer;
};

/*#define uip_tcp_appstate_t \
struct uip_tcp_adptstate {\
	void (*tcp_callback)(struct uip_conn *uip_conn, int fncd, void *p_parblk);\
	union {\
		struct httpd_state httpd;\
	} parblk;\
}
void uip_appcall(void);
#define UIP_APPCALL uip_appcall*/

void uip_udp_appcall(void);
#define uip_udp_appstate_t \
struct uip_udp_adptstate {\
	void (*udp_callback)(struct uip_udp_conn *uip_udp_conn, int fncd, void *p_parblk);\
	union {\
		struct dhcpc_state dhcpc;\
		struct ecnl_state ecnl;\
	} parblk; \
}
#define UIP_UDP_APPCALL uip_udp_appcall

#endif /* __UIP_CONF_H__ */

/** @} */
/** @} */
