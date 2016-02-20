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
 *  サンプルプログラム(1)の本体
 */

#include <kernel.h>
#include <t_syslog.h>
#include <sil.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "main.h"
#include "echonet_cfg.h"
#ifdef __RX
#include "rx630_ccrx/rx630.h"
#else
#include "rx630_msvc/rx630.h"
#endif
#include <net/ip/uip.h>
#include <sys/pt.h>
#include "uip_adpt.h"
#include "wamp.h"
// TODO:コントローラ向けヘッダーファイルを作成する
#include "echonet_task.h"
#include "echonet_agent.h"
#include "echonet_lcl_task.h"
#include "arduino.h"
#include "data_flash.h"

uint8_t mac_addr[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xB8};
bool_t dhcp_enable = true;

#define MAKER_CODE	0x00, 0x00, 0xB3	/* TOPPERSプロジェクト */

/* ノードプロファイルオブジェクト */
struct node_profile_object_t local_node_data = {
	0x30,	/* 動作状態 */
	{ 0x01, 0x0A, 0x01, 0x00 },	/* Ｖｅｒｓｉｏｎ情報 */
	{
		0xFE,	/* 下位通信層IDフィールド */
		{ MAKER_CODE },	/* メーカーコード */
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	/* ユニークID部(メーカー独自) */
	},
	0x0000,	/* 異常内容 */
	{ MAKER_CODE },	/* メーカーコード */
};

/* コントローラークラス */
struct controller_t controller_class_data = {
	0x30,	/* 動作状態 */
	0x00,	/* 設置場所 */
	{ 0x00, 0x00, 'C', 0x00 },	/* 規格Ｖｅｒｓｉｏｎ情報 */
	0x41,	/* 異常発生状態 */
	{ MAKER_CODE },	/* メーカーコード */
};

/*
 * 動作状態ON/OFF設定関数（0x30, 0x31のみ受け付け）
 */
int onoff_prop_set(const EPRPINIB *item, const void *src, int size, bool_t *anno)
{
	/* サイズが１以外は受け付けない */
	if (size != 1)
		return 0;

	*anno = *((uint8_t*)item->exinf) != *((uint8_t*)src);

	switch (*(uint8_t *)src) {
	/* ONの場合 */
	case 0x30:
		*((uint8_t *)item->exinf) = *((uint8_t *)src);
		/* LEDの"."をON */
		sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) | 0x40);
		break;
	/* OFFの場合 */
	case 0x31:
		*((uint8_t *)item->exinf) = *((uint8_t *)src);
		/* LEDの"."をOFF */
		sil_wrb_mem((uint8_t *)0x0008C02A, sil_reb_mem((uint8_t *)0x0008C02A) & ~0x40);
		break;
	/* 0x30か0x31以外は受け付けない */
	default:
		return 0;
	}

	return 1;
}

/*
 * 異常内容設定関数
 */
int node_profile_object_fault_content_set(const EPRPINIB *item, const void *src, int size, bool_t *anno)
{
	/* サイズが2以外は受け付けない */
	if (size != 2)
		return 0;

	if ((*(uint16_t *)src >= 0x0) && (*(uint16_t *)src <= 0x3ec)) {
		*((uint16_t *)item->exinf) = *((uint16_t *)src);
		/* TODO: このの場合の処理*/
	}
	/* 上記以外は受け付けない */
	else {
		return 0;
	}

	return 2;
}

/*
 * 異常発生状態設定関数（0x41, 0x42のみ受け付け）
 */
int alarm_prop_set(const EPRPINIB *item, const void *src, int size, bool_t *anno)
{
	/* サイズが１以外は受け付けない */
	if (size != 1)
		return 0;

	*anno = *((uint8_t *)item->exinf) != *((uint8_t *)src);

	switch (*(uint8_t *)src) {
	/* 異常発生ありの場合 */
	case 0x41:
	/* 異常発生なしの場合 */
	case 0x42:
		*((uint8_t *)item->exinf) = *((uint8_t *)src);
		break;
	/* 0x41か0x42以外は受け付けない */
	default:
		return 0;
	}

	return 1;
}

/*
 * 現在時刻設定関数
 */
int time_prop_set(const EPRPINIB *item, const void *src, int size, bool_t *anno)
{
	uint8_t *p_src;

	if (size != 2)
		return 0;

	/* 時刻設定 */
	p_src = (uint8_t *)src;
	sil_wrb_mem((uint8_t *)RTC_RHRCNT_ADDR, *p_src++);
	sil_wrb_mem((uint8_t *)RTC_RMINCNT_ADDR, *p_src++);
	sil_wrb_mem((uint8_t *)RTC_RSECCNT_ADDR, 0x00);

	return (intptr_t)p_src - (intptr_t)src;
}

