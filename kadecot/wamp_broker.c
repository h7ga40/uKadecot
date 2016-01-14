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

#include <string.h>
#include <stdlib.h>
#include "wamp.h"
#include "wamp_broker.h"
#include "main.h"
#include "fbs_string.h"
#include "kadecot_names.h"
#include "kernel_cfg.h"

#ifndef _MSC_VER
#ifndef strncpy_s
#define strncpy_s(dst, dsz, src, sz) strncpy(dst, src, sz)
#endif

#ifndef strcpy_s
#define strcpy_s(s1, s1m, s2) strcpy(s1, s2)
#endif
#endif

static wamp_subscription_t *wamp_broker_new_subscription(wamp_broker_t *broker);
static void wamp_broker_delete_subscription(wamp_broker_t *broker,
	wamp_subscription_t *subscription);

void wamp_broker_subscribe(wamp_broker_t *broker)
{
	broker->current_subscription = NULL;
}

void wamp_broker_subscribe_request_id(wamp_broker_t *broker, const char *value)
{
	broker->requestId = atoi(value);
}

void wamp_broker_subscribe_options_param(wamp_broker_t *broker, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_broker_subscribe_options(wamp_broker_t *broker)
{
}

void wamp_broker_subscribe_topic(wamp_broker_t *broker, const char *value)
{
	static const char echonetlite_topic[] = "com.sonycsl.kadecot.echonetlite.topic.";
	static const char arduino_topic[] = "com.sonycsl.kadecot.arduino.topic.";

	if (strncmp(echonetlite_topic, value, sizeof(echonetlite_topic) - 1) == 0)
		wamp_broker_subscribe_echonetlite_topic(broker, value, sizeof(echonetlite_topic) - 1);
	else if (strncmp(arduino_topic, value, sizeof(arduino_topic) - 1) == 0)
		wamp_broker_subscribe_arduino_topic(broker, value, sizeof(arduino_topic) - 1);
}

void wamp_broker_subscribe_echonetlite_topic(wamp_broker_t *broker, const char *value, int val_pos)
{
	char *pos, *end;
	wamp_subscription_t *subscription;
	wamp_echonetlite_subscription_t *echonetlite;
	uint16_t devType;
	uint8_t epc;

	subscription = wamp_broker_new_subscription(broker);
	if(subscription == NULL)
		return;
	subscription->kind = WAMP_SUBSCRIPTION_KIND_ECHONETLITE;
	echonetlite = &subscription->echonetlite;

	echonetlite->deviceName[0] = '\0';
	strcpy_s(echonetlite->deviceName, sizeof(echonetlite->deviceName), &value[val_pos]);
	echonetlite->deviceName[sizeof(echonetlite->deviceName) - 1] = '\0';

	end = &echonetlite->deviceName[sizeof(echonetlite->deviceName)];
	for(pos = echonetlite->deviceName; pos < end; pos++){
		if(*pos != '.')
			continue;

		*pos = '\0';
		echonetlite->proprtyName = pos + 1;
		break;
	}

	devType = kadecot_names_get_device_type(echonetlite->deviceName,
		strnlen(echonetlite->deviceName, sizeof(echonetlite->deviceName)));
	if(devType == 0){
		wamp_broker_delete_subscription(broker, subscription);
		return;
	}

	epc = kadecot_names_get_property_code(echonetlite->proprtyName,
		strnlen(echonetlite->proprtyName, echonetlite->proprtyName - echonetlite->deviceName), devType);
	if(epc == 0){
		wamp_broker_delete_subscription(broker, subscription);
		return;
	}

	echonetlite->devType = devType;
	echonetlite->epc = epc;

	wamp_subscription_t *spos, *send = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];

	for (spos = broker->subscriptions; spos < send; spos++) {
		if ((spos == subscription) || (spos->subscriptionId == 0) || (spos->kind != WAMP_SUBSCRIPTION_KIND_ECHONETLITE))
			continue;

		if ((spos->echonetlite.devType != devType) || (spos->echonetlite.epc != epc))
			continue;

		wamp_broker_delete_subscription(broker, subscription);
		subscription = spos;
		break;
	}

	broker->current_subscription = subscription;
}

