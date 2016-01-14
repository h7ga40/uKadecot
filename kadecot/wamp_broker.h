/*
 *  TOPPERS ECHONET Lite Communication Middleware
 *
 *  Copyright (C) 2015 Cores Co., Ltd. Japan
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

#ifndef WAMP_BROKER_H
#define WAMP_BROKER_H

typedef enum wamp_subscription_kind
{
	WAMP_SUBSCRIPTION_KIND_NONE,
	WAMP_SUBSCRIPTION_KIND_ECHONETLITE,
	WAMP_SUBSCRIPTION_KIND_ARDUINO,
} wamp_subscription_kind_t;

typedef struct wamp_echonetlite_subscription
{
	char deviceName[40];
	char *proprtyName;
	uint16_t devType;
	uint8_t epc;
} wamp_echonetlite_subscription_t;

typedef struct wamp_arduino_subscription
{
	uint8_t pinno;
	uint16_t threshold;
} wamp_arduino_subscription_t;

typedef struct wamp_subscription
{
	struct wamp_state *s;
	unsigned int requestId;
	int subscriptionId;
	wamp_subscription_kind_t kind;
	union{
		wamp_echonetlite_subscription_t echonetlite;
		wamp_arduino_subscription_t arduino;
	};
} wamp_subscription_t;

typedef struct wamp_broker
{
	struct wamp_state *s;
	unsigned int requestId;
	int subscriptionId;
	wamp_subscription_t subscriptions[30];
	wamp_subscription_t *current_subscription;
	const char *error;
} wamp_broker_t;

void wamp_broker_subscribe(wamp_broker_t *broker);
void wamp_broker_subscribe_request_id(wamp_broker_t *broker, const char *value);
void wamp_broker_subscribe_options_param(wamp_broker_t *broker, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_broker_subscribe_options(wamp_broker_t *broker);
void wamp_broker_subscribe_topic(wamp_broker_t *broker, const char *value);
void wamp_broker_subscribe_echonetlite_topic(wamp_broker_t *broker, const char *value, int pos);
void wamp_broker_subscribe_arduino_topic(wamp_broker_t *broker, const char *value, int pos);
void wamp_broker_subscribe_close(struct wamp_state *s);

void wamp_broker_unsubscribe(wamp_broker_t *broker);
void wamp_broker_unsubscribe_request_id(wamp_broker_t *broker, const char *value);
void wamp_broker_unsubscribe_subscription_id(wamp_broker_t *broker, const char *value);
void wamp_broker_unsubscribe_close(struct wamp_state *s);

void wamp_broker_publish(wamp_broker_t *broker);
void wamp_broker_publish_request_id(wamp_broker_t *s, const char *value);
void wamp_broker_publish_options(wamp_broker_t *s, const char *value);
void wamp_broker_publish_options_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_broker_publish_topic(wamp_broker_t *s, const char *value);
void wamp_broker_publish_arguments(wamp_broker_t *s, const char *value);
void wamp_broker_publish_arguments_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_broker_publish_argumentskw(wamp_broker_t *s, const char *value);
void wamp_broker_publish_argumentskw_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_broker_publish_close(struct wamp_state *s);

void wamp_broker_publish_inf(wamp_broker_t *broker, uint16_t pubId, int deviceId,
	uint16_t devType, uint8_t epc, uint8_t pdc, uint8_t *p_edt);
void wamp_broker_publish_pin(wamp_broker_t *broker, uint16_t pubId, int deviceId,
	int pinno, uint16_t value);

#endif // WAMP_BROKER_H