/*
 * 現在時刻取得関数
 */
int time_prop_get(const EPRPINIB *item, void *dst, int size)
{
	uint8_t *p_dst;

	if (size != 2)
		return 0;

	/* 時刻設定 */
	p_dst = (uint8_t *)dst;
	*p_dst++ = sil_reb_mem((uint8_t *)RTC_RHRCNT_ADDR);
	*p_dst++ = sil_reb_mem((uint8_t *)RTC_RMINCNT_ADDR);

	return (intptr_t)p_dst - (intptr_t)dst;
}

/*
 * 現在年月日設定関数
 */
int date_prop_set(const EPRPINIB *item, const void *src, int size, bool_t *anno)
{
	uint8_t *p_src;

	if (size != 4)
		return 0;

	/* 年月日設定 */
	p_src = (uint8_t *)src;
	p_src++;	/* 20XX */
	sil_wrb_mem((uint8_t *)RTC_RYRCNT_ADDR, *p_src++);
	sil_wrb_mem((uint8_t *)RTC_RMONCNT_ADDR, *p_src++);
	sil_wrb_mem((uint8_t *)RTC_RDAYCNT_ADDR, *p_src++);

	return (intptr_t)p_src - (intptr_t)src;
}

/*
 * 現在年月日取得関数
 */
int date_prop_get(const EPRPINIB *item, void *dst, int size)
{
	uint8_t *p_dst;

	if (size != 4)
		return 0;

	p_dst = (uint8_t *)dst;
	*p_dst++ = 0x20;
	*p_dst++ = sil_reb_mem((uint8_t *)RTC_RYRCNT_ADDR);
	*p_dst++ = sil_reb_mem((uint8_t *)RTC_RMONCNT_ADDR);
	*p_dst++ = sil_reb_mem((uint8_t *)RTC_RDAYCNT_ADDR);

	return (intptr_t)p_dst - (intptr_t)dst;
}

/*
 *  uIP タスク初期化処理
 */
void uip_task_init(intptr_t exinf)
{
	httpd_init();
}

static void main_initialize();
static TMO main_get_timer();
static void main_progress(TMO interval);
static void main_recv_esv(T_EDATA *esv);
static bool_t main_wbs_msg(ECN_FBS_ID msg);
static bool_t main_wbs_que_msg();
static void main_int_msg(ECN_FBS_ID msg);
static void main_break_wait(uint8_t *brkdat, int32_t len);
static void main_timeout();
static void main_start_service();
static void main_get_device_list_res(ECN_FBS_ID msg);
static void main_get_ipaddr_res(ECN_FBS_ID msg);
static void main_get_device_info_res(ECN_FBS_ID msg);
static void main_kadecot_set_res(T_EDATA *esv);
static void main_kadecot_get_res(T_EDATA *esv);
static void main_ecnl_set_res(T_EDATA *esv);
static void main_ecnl_get_res(T_EDATA *esv);
static void main_ecnl_setget_res(T_EDATA *esv);
static void main_publish(T_EDATA *esv);

enum main_state_t {
	main_state_start,
	main_state_idle,
};

TMO main_timer = TMO_FEVR;
enum main_state_t main_state = main_state_start;
struct pt main_pt;
struct uip_timer main_pt_timer;
T_ECN_FBS_QUEUE wbs_queue;

/*
 *  メインタスク
 */
