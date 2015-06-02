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

#ifndef _MAIN_H_
#define _MAIN_H_

/*
 *		サンプルプログラム(1)のヘッダファイル
 */

/*
 *  ターゲット依存の定義
 */
#include <kernel.h>
#include "target_config.h"
#include "echonet.h"
#include "echonet_fbs.h"

/*
 *  各タスクの優先度の定義
 */

#define MAIN_PRIORITY	7		/* メインタスクの優先度 */
#define INIT_PRIORITY		1	/* 初期化タスクの優先度 */
#define LOGTASK_PRIORITY	8	/* ログタスクの優先度 */

#define MAIN_STACK_SIZE	1024	/* スタック領域のサイズ */

#define EOJ_X3_LOCAL_NODE 1	/* ノードプロファイルオブジェクトのインスタンスコード */

#define EOJ_X3_CONTROLLER 1	/* コントローラークラスのインスタンスコード */

#define NUM_WEBSOCKET_MBXID 2

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void main_task_cychdr(intptr_t exinf);

#ifdef __CA850__
#pragma pack(1)
#elif __RX
#pragma pack
#elif _MSC_VER
#pragma pack(push, 1)
#endif

/*
 *  Ｖｅｒｓｉｏｎ情報の型
 */
struct version_information_t {
	/* メジャーバージョン(小数点以上) */
	uint8_t major_version_number;
	/* マイナーバージョン(小数点以下) */
	uint8_t minor_version_number;
	/* 電文タイプ */
	uint8_t message_type[2];
};

/*
 *  メーカーコードの型
 */
struct manufacturer_code_t {
	/* メーカーコード */
	uint8_t manufacturer_code[3];
};

/*
 *  識別番号の型
 */
struct node_identification_number_t {
	/* 下位通信層IDフィールド */
	uint8_t lower_communication_id_field;
	/* メーカーコード */
	struct manufacturer_code_t manufacturer_code;
	/* ユニークID部(メーカー独自) */
	uint8_t unique_id_section[13];
};

/*
 *  規格Ｖｅｒｓｉｏｎ情報の型
 */
struct standard_version_information_t {
	/* 固定1(for future reserved) */
	uint8_t reserved1;
	/* 固定2(for future reserved) */
	uint8_t reserved2;
	/* リリース順をASCIIで示す */
	uint8_t order_of_release;
	/* 固定4(for future reserved) */
	uint8_t reserved4;
};

/*
 *  ノードプロファイルオブジェクト
 */
struct node_profile_object_t {
	/* 動作状態 */
	uint8_t operation_status;
	/* Ｖｅｒｓｉｏｎ情報 */
	struct version_information_t version_information;
	/* 識別番号 */
	struct node_identification_number_t identification_number;
	/* 異常内容 */
	uint16_t fault_content;
	/* メーカーコード */
	struct manufacturer_code_t manufacturer_code;
};

/*
 *  コントローラークラス
 */
struct controller_t {
	/* 動作状態 */
	uint8_t operation_status;
	/* 設置場所 */
	uint8_t installation_location;
	/* 規格Ｖｅｒｓｉｏｎ情報 */
	struct standard_version_information_t standard_version_information;
	/* 異常発生状態 */
	uint8_t fault_status;
	/* メーカーコード */
	struct manufacturer_code_t manufacturer_code;
};

#ifdef __CA850__
#pragma pack(8)
#elif __RX
#pragma unpack
#elif _MSC_VER
#pragma pack(pop)
#endif

extern struct node_profile_object_t local_node_data;	/* ノードプロファイルオブジェクトのデータ */

extern struct controller_t controller_class_data;	/* コントローラークラスのデータ */

EPRP_SETTER onoff_prop_set;
EPRP_SETTER node_profile_object_fault_content_set;
EPRP_SETTER alarm_prop_set;
EPRP_SETTER time_prop_set;
EPRP_GETTER time_prop_get;
EPRP_SETTER date_prop_set;
EPRP_GETTER date_prop_get;

ER main_send_message(ECN_FBS_ID buf, ID wbsid);
ER main_get_device_list(unsigned int requestId, struct wamp_dealer *dealer);
ER main_get_device_ipaddr(unsigned int requestId, struct wamp_dealer *dealer, ECN_ENOD_ID addrid);
ER main_get_device_info(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid);
ER main_kadecot_get(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, uint8_t epc);
ER main_kadecot_set(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, uint8_t epc,
	uint8_t pdc, uint8_t *edt);
ER main_ecnl_get(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv);
ER main_ecnl_set(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv);
ER main_ecnl_setget(unsigned int requestId, struct wamp_dealer *dealer, ID eobjid, T_EDATA *esv);

#endif /* TOPPERS_MACRO_ONLY */

#endif	/* of #ifndef _MAIN_H_ */
