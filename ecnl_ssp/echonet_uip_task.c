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

/*
 *		ECHONET Lite UDP通信処理タスク
 */
#ifdef SUPPORT_UIP

#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <t_syslog.h>
#include <sil.h>
#include <uip.h>
#include "uip_adpt.h"

#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "echonet.h"
#include "echonet_task.h"
#include "echonet_lcl_task.h"
#include "echonet_dbg.h"

#ifndef ECHONET_UDP_TASK_GET_TIMER
#define ECHONET_UDP_TASK_GET_TIMER	TMO_FEVR
#endif /* ECHONET_UDP_TASK_GET_TIMER */

#ifndef ECHONET_UDP_TASK_PROGRESS
#define ECHONET_UDP_TASK_PROGRESS(timer)
#endif /* ECHONET_UDP_TASK_PROGRESS */

#ifndef ECHONET_UDP_TASK_TIMEOUT
#define ECHONET_UDP_TASK_TIMEOUT
#endif /* ECHONET_UDP_TASK_TIMEOUT */

#ifndef ECHONET_UDP_SEND_TO_MBX
#define ECHONET_UDP_SEND_TO_MBX
#endif /* ECHONET_UDP_SEND_TO_MBX */

#ifndef ECHONET_NODE_MATCH
#define ECHONET_NODE_MATCH(enodcb, edata, ipaddr, portno) is_match(enodcb, edata, ipaddr, portno)
#endif /* ECHONET_NODE_MATCH */

static T_IN4_ADDR localhost;
static T_IN4_ADDR multicast;
static T_IN4_ADDR zeroaddr;
struct uip_udp_conn *UDP_CEPID;

static ECN_ENOD_ID udp_get_id(T_EDATA *edata, const T_IN4_ADDR ipaddr, uint16_t portno);
static int udp_get_ip(T_IPV4EP *fp_ipep, ECN_ENOD_ID fa_enodid);
void _ecn_int_msg(ECN_FBS_ID fbs_id, ECN_FBS_SSIZE_T a_snd_len);
void _ecn_esv_msg(ECN_FBS_ID fbs_id);

static struct pt pt;
static struct uip_timer timer;

/*
 * 受信したUDPデータをMAILBOXに送る
 */
ER _ecn_udp2mbx(const uint8_t *buffer, size_t fa_len, const T_IPV4EP *dst);
ER _ecn_udp2mbx(const uint8_t *buffer, size_t fa_len, const T_IPV4EP *dst)
{
	ECN_FBS_ID	a_fbs_id = { 0 };
	ER			a_ret = E_OK;
	ECN_ENOD_ID	a_enod_id;
	union
	{
		const uint8_t			*buffer;
		const T_ECN_EDT_HDR		*t_esv;
	} a_rcv_pkt;

	a_rcv_pkt.buffer = buffer;
	if (	a_rcv_pkt.t_esv->ecn_hdr.ehd1 != ECN_EDH1_ECHONET_LITE	/* ECHONET Lite規格	*/
	||	a_rcv_pkt.t_esv->ecn_hdr.ehd2 != ECN_EDH2_FORMAT_1			/* 電文形式1		*/) {
		ECN_DBG_PUT "[UDP ECHO SRV] illegal type (0x%02X,0x%02X)", a_rcv_pkt.t_esv->ecn_hdr.ehd1, a_rcv_pkt.t_esv->ecn_hdr.ehd2 ECN_DBG_END;
		return E_PAR;
	}

#ifdef ECN_DBG_PUT_ENA
	_ecn_dbg_bindmp(buffer, fa_len);
#endif

	a_ret = _ecn_fbs_cre(fa_len, &a_fbs_id);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "[UDP ECHO SRV] _ecn_fbs_cre() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return a_ret;
	}
	a_ret = _ecn_fbs_add_data_ex(a_fbs_id, buffer, fa_len);
	if (a_ret) {
		ECN_DBG_PUT "[UDP ECHO SRV] _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		goto lb_except;
	}
	a_fbs_id.ptr->hdr.type = ECN_MSG_ECHONET;
	a_fbs_id.ptr->hdr.target = ENOD_LOCAL_ID;
	a_fbs_id.ptr->hdr.sender = ENOD_NOT_MATCH_ID;

	/* IPアドレスからリモートECHONETノードへ変換 */
	a_enod_id = udp_get_id((T_EDATA *)a_fbs_id.ptr, dst->ipaddr, dst->portno);
	if (a_enod_id < 0 || tnum_enodadr <= a_enod_id) {
		ECN_DBG_PUT "[UDP ECHO SRV] udp src(%s) echonet-node not found.",
			ip2str(NULL, dst->ipaddr) ECN_DBG_END;
	} else {
		/* 送信元ECHONETノードを記録 */
		a_fbs_id.ptr->hdr.sender = a_enod_id;
	}
	a_fbs_id.ptr->hdr.reply = a_fbs_id.ptr->hdr.sender;

	/* echonet_taskに送る */
	a_ret = psnd_dtq(ecn_svc_mailboxid, (intptr_t)a_fbs_id.ptr);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "[UDP ECHO SRV] psnd_dtq(ecn_svc_mailboxid) result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		goto lb_except;
	}
	return a_ret;