static
PT_THREAD(main_task_pt(void))
{
	ER ret, ret2;
	static SYSTIM prev, now;
	TMO timer;
	T_EDATA *esv;
	uint8_t brkdat[64];
	int32_t len;

	PT_BEGIN(&main_pt);

	/* 初期化 */
	if (main_state == main_state_start)
		main_initialize();

	ret2 = get_tim(&now);
	if (ret2 != E_OK) {
		syslog(LOG_ERROR, "get_tim");
		PT_EXIT(&main_pt);
	}

	for (;;) {
		prev = now;

		/* タイマー取得 */
		timer = main_get_timer();
		timer_set(&main_pt_timer, timer);

		/* 応答電文待ち */
		PT_WAIT_UNTIL(&main_pt, (((ret = ecn_prcv_esv(&esv)) == E_OK) || (ret == E_BRK)
			|| ((ret = timer_expired(&main_pt_timer) ? E_TMOUT : E_WBLK) == E_TMOUT)));
		if ((ret != E_OK) && (ret != E_BRK) && (ret != E_TMOUT)) {
			syslog(LOG_ERROR, "ecn_trcv_esv");
			PT_EXIT(&main_pt);
		}

		ret2 = get_tim(&now);
		if (ret2 != E_OK) {
			syslog(LOG_ERROR, "get_tim");
			PT_EXIT(&main_pt);
		}

		/* 時間経過 */
		main_progress(now - prev);

		/* Echonet電文受信の場合 */
		if (ret == E_OK) {
			/* Echonet電文受信処理 */
			main_recv_esv(esv);

			/* 領域解放 */
			ret = ecn_rel_esv(esv);
			if (ret != E_OK) {
				syslog(LOG_ERROR, "ECNL ecn_rel_esv result = %d", ret);
				PT_EXIT(&main_pt);
			}
		}
		/* 応答電文待ちの割り込みの場合 */
		else if (ret == E_BRK) {
			ECN_FBS_ID msg = {(T_ECN_FST_BLK *)esv};
			bool_t rel_msg = true;
			switch (msg.ptr->hdr.type) {
			case 0:
				rel_msg = main_wbs_msg(msg);
				break;
			case 1/*ECN_MSG_INTERNAL*/:
				main_int_msg(msg);
				break;
			case 3/*ECN_MSG_USER_BREAK*/:
				/* 応答電文待ちの割り込みデータ取得 */
				ret = ecn_get_brk_dat(esv, brkdat, sizeof(brkdat), &len);
				if (ret != E_OK) {
					syslog(LOG_ERROR, "ecn_get_brk_dat");
					PT_EXIT(&main_pt);
				}

				/* 応答電文待ちの割り込み処理 */
				main_break_wait(brkdat, len);
				break;
			}

			/* 領域解放 */
			if (rel_msg) {
				ret = ecn_rel_esv(esv);
				if (ret != E_OK) {
					syslog(LOG_ERROR, "BRK ecn_rel_esv msg = %p, type = %d, result = %d", esv, msg.ptr->hdr.type, ret);
					PT_EXIT(&main_pt);
				}
			}
		}

		/* タイムアウト処理 */
		main_timeout();

		/* キューに溜まったメッセージを処理 */
		while (main_wbs_que_msg());
	}

	PT_END(&main_pt);
}

void main_task(intptr_t exinf)
{
	main_task_pt();
}

void main_task_cychdr(intptr_t exinf)
{
	(void)iact_tsk((ID)exinf);
}

typedef struct gpio_state {
	bool_t state;
	int count;
} gpio_state_t;

gpio_state_t main_gpio[2/*btn1, btn2*/ + 14/*arduino pin*/];

#define MAIN_GPIO_BTN1 0
#define MAIN_GPIO_BTN2 1
#define MAIN_GPIO_ARDUINO_PIN0 2
#define MAIN_GPIO_COUNT (sizeof(main_gpio) / sizeof(main_gpio[0]))

typedef struct analog_state {
	int subscriptionId;
	int pinno;
	bool_t state;
	int count;
	uint16_t threshold;
} analog_state_t;

analog_state_t main_analog[16];

#define MAIN_ANALOG_COUNT (sizeof(main_analog) / sizeof(main_analog[0]))

int main_gpio_pubid;

enum request_info_state_t {
	request_info_state_idle,
	request_info_state_search_device,
	request_info_state_get_device_list,
	request_info_state_get_ipaddr,
	request_info_state_get_device_info,
	request_info_state_kadecot_set,
	request_info_state_kadecot_get,
	request_info_state_ecnl_set,
	request_info_state_ecnl_get,
	request_info_state_ecnl_setget,
};

typedef struct request_info {
	enum request_info_state_t state;
	TMO timer;
	unsigned int requestId;
	struct wamp_dealer *dealer;
	ID eobjid;
	uint8_t epc;
} request_info_t;

request_info_t request_infos[1];
wamp_state_t wamp;

#define MAIN_REQUEST_INFOS_COUNT (sizeof(request_infos) / sizeof(request_infos[0]))

/*
 * 初期化
 */
