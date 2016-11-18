/*
 * Copyright (c) 2001, Adam Dunkels.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
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

#include <string.h>
#include <kernel.h>
#include "net/ip/uip.h"
#include "net/ipv4/uip_arp.h"
#include "sys/timer.h"
#include "uip_task.h"
#include "uip_nic_config.h"
#include "kernel_cfg.h"
#include "uip_adpt.h"
#include "uip_app_config.h"
#include "sil.h"

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

struct uip_task_t {
	ID tskid;
	struct pt pt;
	struct uip_timer periodic_timer, arp_timer;
	clock_time_t timer;
	uint8_t uip_buf[UIP_BUFSIZE + 2];
};

struct uip_task_t uiptsk = {
	UIP_TASK
};

uint8_t *uip_buf = uiptsk.uip_buf;

typedef struct _uip_out_buf_t
{
	union {
		struct uip_conn *tcp;
		struct uip_udp_conn *udp;
	} cepid;
	uip_ipaddr_t ripaddr;
	uint16_t rport;
	int len;
	uint8_t data[UIP_BUFSIZE + 2];
} UIP_OUT_BUF_T;

#define UIP_BUF_COUNT 6
UIP_OUT_BUF_T uip_out_buf[UIP_BUF_COUNT];
int uip_out_wpos = 0;
int uip_out_rpos = 0;
uint8_t my_ip[4] = { IPV4_ADDR_LOCAL };
uint8_t my_netmask[4] = { IPV4_ADDR_LOCAL_MASK };
uint8_t my_default_router[4] = { IPV4_ADDR_DEFAULT_GW };
int ws_out_req;
int ws_out_res;
#ifdef __DHCPC_H__
struct dhcpc_state *dhcpc;
extern bool_t dhcp_enable;
#endif

/*---------------------------------------------------------------------------*/
static
PT_THREAD(uip_task_pt(struct uip_task_t *uiptsk))
{
	T_IF_SOFTC	*ic = IF_ETHER_NIC_GET_SOFTC();
	int ret;
	bool_t tx;

	PT_BEGIN(&uiptsk->pt);

	/* NIC を初期化する。*/
	IF_ETHER_NIC_PROBE(ic);
	IF_ETHER_NIC_INIT(ic);

	uip_sethostaddr((uip_ipaddr_t *)&my_ip);
	uip_setnetmask((uip_ipaddr_t *)&my_netmask);
	uip_setdraddr((uip_ipaddr_t *)&my_default_router);

	timer_set(&uiptsk->periodic_timer, CLOCK_SECOND / 2);
	timer_set(&uiptsk->arp_timer, CLOCK_SECOND * 10);

	uip_init();
#ifdef __DHCPC_H__
	if(dhcp_enable){
		dhcpc = dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));
	}
#endif
	uip_task_init((intptr_t)uiptsk);

	for (;;) {
		sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) & ~0x03);

		PT_WAIT_UNTIL(&uiptsk->pt, (ret = ic->link_pre != ic->link_now ? 1 :
			(uip_out_buf[uip_out_rpos].len > 0 ? 3 :
			(ws_out_req != ws_out_res ? 4 :
			(timer_expired(&uiptsk->periodic_timer) ? 5 :
			(ic->rxb_read != ic->rxb_write ? 2 : 0))))) != 0);

		if(ret == 1){
			sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x01);
			if(IF_ETHER_NIC_LINK(ic)){
				bool_t cng = ic->link_pre != ic->link_now;
				ic->link_pre = ic->link_now;
				if(cng && ic->link_pre){
#ifdef __DHCPC_H__
					if(dhcp_enable){
						sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x04);
						uip_buf = uiptsk->uip_buf;
						uip_len = 0;
						dhcpc_request(dhcpc);
						/* If the above function invocation resulted in data that
							should be sent out on the network, the global variable
							uip_len is set to a value > 0. */
						if (uip_len > 0) {
							uip_arp_out();
							for (;;) {
								tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
								if (tx)
									break;
								PT_YIELD(&uiptsk->pt);
							}
						}
					}
					else{
						DHCP4_CLI_SET_ADDR_CALLBACK();
					}
#else
					DHCP4_CLI_SET_ADDR_CALLBACK();
#endif
				}
			}
			PT_YIELD(&uiptsk->pt);
			continue;
		}
		else if (ret == 2) {
			sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x02);
			ic->rxb_read++;
			uip_buf = uiptsk->uip_buf;
			uip_len = IF_ETHER_NIC_READ(ic, (void **)&uip_buf);
			if ((uip_len > 0) && (uip_buf != NULL)){
				if (((struct uip_eth_hdr *)uip_buf)->type == uip_htons(UIP_ETHTYPE_IP)) {
					uip_arp_ipin();
					uip_input();
					/* If the above function invocation resulted in data that
					   should be sent out on the network, the global variable
					   uip_len is set to a value > 0. */
					if (uip_len > 0) {
						uip_arp_out();
						for (;;) {
							tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
							if (tx)
								break;
							PT_YIELD(&uiptsk->pt);
						}
						PT_YIELD(&uiptsk->pt);
						continue;
					}
				}
				else if (((struct uip_eth_hdr *)uip_buf)->type == uip_htons(UIP_ETHTYPE_ARP)) {
					uip_arp_arpin();
					/* If the above function invocation resulted in data that
					   should be sent out on the network, the global variable
					   uip_len is set to a value > 0. */
					if (uip_len > 0) {
						for (;;) {
							tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
							if (tx)
								break;
							PT_YIELD(&uiptsk->pt);
						}
						PT_YIELD(&uiptsk->pt);
						continue;
					}
				}
			}
		}