lb_except:
	_ecn_fbs_del(a_fbs_id);
	return a_ret;
}

/*
 *  ECHONET UDP通信処理タスク
 */
static
PT_THREAD(echonet_udp_task_pt(void))
{
	/* UDP出力領域（暫定） */
	static uint_t buffer[(UIP_BUFSIZE) / sizeof(uint_t) + 1];

	T_IN4_ADDR	a_src;
	ER			a_ret, a_ret2;
	ECN_FBS_SSIZE_T	a_snd_len;
	union
	{
		intptr_t	p_msg;
		ECN_FBS_ID	fbs_id;
	} a_mdt;
	static SYSTIM a_prev, a_now;
	TMO a_timer;
	T_UDP_CCEP ccep;

	PT_BEGIN(&pt);

	uip_ipaddr(localhost, 127, 0, 0, 1);
	uip_ipaddr(multicast, 224, 0, 23, 0);
	uip_ipaddr(zeroaddr, 0, 0, 0, 0);
	uip_ipaddr(ccep.ripaddr, 0, 0, 0, 0);
	ccep.rport = 0;
	ccep.lport = 3610;
	ccep.callback = callback_nblk_udp;

	uip_gethostaddr(a_src);
	ECN_DBG_PUT "[UDP TSK:%d] started, IP Address: %s ether_max_len: %u",
		ecn_udp_taskid,
		ip2str(NULL, a_src),
		UIP_BUFSIZE ECN_DBG_END;
	uip_ipaddr_copy(enodadrb_table[ENOD_LOCAL_ID].ipaddr, a_src);

	UDP_CEPID = udp_acre_cep(&ccep);
	if (UDP_CEPID == NULL) {
		ECN_DBG_PUT "udp_acre_cep()" ECN_DBG_END;
		PT_EXIT(&pt);
	}

	/* 周期ハンドラの起動 */
	a_ret = sta_cyc(ecn_udp_cychdrid);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "sta_cyc() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		PT_EXIT(&pt);
	}

	a_ret = act_tsk(ecn_svc_taskid);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "act_tsk() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		PT_EXIT(&pt);
	}

	a_ret = get_tim(&a_now);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "get_tim() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		PT_EXIT(&pt);
	}

	/* メッセージループ */
	for (;;) {
		a_prev = a_now;

		a_timer = ECHONET_UDP_TASK_GET_TIMER;
		timer_set(&timer, a_timer);

		PT_WAIT_UNTIL(&pt, (((a_ret = prcv_dtq(ecn_udp_mailboxid, &a_mdt.p_msg)) == E_OK)
			|| ((a_ret = timer_expired(&timer) ? E_TMOUT : E_WBLK) == E_TMOUT)));
		if ((a_ret != E_OK) && (a_ret != E_TMOUT)) {
			ECN_DBG_PUT "trcv_dtq() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
			PT_EXIT(&pt);
		}

		a_ret2 = get_tim(&a_now);
		if (a_ret2 != E_OK) {
			ECN_DBG_PUT "get_tim() result = %d:%s", a_ret2, itron_strerror(a_ret2) ECN_DBG_END;
			PT_EXIT(&pt);
		}

		a_timer = a_now - a_prev;
		ECHONET_UDP_TASK_PROGRESS(a_timer);

		if (a_ret == E_OK) {
			/* 送信データ長を取得 */
			a_snd_len = _ecn_fbs_get_datalen(a_mdt.fbs_id);

			ECN_DBG_PUT "[UDP TSK] trcv_dtq() mbx recv (%d byte)", a_snd_len ECN_DBG_END;

			if (0 < a_snd_len) {
				switch(a_mdt.fbs_id.ptr->hdr.type){
				case ECN_MSG_INTERNAL:
					_ecn_int_msg(a_mdt.fbs_id, a_snd_len);
					break;
				case ECN_MSG_ECHONET:
					_ecn_esv_msg(a_mdt.fbs_id);
					break;
				}
			}

			_ecn_fbs_del(a_mdt.fbs_id);
		}

		ECHONET_UDP_TASK_TIMEOUT;
	}

	PT_END(&pt);
}