void wamp_broker_subscribe_arduino_topic(wamp_broker_t *broker, const char *value, int val_pos)
{
	char *pos, *end;
	wamp_subscription_t *subscription;
	wamp_arduino_subscription_t *arduino;
	char pinno[40], *threshold = NULL;

	subscription = wamp_broker_new_subscription(broker);
	if (subscription == NULL)
		return;
	subscription->kind = WAMP_SUBSCRIPTION_KIND_ARDUINO;
	arduino = &subscription->arduino;

	pinno[0] = '\0';
	strcpy_s(pinno, sizeof(pinno), &value[val_pos]);
	pinno[sizeof(pinno) - 1] = '\0';

	if (pinno[0] != 'p' || pinno[1] != 'i' || pinno[2] != 'n') {
		wamp_broker_delete_subscription(broker, subscription);
		return;
	}

	end = &pinno[sizeof(pinno)];
	for (pos = pinno; pos < end; pos++) {
		if (*pos != '.')
			continue;

		*pos = '\0';
		threshold = pos + 1;
		if (threshold[0] != 't' || threshold[1] != 'h' || threshold[2] != 'r') {
			threshold = NULL;
		}
		break;
	}

	arduino->pinno = atoi(&pinno[3]);
	if (threshold != NULL) {
		arduino->threshold = atoi(&threshold[3]);
	}
	else {
		arduino->threshold = 0;
	}

	wamp_subscription_t *spos, *send = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];

	for (spos = broker->subscriptions; spos < send; spos++) {
		if ((spos == subscription) || (spos->subscriptionId == 0) || (spos->kind != WAMP_SUBSCRIPTION_KIND_ARDUINO))
			continue;

		if ((spos->arduino.pinno != arduino->pinno) || (spos->arduino.threshold != arduino->threshold))
			continue;

		wamp_broker_delete_subscription(broker, subscription);
		subscription = spos;
		break;
	}

	broker->current_subscription = subscription;
}

static void wamp_broker_subscribe_echonetlite_res(wamp_broker_t *broker);
static void wamp_broker_subscribe_arduino_res(wamp_broker_t *broker);

void wamp_broker_subscribe_close(struct wamp_state *s)
{
	wamp_broker_t *broker = &s->broker;
	ER ret;

	broker->error = internal_error;

	// send RESULT
	if (broker->current_subscription != NULL) {
		switch (broker->current_subscription->kind) {
		case WAMP_SUBSCRIPTION_KIND_ECHONETLITE:
			wamp_broker_subscribe_echonetlite_res(broker);
			break;
		case WAMP_SUBSCRIPTION_KIND_ARDUINO:
			wamp_broker_subscribe_arduino_res(broker);
			break;
		default:
			break;
		}
	}

	if (broker->error != NULL){
		ret = wamp_send_error_res(s, WAMP_CODE_SUBSCRIBE, broker->requestId, broker->error);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "wamp_broker_subscribe_close() : wamp_send_error_res() result = %d", ret);
		}
		broker->error = NULL;
	}
}

static void wamp_broker_subscribe_echonetlite_res(wamp_broker_t *broker)
{
	struct wamp_state *s = broker->s;
	wamp_subscription_t *subscription = broker->current_subscription;
	// send SUBSCRIBED or ERROR
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	if ((subscription == NULL) || (subscription->echonetlite.devType == 0) || (subscription->echonetlite.epc == 0))
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[33,%d,%d]", broker->requestId, subscription->subscriptionId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_broker_subscribe_echonetlite_res() : main_send_message() result = %d", ret);
		return;
	}

	broker->error = NULL;
}

static void wamp_broker_subscribe_arduino_res(wamp_broker_t *broker)
{
	struct wamp_state *s = broker->s;
	wamp_subscription_t *subscription = broker->current_subscription;
	// send SUBSCRIBED or ERROR
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	if ((subscription == NULL)
		|| ((subscription->arduino.pinno < 0) || (subscription->arduino.pinno > 19)))
		return;

	if (subscription->arduino.pinno >= 14)
		main_add_threshold(subscription->subscriptionId, subscription->arduino.pinno, subscription->arduino.threshold);

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[33,%d,%d]", broker->requestId, subscription->subscriptionId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_broker_subscribe_arduino_res() : main_send_message() result = %d", ret);
		return;
	}

	broker->error = NULL;
}

void wamp_broker_unsubscribe(wamp_broker_t *broker)
{
	broker->current_subscription = NULL;
}

void wamp_broker_unsubscribe_request_id(wamp_broker_t *broker, const char *value)
{
	broker->requestId = atoi(value);
}

void wamp_broker_unsubscribe_subscription_id(wamp_broker_t *broker, const char *value)
{
	int subscriptionId = atoi(value);
	wamp_subscription_t *pos, *end = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];

	for(pos = broker->subscriptions; pos < end; pos++){
		if(pos->subscriptionId != subscriptionId)
			continue;

		broker->current_subscription = pos;
		break;
	}
}

void wamp_broker_unsubscribe_close(struct wamp_state *s)
{
	wamp_broker_t *broker = &s->broker;
	wamp_subscription_t *subscription = broker->current_subscription;
	// send UNSUBSCRIBED or ERROR
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	if(subscription == NULL)
		return;

	if (subscription->kind == WAMP_SUBSCRIPTION_KIND_ARDUINO)
		main_del_threshold(subscription->subscriptionId);

	wamp_broker_delete_subscription(broker, subscription);

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[35,%d,%d]", broker->requestId, subscription->subscriptionId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_broker_unsubscribe_close() : main_send_message() result = %d", ret);
	}
}