static void main_initialize()
{
	extern uint8_t my_ip[4];
	extern uint8_t my_netmask[4];
	extern uint8_t my_default_router[4];
	uint8_t btn1, btn2;
	uint8_t data[32], c;
	int i, j;
	bool_t dflt_addr = true;
	ER ret;

	/* LEDを"000"と表示 */
	sil_wrb_mem((uint8_t *)0x0008C02A, 0x00);

	ret = data_flash_init();
	while (ret == E_OK) {
		ret = data_flash_read(0, data);
		if (ret != E_OK) {
			break;
		}

		dflt_addr = false;
		memcpy(mac_addr, data, sizeof(mac_addr));
		dhcp_enable = data[sizeof(mac_addr)] != 0;
		memcpy(my_ip, &data[sizeof(mac_addr) + 1], sizeof(my_ip));
		memcpy(my_netmask, &data[sizeof(mac_addr) + 5], sizeof(my_netmask));
		memcpy(my_default_router, &data[sizeof(mac_addr) + 9], sizeof(my_default_router));
		break;
	}

	for (i = 0, j = 0; i < sizeof(mac_addr); i++) {
		c = mac_addr[i] >> 4;
		data[j++] = (c < 10) ? ('0' + c) : ('A' - 10 + c);
		c = mac_addr[i] & 0xF;
		data[j++] = (c < 10) ? ('0' + c) : ('A' - 10 + c);
		data[j++] = ':';
	}
	data[--j] = '\0';

	syslog(LOG_INFO, "mac_addr %s %s %s", data, dflt_addr ? "default" : "flash-rom", dhcp_enable ? "dhcp" : "static");

	/* uIPを開始 */
	uip_start();

	wamp_init(&wamp);

	/* 10ms後にボタン状態を確認 */
	main_state = main_state_start;
	main_timer = TMO_FEVR;

	/* Arduino互換機能初期化 */
	arduino_init();

	/* ボタン状態読み込み */
	btn1 = sil_reb_mem((uint8_t *)0x0008C04A);
	/*btn1 = sil_reb_mem((uint8_t *)0x0008C040);*/
	btn2 = sil_reb_mem((uint8_t *)0x0008C040);
	main_gpio[MAIN_GPIO_BTN1].state = (btn1 & 0x80/*0x0x20*/) != 0;
	main_gpio[MAIN_GPIO_BTN2].state = (btn2 & 0x80) != 0;
	for (i = MAIN_GPIO_ARDUINO_PIN0; i < MAIN_GPIO_COUNT; i++) {
		arduino_digital_value_t dval;
		arduino_digitalRead(i - MAIN_GPIO_ARDUINO_PIN0, &dval);
		main_gpio[i].state = dval == ARDUINO_DIGITAL_VALUE_HIGH;
	}

	for (i = 0; i < MAIN_ANALOG_COUNT; i++) {
		main_analog[i].state = true; /* たぶんプルアップされている */
	}

	for (i = 0; i < MAIN_REQUEST_INFOS_COUNT; i++) {
		request_infos[i].timer = TMO_FEVR;
	}
}

/*
 * タイマー取得
 */
static TMO main_get_timer()
{
	int i;
	TMO timer = main_timer, temp;

	for (i = 0; i < sizeof(request_infos) / sizeof(request_infos[0]); i++) {
		temp = request_infos[i].timer;
		if (temp != TMO_FEVR) {
			if ((timer == TMO_FEVR) || (temp < timer)) {
				timer = temp;
			}
		}
	}

	return timer;
}

/*
 * 時間経過
 */
static void main_progress(TMO interval)
{
	int i;
	TMO temp;

	if (main_timer != TMO_FEVR) {
		main_timer -= interval;
		if (main_timer < 0) {
			main_timer = 0;
		}
	}

	for (i = 0; i < sizeof(request_infos) / sizeof(request_infos[0]); i++) {
		temp = request_infos[i].timer;
		if (temp != TMO_FEVR) {
			temp -= interval;
			if (temp < 0) {
				temp = 0;
			}
			request_infos[i].timer = temp;
		}
	}
}

static void main_self_msg(T_EDATA *esv);

/*
 * Echonet電文受信処理
 */
static void main_recv_esv(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];

	switch (esv->hdr.edata.esv) {
	case ESV_SET_RES:
	case ESV_SET_C_SNA:
		if (request_info->eobjid != ecn_get_eobj(esv)) {
			main_self_msg(esv);
			break;
		}

		switch (request_info->state) {
		case request_info_state_kadecot_set:
			main_kadecot_set_res(esv);
			break;
		case request_info_state_ecnl_set:
			main_ecnl_set_res(esv);
			break;
		}
		break;
	case ESV_GET_RES:
	case ESV_GET_SNA:
		if (request_info->eobjid != ecn_get_eobj(esv)) {
			main_self_msg(esv);
			break;
		}

		switch (request_info->state) {
		case request_info_state_kadecot_get:
			main_kadecot_get_res(esv);
			break;
		case request_info_state_ecnl_get:
			main_ecnl_get_res(esv);
			break;
		}
		break;
	case ESV_SET_GET_RES:
	case ESV_SET_GET_SNA:
		if (request_info->eobjid != ecn_get_eobj(esv)) {
			main_self_msg(esv);
			break;
		}

		switch (request_info->state) {
		case request_info_state_ecnl_setget:
			main_ecnl_setget_res(esv);
			break;
		}
		break;
	case ESV_INF:
		main_self_msg(esv);
		main_publish(esv);
		break;
	default:
		main_self_msg(esv);
		break;
	}
}