/*
 *  ECHONET UDP通信処理プロトスレッド起動タスク
 */
void echonet_udp_task(intptr_t exinf)
{
	echonet_udp_task_pt();
}

/*
 *  ECHONET UDP通信処理タスク起動用周期ハンドラー
 */
void echonet_udp_cychdr(intptr_t exinf)
{
	(void)iact_tsk((ID)exinf);
}

/* 応答電文用fbs設定(sender/targetの設定) */
static ER _ecn_udp_cre_req_fbs(ID sender, uint8_t cmd, ECN_FBS_ID *pk_req)
{
	ER ret;
	ECN_FBS_ID req;

	ret = _ecn_fbs_cre(1, &req);
	if (ret != E_OK) {
		ECN_DBG_PUT "_ecn_udp_cre_req_fbs() : _ecn_fbs_cre() result = %d:%s", ret, itron_strerror(ret) ECN_DBG_END;
		return ret;
	}

	ret = _ecn_fbs_add_data(req, &cmd, sizeof(cmd));
	if (ret != E_OK) {
		_ecn_fbs_del(req);
		ECN_DBG_PUT "_ecn_udp_cre_req_fbs() : _ecn_fbs_add_data() result = %d:%s", ret, itron_strerror(ret) ECN_DBG_END;
		return ret;
	}

	req.ptr->hdr.type = ECN_MSG_INTERNAL;
	req.ptr->hdr.sender_mbxid = sender;
	req.ptr->hdr.target_mbxid = ecn_udp_mailboxid;
	req.ptr->hdr.reply_mbxid = sender;

	*pk_req = req;

	return E_OK;
}

/* 応答電文用fbs設定(sender/targetの設定) */
static ER _ecn_udp_cre_res_fbs(ECN_FBS_ID req, uint8_t cmd, ECN_FBS_ID *pk_res)
{
	ER ret;
	ECN_FBS_ID res;

	ret = _ecn_fbs_cre(1, &res);
	if (ret != E_OK) {
		ECN_DBG_PUT "_ecn_udp_cre_res_fbs() : _ecn_fbs_cre() result = %d:%s", ret, itron_strerror(ret) ECN_DBG_END;
		return ret;
	}

	ret = _ecn_fbs_add_data(res, &cmd, sizeof(cmd));
	if (ret != E_OK) {
		_ecn_fbs_del(res);
		ECN_DBG_PUT "_ecn_udp_cre_res_fbs() : _ecn_fbs_add_data() result = %d:%s", ret, itron_strerror(ret) ECN_DBG_END;
		return ret;
	}

	res.ptr->hdr.type = ECN_MSG_INTERNAL;
	res.ptr->hdr.sender_mbxid = ecn_udp_mailboxid;
	res.ptr->hdr.target_mbxid = req.ptr->hdr.reply_mbxid;
	res.ptr->hdr.reply_mbxid = ecn_udp_mailboxid;

	*pk_res = res;

	return E_OK;
}

/*
 *  内部メッセージ受信処理
 */
