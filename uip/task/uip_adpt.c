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

#include <kernel.h>
#include "net/ip/uip.h"
#include "uip_adpt.h"
#include "t_syslog.h"
#include "kernel_cfg.h"

/*
 *  uip_taskを起動
 */
ER uip_start()
{
	ER ret;

	/* uip_task用周期ハンドラの起動 */
	ret = sta_cyc(UIP_CYCHDR);
	if (ret != E_OK) {
		syslog(LOG_DEBUG, "sta_cyc() result = %d", ret);
		return ret;
	}

	return E_OK;
}

/*
 *  convert_hexdigit -- 16進数→文字列変換
 */
int_t convert_hexdigit (char *buf, uint_t val, int_t radix, int_t width, char padchar)
{
	static const char radhex[] = "0123456789abcdef";

	char	digits[11], *start;
	int_t	ix, pad;

	ix = 0;
	start = buf;
	do {
		digits[ix ++] = radhex[(val % radix) & 0x0f];
		val /= radix;
	} while (val != 0);
	for (pad = ix; pad < width; pad ++)
		*buf ++ = padchar;
	while (ix -- > 0)
		*buf ++ = digits[ix];
	*buf = '\0';
	return (int_t)(buf - start);
}

/*
 *  ip2str -- IPv4 アドレスを文字列に変換する。
 */
char *ip2str(char *buf, const T_IN4_ADDR ipaddr)
{
	char *start;

	start = buf;
	buf += convert_hexdigit(buf, (uint_t)((ipaddr[0] >> 0) & 0xff), 10, 0, ' ');
	*(buf ++) = '.';
	buf += convert_hexdigit(buf, (uint_t)((ipaddr[0] >> 8) & 0xff), 10, 0, ' ');
	*(buf ++) = '.';
	buf += convert_hexdigit(buf, (uint_t)((ipaddr[1] >> 0) & 0xff), 10, 0, ' ');
	*(buf ++) = '.';
	buf += convert_hexdigit(buf, (uint_t)((ipaddr[1] >> 8) & 0xff), 10, 0, ' ');
	*buf = '\0';

	return start;
}

ID uip_getid(struct uip_conn *conn)
{
	int index;

	index = ((intptr_t)conn - (intptr_t)uip_conns) / sizeof(uip_conns[0]);
	if ((index < 0) || (index >= sizeof(uip_conns) / sizeof(uip_conns[0])))
		return -1;

	return index + 1;
}

struct uip_conn *uip_getconn(ID connid)
{
	int index = connid - 1;

	if ((index < 0) || (index >= sizeof(uip_conns) / sizeof(uip_conns[0])))
		return NULL;

	return &uip_conns[index];
}