#if UIP_UDP
		else if (ret == 3) {
			UIP_OUT_BUF_T *buf = &uip_out_buf[uip_out_rpos];
			struct uip_udp_conn *cepid = buf->cepid.udp;

			sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x02);

			uip_ipaddr_copy(&cepid->ripaddr, &buf->ripaddr);
			cepid->rport = buf->rport;
			uip_buf = buf->data;
			uip_udp_send(buf->len);

			uip_udp_conn = cepid;
			uip_process(UIP_UDP_SEND_CONN);
			uip_ipaddr(&cepid->ripaddr, 0, 0, 0, 0);
			cepid->rport = 0;
			uip_arp_out();
			for (;;) {
				tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
				if (tx)
					break;
				PT_YIELD(&uiptsk->pt);
			}

			buf = &uip_out_buf[uip_out_rpos];
			uip_out_rpos++;
			if (uip_out_rpos >= UIP_BUF_COUNT)
				uip_out_rpos = 0;
			buf->len = 0;

			PT_YIELD(&uiptsk->pt);
			continue;
		}
#endif /* UIP_UDP */
		else if (ret == 4) {
			sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x02);
			ws_out_res++;
			for (ic->udp_pos = 0; ic->udp_pos < UIP_CONNS; ic->udp_pos++) {
				uip_buf = uiptsk->uip_buf;
				uip_len = 0;
				uip_poll_conn(&uip_conns[ic->udp_pos]);
				/* If the above function invocation resulted in data that
				   should be sent out on the network, the global variable
				   uip_len is set to a value > 0. */
				if (uip_len > 0) {
					uip_arp_out();
					for (;;) {
						tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
						if (tx)
							break;
						PT_YIELD(&uiptsk->pt);
					}
					PT_YIELD(&uiptsk->pt);
				}
			}
		}
		else if (ret == 5) {
			timer_reset(&uiptsk->periodic_timer);
			for (ic->udp_pos = 0; ic->udp_pos < UIP_CONNS; ic->udp_pos++) {
				uip_buf = uiptsk->uip_buf;
				uip_len = 0;
				uip_periodic(ic->udp_pos);
				/* If the above function invocation resulted in data that
				   should be sent out on the network, the global variable
				   uip_len is set to a value > 0. */
				if (uip_len > 0) {
					uip_arp_out();
					for (;;) {
						tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
						if (tx)
							break;
						PT_YIELD(&uiptsk->pt);
					}
					PT_YIELD(&uiptsk->pt);
				}
			}
#if UIP_UDP
			for (ic->udp_pos = 0; ic->udp_pos < UIP_UDP_CONNS; ic->udp_pos++) {
				uip_buf = uiptsk->uip_buf;
				uip_len = 0;
				uip_udp_periodic(ic->udp_pos);
				/* If the above function invocation resulted in data that
				   should be sent out on the network, the global variable
				   uip_len is set to a value > 0. */
				if (uip_len > 0) {
					uip_arp_out();
					for (;;) {
						tx = IF_ETHER_NIC_START(ic, uip_buf, uip_len);
						if (tx)
							break;
						PT_YIELD(&uiptsk->pt);
					}
					PT_YIELD(&uiptsk->pt);
				}
			}
#endif /* UIP_UDP */

			/* Call the ARP timer function every 10 seconds. */
			if (timer_expired(&uiptsk->arp_timer)) {
				timer_reset(&uiptsk->arp_timer);
				uip_buf = uiptsk->uip_buf;
				uip_len = 0;
				uip_arp_timer();
			}
		}
	}

	PT_END(&uiptsk->pt);
}

/*
 *  uIP プロトスレッド起動タスク
 */
void uip_task(intptr_t exinf)
{
	struct uip_task_t *uiptsk = (struct uip_task_t *)exinf;

	uip_task_pt(uiptsk);
}

/*
 *  uIP タスク起動用周期ハンドラー
 */
void uip_cychdr(intptr_t exinf)
{
	struct uip_task_t *uiptsk = (struct uip_task_t *)exinf;
	uiptsk->timer++;
#ifndef UIP_ACT_TSK_ONLY_ON_SEND
	if(timer_expired(&uiptsk->periodic_timer))
		(void)iact_tsk(uiptsk->tskid);
#endif
}