void wamp_broker_publish(wamp_broker_t *broker)
{
}

void wamp_broker_publish_request_id(wamp_broker_t *s, const char *value)
{
}

void wamp_broker_publish_options(wamp_broker_t *s, const char *value)
{
}

void wamp_broker_publish_options_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_broker_publish_topic(wamp_broker_t *s, const char *value)
{
}

void wamp_broker_publish_arguments(wamp_broker_t *s, const char *value)
{
}

void wamp_broker_publish_arguments_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_broker_publish_argumentskw(wamp_broker_t *s, const char *value)
{
}

void wamp_broker_publish_argumentskw_param(wamp_broker_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_broker_publish_close(struct wamp_state *s)
{
}

static wamp_subscription_t *wamp_broker_new_subscription(wamp_broker_t *broker)
{
	wamp_subscription_t *pos, *end = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];

	for(pos = broker->subscriptions; pos < end; pos++){
		if(pos->subscriptionId != 0)
			continue;

		broker->subscriptionId++;
		if(broker->subscriptionId == 0)
			broker->subscriptionId++;

		pos->subscriptionId = broker->subscriptionId;
		return pos;
	}

	return NULL;
}

static void wamp_broker_delete_subscription(wamp_broker_t *broker,
	wamp_subscription_t *subscription)
{
	memset(subscription, 0, sizeof(*subscription));
}

void wamp_broker_publish_inf(wamp_broker_t *broker, uint16_t pubId, int deviceId,
	uint16_t devType, uint8_t epc, uint8_t pdc, uint8_t *p_edt)
{
	struct wamp_state *s = broker->s;
	wamp_subscription_t *subscription, *end = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	ER ret;
	int i;

	for(subscription = broker->subscriptions; subscription < end; subscription++){
		if ((subscription->subscriptionId == 0) || (subscription->kind != WAMP_SUBSCRIPTION_KIND_ECHONETLITE))
			continue;

		if ((subscription->echonetlite.devType != devType) || (subscription->echonetlite.epc != epc))
			continue;

		ret = _ecn_fbs_cre(1, &buf);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
			break;
		}

		ret = fbs_printf(buf, &pos, "[36,%d,%d,{\"deviceId\":%d},[],",
			subscription->subscriptionId, pubId, deviceId);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			break;
		}

		ret = fbs_printf(buf, &pos, "{\"propertyName\":\"%s\",\"propertyValue\":[",
			subscription->echonetlite.proprtyName);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			break;
		}

		start = pos;
		for(i = 0; i < pdc; i++){
			ret = fbs_printf(buf, &pos, "%d,", p_edt[i]);
			if (ret != E_OK) {
				syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
				_ecn_fbs_del(buf);
				break;
			}
		}

		/* 最後の","を消す */
		if(start != pos)
			pos--;

		ret = fbs_printf(buf, &pos, "]}]");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			break;
		}

		ret = main_send_message(buf, s->wbsid);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "wamp_broker_publish_inf() : main_send_message() result = %d", ret);
		}
		break;
	}
}

void wamp_broker_publish_pin(wamp_broker_t *broker, uint16_t pubId, int deviceId, int pinno, uint16_t value)
{
	struct wamp_state *s = broker->s;
	wamp_subscription_t *subscription, *end = &broker->subscriptions[sizeof(broker->subscriptions) / sizeof(broker->subscriptions[0])];
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	for (subscription = broker->subscriptions; subscription < end; subscription++) {
		if ((subscription->subscriptionId == 0) || (subscription->kind != WAMP_SUBSCRIPTION_KIND_ARDUINO))
			continue;

		if (subscription->arduino.pinno != pinno)
			continue;

		ret = _ecn_fbs_cre(1, &buf);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
			break;
		}

		ret = fbs_printf(buf, &pos, "[36,%d,%d,{\"deviceId\":%d},[],",
			subscription->subscriptionId, pubId, deviceId);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			break;
		}

		if ((pinno >= 0) && (pinno <= 13)) {
			ret = fbs_printf(buf, &pos, "{\"pin\":%d,\"value\":\"%s\"}]",
				subscription->arduino.pinno, (value == 0) ? "LOW" : "HIGH");
			if (ret != E_OK) {
				syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
				_ecn_fbs_del(buf);
				break;
			}
		}
		else {
			ret = fbs_printf(buf, &pos, "{\"pin\":%d,\"value\":%d}]",
				subscription->arduino.pinno, value);
			if (ret != E_OK) {
				syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
				_ecn_fbs_del(buf);
				break;
			}
		}

		ret = main_send_message(buf, s->wbsid);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "wamp_broker_publish_inf() : main_send_message() result = %d", ret);
		}
		break;
	}
}