/*
 * WebSocketメッセージ受信処理
 */
static bool_t main_wbs_msg(ECN_FBS_ID msg)
{
	request_info_t *request_info = &request_infos[0];

	if (request_info->state != request_info_state_idle) {
		ecn_fbs_enqueue(&wbs_queue, msg.ptr);
		return false;
	}

	wamp_put_msg(&wamp, msg, ((ID *)msg.ptr->_gap)[0]);
	return true;
}

static bool_t main_wbs_que_msg()
{
	request_info_t *request_info = &request_infos[0];
	ECN_FBS_ID msg;
	ER ret;

	if (request_info->state != request_info_state_idle)
		return false;

	ret = ecn_fbs_dequeue(&wbs_queue, &msg.ptr);
	if (ret == E_TMOUT)
		return false;

	wamp_put_msg(&wamp, msg, ((ID *)msg.ptr->_gap)[0]);

	_ecn_fbs_del(msg);

	return true;
}

/*
 * 割り込みメッセージ受信処理
 */
static void main_int_msg(ECN_FBS_ID msg)
{
	ER ret;
	uint8_t cmd;
	ECN_FBS_SSIZE_T len;

	ret = _ecn_fbs_get_data(msg, &cmd, 1, &len);
	if (ret != E_OK) {
		return;
	}

	switch (cmd) {
	case ECN_INM_GET_DEVICE_LIST_RES:
		main_get_device_list_res(msg);
		break;
	case ECN_INM_GET_DEVICE_INFO_RES:
		main_get_device_info_res(msg);
		break;
	case ECN_UDP_MSG_GET_IPADDR_RES:
		main_get_ipaddr_res(msg);
		break;
	}
}

/*
 * 応答電文待ちの割り込み処理
 */
static void main_break_wait(uint8_t *brkdat, int32_t len)
{
	switch (main_state) {
	case main_state_start:
		main_start_service();
		main_state = main_state_idle;
		main_timer = 10;
		break;
	case main_state_idle:
		break;
	}
}

/*
 * 応答電文待ちの割り込み処理
 */
void main_set_addr_callback()
{
	ER ret;
	uint8_t data[1];

	/* メインタスクに通知 */
	data[0] = 0x01;
	ret = ecn_brk_wai(data, sizeof(data));
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_brk_wai");
	}
}

/*
 * 応答電文待ちの割り込み処理
 */
static void main_start_service()
{
	char ip_addr[16], netmask[16], gateway[16];
	ER ret;

	ip2str(ip_addr, uip_hostaddr);
	ip2str(netmask, uip_netmask);
	ip2str(gateway, uip_draddr);
	syslog(LOG_INFO, "ip_addr %s, netmask %s, gateway %s", ip_addr, netmask, gateway);

	/* ECHONETミドルウェアを起動 */
	ret = ecn_sta_svc();
	if (ret != E_OK)
		return;
}

static void main_btn_timeout();
static void main_request_info_timeout(request_info_t *request_info);

/*
 * タイムアウト処理
 */
static void main_timeout()
{
	int i;
	TMO temp;

	if (main_timer == 0) {
		main_btn_timeout();
	}

	for (i = 0; i < sizeof(request_infos) / sizeof(request_infos[0]); i++) {
		temp = request_infos[i].timer;
		if (temp != 0)
			continue;

		main_request_info_timeout(&request_infos[i]);;
	}
}

static void main_btn1_change(bool_t push);
static void main_btn2_change(bool_t push);
static void main_gpio_change(int pinno, uint16_t high);
static ER main_search_device();