struct uip_conn *
tcp_acre_cep(const T_TCP_CCEP *pk_ccep)
{
	struct uip_conn *cepid;

	cepid = uip_connect((uip_ipaddr_t *)&pk_ccep->ripaddr, uip_htons(pk_ccep->rport));
	if (cepid != NULL) {
		//cepid->appstate.tcp_callback = pk_ccep->callback;
	}

	return cepid;
}

int
tcp_snd_dat(struct uip_conn *cepid, T_IPV4EP *p_dstaddr, void *data, int len)
{
	UIP_OUT_BUF_T *buf = &uip_out_buf[uip_out_wpos];

	if (buf->len == 0) {
		buf->cepid.tcp = cepid;
		uip_ipaddr_copy(&buf->ripaddr, &p_dstaddr->ipaddr);
		buf->rport = uip_htons(p_dstaddr->portno);
		memcpy(&buf->data[UIP_IPTCPH_LEN + UIP_LLH_LEN], data, len);
		uip_out_buf[uip_out_wpos].len = len;
		uip_out_wpos++;
		if (uip_out_wpos >= UIP_BUF_COUNT)
			uip_out_wpos = 0;
#ifndef UIP_ACT_TSK_ONLY_ON_SEND
		act_tsk(UIP_TASK);
#endif
		return len;
	}

	return 0;
}

int
tcp_rcv_dat(struct uip_conn *cepid, T_IPV4EP *p_srcaddr, void *data, int len)
{
	int rlen;

	if (!uip_newdata())
		return 0;

	rlen = uip_datalen();
	if (rlen > len)
		rlen = len;

	uip_ipaddr_copy(&p_srcaddr->ipaddr, &((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])->srcipaddr);
	p_srcaddr->portno = uip_ntohs(((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])->srcport);
	memcpy(data, uip_appdata, rlen);

	return rlen;
}

void uip_appcall(void)
{
	uip_tcp_appstate_t *s = &uip_conn->appstate;
	//s->tcp_callback(uip_conn, 0, &s->parblk);
}
#if UIP_UDP
/*---------------------------------------------------------------------------*/
struct uip_udp_conn *
udp_acre_cep(const T_UDP_CCEP *pk_ccep)
{
	struct uip_udp_conn *cepid;

	cepid = uip_udp_new((uip_ipaddr_t *)&pk_ccep->ripaddr, uip_htons(pk_ccep->rport));
	if (cepid != NULL) {
		cepid->appstate.udp_callback = pk_ccep->callback;
		uip_udp_bind(cepid, uip_htons(pk_ccep->lport));
	}

	return cepid;
}

int
udp_snd_dat(struct uip_udp_conn *cepid, T_IPV4EP *p_dstaddr, void *data, int len)
{
	UIP_OUT_BUF_T *buf = &uip_out_buf[uip_out_wpos];

	if (buf->len == 0) {
		buf->cepid.udp = cepid;
		uip_ipaddr_copy(&buf->ripaddr, &p_dstaddr->ipaddr);
		buf->rport = uip_htons(p_dstaddr->portno);
		memcpy(&buf->data[UIP_IPUDPH_LEN + UIP_LLH_LEN], data, len);
		uip_out_buf[uip_out_wpos].len = len;
		uip_out_wpos++;
		if (uip_out_wpos >= UIP_BUF_COUNT)
			uip_out_wpos = 0;
#ifndef UIP_ACT_TSK_ONLY_ON_SEND
		act_tsk(UIP_TASK);
#endif
		return len;
	}

	return 0;
}

int
udp_rcv_dat(struct uip_udp_conn *cepid, T_IPV4EP *p_srcaddr, void *data, int len)
{
	int rlen;

	if (!uip_newdata())
		return 0;

	rlen = uip_datalen();
	if (rlen > len)
		rlen = len;

	uip_ipaddr_copy(&p_srcaddr->ipaddr, &((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])->srcipaddr);
	p_srcaddr->portno = uip_ntohs(((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])->srcport);
	memcpy(data, uip_appdata, rlen);

	return rlen;
}

void uip_udp_appcall(void)
{
	uip_udp_appstate_t *s = &uip_udp_conn->appstate;
	s->udp_callback(uip_udp_conn, 0, &s->parblk);
}
#endif
/*---------------------------------------------------------------------------*/
void
uip_log(char *m)
{
	printf("uIP log message: %s\n", m);
}
#ifdef __DHCPC_H__
void
dhcpc_configured(const struct dhcpc_state *s)
{
	sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) & ~0x04);
	uip_sethostaddr((uip_ipaddr_t *)&s->ipaddr);
	uip_setnetmask((uip_ipaddr_t *)&s->netmask);
	uip_setdraddr((uip_ipaddr_t *)&s->default_router);
	// resolv_conf(s->dnsaddr);
	DHCP4_CLI_SET_ADDR_CALLBACK();
}
#endif /* __DHCPC_H__ */
/*---------------------------------------------------------------------------*/
clock_time_t clock_time(void)
{
	return uiptsk.timer;
}
