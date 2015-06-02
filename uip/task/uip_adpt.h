/*
 *  TOPPERS ECHONET Lite Communication Middleware
 * 
 *  Copyright (C) 2014 Cores Co., Ltd. Japan
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

#ifndef __UIP_ADPT_H__
#define __UIP_ADPT_H__

#if defined __cplusplus
extern "C" {
#endif

/*
 *  IPv4 アドレス
 */

#define T_IN4_ADDR uip_ipaddr_t

/*
 *  ITRON TCP/IPv4 アドレス/ポート番号の定義
 */

typedef struct t_ipv4ep {
	T_IN4_ADDR	ipaddr;		/* IPv4アドレス		*/
	u16_t		portno;		/* ポート番号		*/
} T_IPV4EP;

#define MAKE_IPV4_ADDR(a,b,c,d) a,b,c,d

typedef struct t_tcp_ccep
{
	T_IN4_ADDR	ripaddr;		/* リモートIPv4アドレス		*/
	u16_t		rport;			/* リモートポート番号		*/
	u16_t		lport;			/* ローカルポート番号		*/
	void(*callback)(struct uip_conn *uip_conn, int fncd, void *p_parblk);
} T_TCP_CCEP;

struct uip_conn *tcp_acre_cep(const T_TCP_CCEP *pk_ccep);
int tcp_snd_dat(struct uip_conn *cepid, T_IPV4EP *p_dstaddr, void *data, int len);
int tcp_rcv_dat(struct uip_conn *cepid, T_IPV4EP *p_srcaddr, void *data, int len);

typedef struct t_udp_ccep
{
	T_IN4_ADDR	ripaddr;		/* リモートIPv4アドレス		*/
	u16_t		rport;			/* リモートポート番号		*/
	u16_t		lport;			/* ローカルポート番号		*/
	void (*callback)(struct uip_udp_conn *uip_udp_conn, int fncd, void *p_parblk);
} T_UDP_CCEP;

struct uip_udp_conn *udp_acre_cep(const T_UDP_CCEP *pk_ccep);
int udp_snd_dat(struct uip_udp_conn *cepid, T_IPV4EP *p_dstaddr, void *data, int len);
int udp_rcv_dat(struct uip_udp_conn *cepid, T_IPV4EP *p_srcaddr, void *data, int len);

ER uip_start();
char *ip2str(char *buf, const T_IN4_ADDR ipaddr);
ID uip_getid(struct uip_conn *conn);
struct uip_conn *uip_getconn(ID connid);

#if defined __cplusplus
}
#endif

#endif /* __UIP_ADPT_H__ */