void _ecn_int_msg(ECN_FBS_ID fbs_id, ECN_FBS_SSIZE_T a_snd_len)
{
	ER result = E_OK, a_ret;
	ecn_udp_msg_get_ipaddr_req_t msg;
	ecn_udp_msg_get_ipaddr_error_t err;
	ECN_FBS_SSIZE_T len;
	ECN_FBS_ID buf;
	uint8_t cmd;

	a_ret = _ecn_fbs_get_data(fbs_id, &cmd, 1, &len);
	if (a_ret != E_OK) {
		ECN_DBG_PUT "[UDP TSK] _ecn_fbs_get_data() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return;
	}

	switch(cmd){
	// IPアドレスを返信
	case ECN_UDP_MSG_GET_IPADDR_REQ:
		if (a_snd_len < sizeof(msg)) {
			result = E_PAR;
			break;
		}

		a_snd_len = 0;
		a_ret = _ecn_fbs_get_data(fbs_id, &msg, sizeof(msg), &a_snd_len);
		if (a_ret != E_OK) {
			ECN_DBG_PUT "[UDP TSK] _ecn_fbs_get_data() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		}

		if ((msg.enodid < 0) && (msg.enodid >= tnum_enodadr)) {
			result = E_PAR;
			break;
		}
		
		a_ret = _ecn_udp_cre_res_fbs(fbs_id, ECN_UDP_MSG_GET_IPADDR_RES, &buf);
		if (a_ret != E_OK) {
			return;
		}

		a_ret = _ecn_fbs_add_data_ex(buf, &msg.requestid, offsetof(ecn_udp_msg_get_ipaddr_res_t, enodadrb));
		if (a_ret != E_OK) {
			_ecn_fbs_del(buf);
			ECN_DBG_PUT "_ecn_int_msg() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
			return;
		}

		a_ret = _ecn_fbs_add_data_ex(buf, &enodadrb_table[msg.enodid], sizeof(enodadrb_table[msg.enodid]));
		if (a_ret != E_OK) {
			_ecn_fbs_del(buf);
			ECN_DBG_PUT "_ecn_int_msg() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
			return;
		}

		a_ret = psnd_dtq(buf.ptr->hdr.target_mbxid, (intptr_t)buf.ptr);
		if (a_ret != E_OK) {
			_ecn_fbs_del(buf);
			ECN_DBG_PUT "_ecn_int_msg() : psnd_dtq() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
			return;
		}
		return;
	}

	a_ret = _ecn_udp_cre_res_fbs(fbs_id, ECN_UDP_MSG_GET_IPADDR_ERROR, &buf);
	if (a_ret != E_OK) {
		return;
	}

	err.requestid = msg.requestid;
	err.error = result;
	a_ret = _ecn_fbs_add_data_ex(buf, &err, sizeof(err));
	if (a_ret != E_OK) {
		_ecn_fbs_del(buf);
		ECN_DBG_PUT "_ecn_int_msg() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return;
	}

	a_ret = psnd_dtq(buf.ptr->hdr.target_mbxid, (intptr_t)buf.ptr);
	if (a_ret != E_OK) {
		_ecn_fbs_del(buf);
		ECN_DBG_PUT "_ecn_int_msg() : psnd_dtq() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return;
	}
}

/*
 *  ECHONET 電文受信処理
 */
void _ecn_esv_msg(ECN_FBS_ID fbs_id)
{
	/* UDP出力領域（暫定） */
	static uint_t buffer[(UIP_BUFSIZE) / sizeof(uint_t) + 1];
	T_IPV4EP a_dst;
	ER a_ret;
	ECN_FBS_SSIZE_T a_snd_len;
	static SYSTIM a_prev, a_now;

	uip_ipaddr(a_dst.ipaddr, 0,0,0,0);
	a_dst.portno = 0;
	/* 送信先IPアドレス */
	a_ret = udp_get_ip(&a_dst, fbs_id.ptr->hdr.target);
	if (!a_ret) {
		ECN_DBG_PUT "[UDP TSK] echonet-node 0x%02X-0x%02X-0x%02X → udp dest(%s)",
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx1,
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx2,
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx3,
			ip2str(NULL, a_dst.ipaddr) ECN_DBG_END;

		/* fbsから出力領域にデータを抽出 */
		a_snd_len = 0;
		a_ret = _ecn_fbs_get_data(fbs_id, buffer, sizeof(buffer), &a_snd_len);
		if (a_ret != E_OK) {
			ECN_DBG_PUT "[UDP TSK] _ecn_fbs_get_data() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		}
		else if(uip_ipaddr_cmp(a_dst.ipaddr, localhost)) {
			/* 送信先が127.0.0.1 → mbxに転送 */
			ECN_DBG_PUT "redirect ecn_udp_mailboxid → ecn_svc_mailboxid (esv:0x%02X)",
				((T_EDATA *)fbs_id.ptr)->hdr.edata.esv ECN_DBG_END;

			a_ret = _ecn_udp2mbx((const uint8_t *)buffer, a_snd_len, &a_dst);
			if (a_ret != E_OK) {
				syslog(LOG_WARNING, "_ecn_esv_msg() : _ecn_udp2mbx() result = %d:%s", a_ret, itron_strerror(a_ret));
			}
		}
		else {
			ECN_DBG_PUT "[UDP TSK] udp_snd_dat() to:%s %ubyte(s)",
				ip2str(NULL, a_dst.ipaddr), a_snd_len ECN_DBG_END;
#ifdef ECN_DBG_PUT_ENA
			_ecn_dbg_bindmp((const uint8_t *)buffer, a_snd_len);
#endif
			/* UDP送信 */
			a_ret = udp_snd_dat(UDP_CEPID, &a_dst, buffer, a_snd_len);
			if (a_ret < 0) {
				ECN_DBG_PUT "[UDP TSK] send, error: %s", itron_strerror(a_ret) ECN_DBG_END;
			}
		}

		/* データが長すぎて1パケットに収まらなかった場合 */
		if (_ecn_fbs_exist_data(fbs_id)) {
			ECN_DBG_PUT "[UDP TSK] send, data so long: %dbyte(s)", _ecn_fbs_get_datalen(fbs_id) ECN_DBG_END;
		}
	} else {
		ECN_DBG_PUT "[UDP TSK] echonet-node 0x%02X-0x%02X-0x%02X not found.",
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx1,
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx2,
			((T_EDATA *)fbs_id.ptr)->hdr.edata.deoj.eojx3 ECN_DBG_END;
	}
}

static struct ecnl_state s;

/*
 *  ノンブロッキングコールのコールバック関数
 */
static
PT_THREAD(callback_nblk_udp_pt(void))
{
	/* void *p_parblk = uip_buf; */
	static uint8_t buffer[(UIP_BUFSIZE) / sizeof(uint_t) + 1];
	T_IPV4EP dst;
	int a_len;

	PT_BEGIN(&s.pt);

	/* ECN_CAP_PUT "[UDP ECHO SRV] callback_nblk_udp() recv: %u", *(int *)p_parblk ECN_CAP_END; */
	memset(buffer, 0, sizeof(buffer));

	PT_WAIT_UNTIL(&s.pt, ((a_len = udp_rcv_dat(UDP_CEPID, &dst, buffer, sizeof(buffer) - 1)) != 0));
	if(a_len < 0) {
		syslog(LOG_WARNING, "[UDP ECHO SRV] recv, error: %s", itron_strerror(a_len));
		PT_EXIT(&s.pt);
	}
#ifdef _MSC_VER
	/* (開発用) ECHONET用パケット以外は読み捨てる */
	if (buffer[0] != 0x10 || buffer[1] != 0x81) {
		PT_EXIT(&s.pt);
	}
#endif
	ECN_CAP_PUT "[UDP ECHO SRV] recv, addr: %s:%d, len: %d, msg: '%s'",
			ip2str(NULL, dst.ipaddr), dst.portno, a_len, buffer ECN_CAP_END;
	if ((int)sizeof(buffer) <= a_len)
		a_len = (int)sizeof(buffer) - 1;
	if (0 < a_len) {
		ECHONET_UDP_SEND_TO_MBX;

		_ecn_udp2mbx(buffer, a_len, &dst);
	}

	PT_END(&s.pt);
}

ER callback_nblk_udp(struct uip_udp_conn *uip_udp_conn, int fncd, void *p_parblk)
{
	callback_nblk_udp_pt();

	return E_OK;
}

void callback_nblk_tcp(void)
{
}

/*
 *  リモートECHONETノードの適合確認
 */
bool_t is_match(const EOBJCB *enodcb, T_EDATA *edata, const void *_ipaddr, uint16_t portno)
{
	/*T_IN4_ADDR *ipaddr = (T_IN4_ADDR *)_ipaddr;*/
	ER			ret;
	T_ENUM_EPC	enm;
	uint8_t		epc;
	uint8_t		pdc;
	uint8_t		p_edt[256];
	int			i, j, k;
	int			count;
	const EOBJINIB	*p_eobj;
	bool_t		match;

	if (!edata)
		return false;

	/* ノードスタート時インスタンスリスト通知以外は除外 */
	if (	edata->hdr.edata.esv != ESV_INF
		||	edata->hdr.edata.deoj.eojx1 != EOJ_X1_PROFILE
		||	edata->hdr.edata.deoj.eojx2 != EOJ_X2_NODE_PROFILE
		||	edata->hdr.edata.deoj.eojx3 != 0x01
		||	edata->hdr.edata.seoj.eojx1 != EOJ_X1_PROFILE
		||	edata->hdr.edata.seoj.eojx2 != EOJ_X2_NODE_PROFILE
		||	(	edata->hdr.edata.seoj.eojx3 != 0x01
			&&	edata->hdr.edata.seoj.eojx3 != 0x02)) {
		return false;
	}

	ret = ecn_itr_ini(&enm, edata);
	if (ret) {
		syslog(LOG_WARNING, "is_match(): ecn_itr_ini() result = %d:%s", ret, itron_strerror(ret));
		return false;
	}
	while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
		if (enm.is_eof) {
			break;
		}
		ECN_DBG_PUT "is_match(): ecn_itr_nxt() result: epc=0x%02X, pdc=%d", epc, pdc ECN_DBG_END;
		/* インスタンスリスト通知または自ノードインスタンスリストＳ以外は除外 */
		if ((epc != 0xD5) && (epc != 0xD6)) {
			continue;
		}

		/* ２バイト目以降にeojが列挙されている */
		count = (pdc - 1) / sizeof(T_ECN_EOJ);

		/* ノード内の機器オブジェクトを検索 */
		for (k = 0; k < enodcb->eobjcnt; k++) {
			p_eobj = enodcb->eobjs[k];

			/* インスタンスリストを確認 */
			match = false;
			for (i = 0, j = 1; i < count; i++, j += sizeof(T_ECN_EOJ)) {
				if (p_eobj->eojx1 != p_edt[j])
					continue;
				if (p_eobj->eojx2 != p_edt[j + 1])
					continue;
				if (p_eobj->eojx3 != p_edt[j + 2])
					continue;

				match = true;
				break;
			}

			if (!match)
				return false;
		}

		/* すべて揃っていたら適合（インスタンスリストの方が多くてもいいこととする） */
		return true;
	}

	return false;
}

/*
 *  IPアドレスからリモートECHONETノードへ変換
 */
ECN_ENOD_ID udp_get_id(T_EDATA *edata, const T_IN4_ADDR ipaddr, uint16_t portno)
{
	T_ENOD_ADDR	*ea;
	int			i;

	if (uip_ipaddr_cmp(ipaddr, localhost))
		return ENOD_LOCAL_ID;
	if (uip_ipaddr_cmp(ipaddr, multicast))
		return ENOD_MULTICAST_ID;

	/* IPアドレスの同じものを検索 */
	for (i = 0, ea = enodadrb_table; i < tnum_enodadr; i++, ea++) {
		if (!ea->inuse)
			continue;
		if (!uip_ipaddr_cmp(ea->ipaddr, ipaddr))
			continue;

		ECN_CAP_PUT "udp_get_id(): ip-found remote(%d) = %s",
			i - ENOD_REMOTE_ID, ip2str(NULL, ipaddr) ECN_CAP_END;
		return (ECN_ENOD_ID)i;
	}

	/* 対応するリモートノードを検索 */
	for (i = ENOD_REMOTE_ID, ea = &enodadrb_table[ENOD_REMOTE_ID]; i < tnum_enodadr; i++, ea++) {
		if (!ea->inuse)
			continue;
		if ((i - ENOD_REMOTE_ID + 1) >= tnum_enodid)
			break;
		if (uip_ipaddr_cmp(ea->ipaddr, zeroaddr))
			continue;
		if (!ECHONET_NODE_MATCH(&eobjcb_table[i - ENOD_REMOTE_ID + 1], edata, ipaddr, portno))
			continue;

		/* 対応するリモートノードがあればIPアドレスを設定 */
		uip_ipaddr_copy(ea->ipaddr, ipaddr);

		ECN_CAP_PUT "udp_get_id(): enod-found remote(%d) = %s",
			i - ENOD_REMOTE_ID, ip2str(NULL, ipaddr) ECN_CAP_END;
		return (ECN_ENOD_ID)i;
	}

	/* 空き領域を探して自動登録 */
	for (i = ENOD_REMOTE_ID, ea = &enodadrb_table[ENOD_REMOTE_ID]; i < tnum_enodadr; i++, ea++) {
		if (ea->inuse)
			continue;

		ea->inuse = true;
		uip_ipaddr_copy(ea->ipaddr, ipaddr);

		ECN_CAP_PUT "udp_get_id(): empty-found remote(%d) = %s",
			i - ENOD_REMOTE_ID, ip2str(NULL, ipaddr) ECN_CAP_END;
		return (ECN_ENOD_ID)i;
	}

	return ENOD_NOT_MATCH_ID;
}

/*
 *  リモートECHONETノードからIPアドレスへ変換
 */
int udp_get_ip(T_IPV4EP *fp_ipep, ECN_ENOD_ID fa_enodid)
{
	T_ENOD_ADDR *ea;

	if (!fp_ipep)
		return -1;	/* NG */

	fp_ipep->portno = 3610;

	if (fa_enodid == ENOD_MULTICAST_ID) {
		/* targetがENOD_MULTICAST_IDの場合、マルチキャストを行う */
		uip_ipaddr_copy(fp_ipep->ipaddr, multicast);
		return 0;	/* ok */
	}

	if (fa_enodid < ENOD_REMOTE_ID) {
		/* targetが未定義・LOCAL・APIの場合、ローカル配送を行う */
		uip_ipaddr_copy(fp_ipep->ipaddr, localhost);
		return 0;	/* ok */
	}

	if (fa_enodid >= tnum_enodadr)
		return -1;	/* NG */

	ea = &enodadrb_table[fa_enodid];
	if (!ea->inuse)
		return -1;	/* NG */

	if (uip_ipaddr_cmp(ea->ipaddr, zeroaddr))
		return -1;	/* NG */

	uip_ipaddr_copy(fp_ipep->ipaddr, ea->ipaddr);
	return 0;	/* ok */
}

ER ecn_udp_get_ipaddr(ID sender, int requestid, ECN_ENOD_ID enodid, ECN_FBS_ID *pk_req)
{
	ER a_ret;
	ECN_FBS_ID req;

	a_ret = _ecn_udp_cre_req_fbs(sender, ECN_UDP_MSG_GET_IPADDR_REQ, &req);
	if (a_ret != E_OK) {
		return a_ret;
	}

	a_ret = _ecn_fbs_add_data_ex(req, &requestid, sizeof(((ecn_udp_msg_get_ipaddr_res_t *)0)->requestid));
	if (a_ret != E_OK) {
		_ecn_fbs_del(req);
		ECN_DBG_PUT "ecn_udp_get_ipaddr() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return a_ret;
	}

	a_ret = _ecn_fbs_add_data_ex(req, &enodid, sizeof(((ecn_udp_msg_get_ipaddr_res_t *)0)->enodid));
	if (a_ret != E_OK) {
		_ecn_fbs_del(req);
		ECN_DBG_PUT "ecn_udp_get_ipaddr() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return a_ret;
	}

	a_ret = _ecn_fbs_add_data_ex(req, &enodadrb_table[enodid], sizeof(((ecn_udp_msg_get_ipaddr_res_t *)0)->enodadrb));
	if (a_ret != E_OK) {
		_ecn_fbs_del(req);
		ECN_DBG_PUT "ecn_udp_get_ipaddr() : _ecn_fbs_add_data_ex() result = %d:%s", a_ret, itron_strerror(a_ret) ECN_DBG_END;
		return a_ret;
	}

	*pk_req = req;

	return E_OK;
}

char *ipaddr2str(char *buf, int bubsz, uint8_t *ipaddr)
{
	return ip2str(buf, *(T_IN4_ADDR *)ipaddr);
}

#endif