static void main_btn_timeout()
{
	gpio_state_t *gpio;
	analog_state_t *analog;
	arduino_digital_value_t dval;
	int aval;
	bool_t val;
	int i;

	if (main_timer != 0)
		return;

	switch (main_state) {
	case main_state_idle:
		/* 10ms後にボタン状態を確認 */
		main_timer = 10;

		arduino_tick();

		for (i = 0; i < MAIN_GPIO_COUNT; i++) {
			gpio = &main_gpio[i];

			/* GPIO状態読み込み */
			switch (i) {
			case MAIN_GPIO_BTN1:
				val = (sil_reb_mem((uint8_t *)0x0008C04A) & 0x80) != 0;
				/*val = (sil_reb_mem((uint8_t *)0x0008C040) & 0x20) != 0;*/
				break;
			case MAIN_GPIO_BTN2:
				val = (sil_reb_mem((uint8_t *)0x0008C040) & 0x80) != 0;
				break;
			default:
				arduino_digitalRead(i - MAIN_GPIO_ARDUINO_PIN0, &dval);
				val = dval == ARDUINO_DIGITAL_VALUE_HIGH;
				break;
			}

			if (val && !gpio->state) {
				gpio->count++;
				if (gpio->count > 10) {
					gpio->count = 0;
					gpio->state = true;

					switch (i) {
					case MAIN_GPIO_BTN1:
						main_btn1_change(true);
						break;
					case MAIN_GPIO_BTN2:
						main_btn2_change(true);
						break;
					default:
						main_gpio_change(i - MAIN_GPIO_ARDUINO_PIN0, 1);
						break;
					}
				}
			}
			else if (!val && gpio->state) {
				gpio->count++;
				if (gpio->count > 10) {
					gpio->count = 0;
					gpio->state = false;

					switch (i) {
					case MAIN_GPIO_BTN1:
						main_btn1_change(false);
						break;
					case MAIN_GPIO_BTN2:
						main_btn2_change(false);
						break;
					default:
						main_gpio_change(i - MAIN_GPIO_ARDUINO_PIN0, 0);
						break;
					}
				}
			}
			else {
				gpio->count = 0;
			}
		}

		for (i = 0; i < MAIN_ANALOG_COUNT; i++) {
			analog = &main_analog[i];

			if (analog->subscriptionId == 0)
				continue;

			/* GPIO状態読み込み */
			arduino_analogRead(analog->pinno, &aval);

			if (analog->state)
				val = aval < analog->threshold;
			else
				val = aval > analog->threshold;

			if (val && !analog->state) {
				analog->count++;
				if (analog->count > 10) {
					analog->count = 0;
					analog->state = true;

					main_gpio_change(analog->pinno, aval);
				}
			}
			else if (val && analog->state) {
				analog->count++;
				if (analog->count > 10) {
					analog->count = 0;
					analog->state = false;

					main_gpio_change(analog->pinno, aval);
				}
			}
			else {
				analog->count = 0;
			}
		}
		break;
	}
}

ER main_add_threshold(int subscriptionId, int pinno, uint16_t threshold)
{
	int i, aval;
	analog_state_t *analog;

	if ((pinno < 14) || (pinno > 19))
		return E_PAR;

	for (i = 0; i < MAIN_ANALOG_COUNT; i++) {
		analog = &main_analog[i];
		if (analog->subscriptionId != 0)
			continue;

		analog->subscriptionId = subscriptionId;
		analog->pinno = pinno;
		analog->threshold = threshold;

		arduino_analogRead(analog->pinno, &aval);

		analog->state = (aval < analog->threshold);
		analog->count = 0;

		return E_OK;
	}

	return E_NOMEM;
}

ER main_del_threshold(int subscriptionId)
{
	int i;
	analog_state_t *analog;

	for (i = 0; i < MAIN_ANALOG_COUNT; i++) {
		analog = &main_analog[i];
		if (analog->subscriptionId != subscriptionId)
			continue;

		analog->subscriptionId = 0;

		return E_OK;
	}

	return E_OBJ;
}

static void main_self_msg(T_EDATA *esv)
{
	ER ret;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	for (;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			switch (epc) {
			case 0x80:
				break;
			case 0x81:
				break;
			}
		}
		if (ret != E_BOVR) {
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if (enm.is_eof)
			break;
	}
}

/*
 * ボタン１状態変化処理
 */
static void main_btn1_change(bool_t push)
{
	/* 押されて戻った時に処理する */
	if (push)
		return;

	/* 機器の検索 */
	main_search_device();
}

/*
 * ボタン２状態変化処理
 */
static void main_btn2_change(bool_t push)
{
}

/*
 * GPIO状態変化処理
 */
static void main_gpio_change(int pinno, uint16_t value)
{
	main_gpio_pubid++;

	wamp_broker_publish_pin(&wamp.broker, main_gpio_pubid, 1, pinno, value);
}

/*
 * 機器の検索
 */
static ER main_search_device()
{
	ER ret;
	T_EDATA *esv;

	/* 機器の検索 */
	ret = ecn_esv_inf_req(&esv, EOBJ_NULL, 0xD6);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_esv_inf_req");
		return ret;
	}

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	return E_OK;
}

extern int ws_out_req;

ER main_send_message(ECN_FBS_ID msg, ID wbsid)
{
	ER ret;

	((ID *)msg.ptr->_gap)[0] = wbsid;

	ret = psnd_dtq(WEBSOCKET_MBXID, (intptr_t)msg.ptr);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "psnd_dtq(WEBSOCKET_MBXID) : result=%d", ret);
		_ecn_fbs_del(msg);
		return ret;
	}

	ws_out_req++;
	act_tsk(UIP_TASK);

	return ret;
}

ER main_get_device_list(unsigned int requestId, struct wamp_dealer *dealer)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* 機器の検索 */
	ret = main_search_device();
	if (ret != E_OK) {
		return ret;
	}

	request_info->state = request_info_state_search_device;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;

	return E_OK;
}

static void main_search_device_timeout()
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	ECN_FBS_ID req;

	for (;;) {
		ret = ecn_agent_get_device_list(ECHONET_API_MAILBOX, request_info->requestId, &req);
		if (ret != E_OK) {
			syslog(LOG_ERROR, "ecn_agent_get_device_list");
			break;
		}

		ret = psnd_dtq(req.ptr->hdr.target_mbxid, (intptr_t)req.ptr);
		if (ret != E_OK) {
			syslog(LOG_ERROR, "psnd_dtq");
			_ecn_fbs_del(req);
			break;
		}

		request_info->state = request_info_state_get_device_list;
		request_info->timer = (TMO)1000;
		return;
	}

	wamp_dealer_get_devicelist_timeout(request_info->dealer);

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;
}

static void main_get_device_list_res(ECN_FBS_ID msg)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	unsigned int requestId;
	struct wamp_dealer *dealer;
	ECN_FBS_SSIZE_T len;

	ret = _ecn_fbs_get_data(msg, &requestId, sizeof(requestId), &len);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "_ecn_fbs_get_data");
		return;
	}

	if (request_info->requestId != requestId)
		return;

	dealer = request_info->dealer;
	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;

	wamp_dealer_set_devicelist(dealer, msg);
}

ER main_get_device_ipaddr(unsigned int requestId, struct wamp_dealer *dealer, ECN_ENOD_ID addrid)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	ECN_FBS_ID req;

	if (request_info->requestId != 0)
		return E_QOVR;

	ret = ecn_udp_get_ipaddr(ECHONET_API_MAILBOX, requestId, addrid, &req);
	if (ret != E_OK) {
		return ret;
	}

	ret = psnd_dtq(req.ptr->hdr.target_mbxid, (intptr_t)req.ptr);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "psnd_dtq");
		_ecn_fbs_del(req);
		return ret;
	}

	request_info->state = request_info_state_get_ipaddr;
	request_info->timer = (TMO)1000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;

	return E_OK;
}

static void main_get_ipaddr_res(ECN_FBS_ID msg)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	struct wamp_dealer *dealer;
	ECN_FBS_SSIZE_T len;
	ecn_udp_msg_get_ipaddr_res_t ipaddr;
	char str[16];

	ret = _ecn_fbs_get_data(msg, &ipaddr, sizeof(ipaddr), &len);
	if (ret || (len != sizeof(ipaddr))) {
		syslog(LOG_ERROR, "_ecn_fbs_get_data");
		return;
	}

	if (request_info->requestId != ipaddr.requestid)
		return;

	dealer = request_info->dealer;
	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;

	ipaddr2str(str, sizeof(str), ipaddr.enodadrb.ipaddr);

	wamp_dealer_set_ipaddr(dealer, str);
}

ER main_get_device_info(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	ECN_FBS_ID req;

	if (request_info->requestId != 0)
		return E_QOVR;

	ret = ecn_agent_get_device_info(ECHONET_API_MAILBOX, requestId, eobjid, &req);
	if (ret != E_OK) {
		return ret;
	}

	ret = psnd_dtq(req.ptr->hdr.target_mbxid, (intptr_t)req.ptr);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "psnd_dtq");
		_ecn_fbs_del(req);
		return ret;
	}

	request_info->state = request_info_state_get_device_info;
	request_info->timer = (TMO)1000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;

	return E_OK;
}

static void main_get_device_info_res(ECN_FBS_ID msg)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	struct wamp_dealer *dealer;
	ecn_inm_get_device_info_res_t rmsg;
	ECN_FBS_SSIZE_T len;

	ret = _ecn_fbs_get_data(msg, &rmsg, sizeof(rmsg), &len);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "_ecn_fbs_get_data");
		return;
	}

	if (request_info->requestId != rmsg.requestid)
		return;

	dealer = request_info->dealer;
	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;

	wamp_dealer_set_deviceinfo(dealer, rmsg.eobjid, rmsg.pmapSet, rmsg.pmapGet, rmsg.pmapAnno);
}

ER main_kadecot_get(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, uint8_t epc)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	T_EDATA *esv;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* プロパティ取得電文作成 */
	ret = ecn_esv_get(&esv, eobjid, epc);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_esv_get");
		return ret;
	}

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	request_info->state = request_info_state_kadecot_get;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;
	request_info->eobjid = eobjid;
	request_info->epc = epc;

	return E_OK;
}

static void main_kadecot_get_res(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	ID eobjid = ecn_get_eobj(esv);
	struct wamp_dealer *dealer = request_info->dealer;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;

	if (request_info->eobjid != eobjid)
		return;

	dealer = request_info->dealer;
	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	for (;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			if (request_info->epc == epc) {
				wamp_dealer_kadecot_get(dealer, eobjid, epc, pdc, p_edt);
			}
		}
		if (ret != E_BOVR) {
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if (enm.is_eof)
			break;
	}
}

ER main_kadecot_set(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, uint8_t epc,
	uint8_t pdc, uint8_t *edt)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	T_EDATA *esv;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* プロパティ取得電文作成 */
	ret = ecn_esv_setc(&esv, eobjid, epc, pdc, edt);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_esv_setc");
		return ret;
	}

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	request_info->state = request_info_state_kadecot_set;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;
	request_info->eobjid = eobjid;
	request_info->epc = epc;

	return E_OK;
}

static void main_kadecot_set_res(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;
	ID eobjid = ecn_get_eobj(esv);
	struct wamp_dealer *dealer = request_info->dealer;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;

	if (dealer == NULL)
		return;

	if (request_info->eobjid != eobjid)
		return;

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	for (;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			if (request_info->epc == epc) {
				wamp_dealer_kadecot_set(dealer, eobjid, epc);
			}
		}
		if (ret != E_BOVR) {
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if (enm.is_eof)
			break;
	}
}

ER main_ecnl_get(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	request_info->state = request_info_state_ecnl_get;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;
	request_info->eobjid = eobjid;

	return E_OK;
}

static void main_ecnl_get_res(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];

	wamp_dealer_ecnl_get_res(request_info->dealer, esv);

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;
}

ER main_ecnl_set(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	request_info->state = request_info_state_ecnl_set;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;
	request_info->eobjid = eobjid;

	return E_OK;
}

static void main_ecnl_set_res(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];

	wamp_dealer_ecnl_set_res(request_info->dealer, esv);

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;
}

ER main_ecnl_setget(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];
	ER ret;

	if (request_info->requestId != 0)
		return E_QOVR;

	/* 電文送信 */
	ret = ecn_snd_esv(esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_snd_esv");
		return ret;
	}

	request_info->state = request_info_state_ecnl_setget;
	request_info->timer = (TMO)5000;
	request_info->requestId = requestId;
	request_info->dealer = dealer;
	request_info->eobjid = eobjid;

	return E_OK;
}

static void main_ecnl_setget_res(T_EDATA *esv)
{
	request_info_t *request_info = &request_infos[0];

	wamp_dealer_ecnl_setget_res(request_info->dealer, esv);

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;
}

static void main_request_info_timeout(request_info_t *request_info)
{
	struct wamp_dealer *dealer = request_info->dealer;

	switch (request_info->state) {
	case request_info_state_search_device:
		main_search_device_timeout();
		return;
	case request_info_state_get_device_list:
		wamp_dealer_get_devicelist_timeout(dealer);
		break;
	case request_info_state_get_ipaddr:
		wamp_dealer_get_ipaddr_timeout(dealer);
		break;
	case request_info_state_get_device_info:
		wamp_dealer_get_deviceinfo_timeout(dealer);
		break;
	case request_info_state_kadecot_set:
		wamp_dealer_kadecot_set_timeout(dealer);
		break;
	case request_info_state_kadecot_get:
		wamp_dealer_kadecot_get_timeout(dealer);
		break;
	case request_info_state_ecnl_set:
		wamp_dealer_ecnl_set_timeout(dealer);
		break;
	case request_info_state_ecnl_get:
		wamp_dealer_ecnl_get_timeout(dealer);
		break;
	case request_info_state_ecnl_setget:
		wamp_dealer_ecnl_setget_timeout(dealer);
		break;
	}

	request_info->state = request_info_state_idle;
	request_info->timer = TMO_FEVR;
	request_info->requestId = 0;
	request_info->dealer = NULL;
}

static void main_publish(T_EDATA *esv)
{
	ID eobjid = ecn_get_eobj(esv);
	uint16_t devType = (esv->hdr.edata.seoj.eojx1 << 8) | esv->hdr.edata.seoj.eojx2;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;
	ER ret;

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK) {
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	for (;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			wamp_broker_publish_inf(&wamp.broker, esv->hdr.ecn_hdr.tid, eobjid,
				devType, epc, pdc, p_edt);
		}
		if (ret != E_BOVR) {
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if (enm.is_eof)
			break;
	}
}
