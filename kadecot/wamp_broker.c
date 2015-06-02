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
	static const char topic[] = "com.sonycsl.kadecot.echonetlite.topic.";
	char *pos, *end;
	wamp_subscription_t *subscription;
	uint16_t devType;
	uint8_t epc;

	if(strncmp(topic, value, sizeof(topic) - 1) != 0)
		return;

	subscription = wamp_broker_new_subscription(broker);

	subscription->deviceName[0] = '\0';
	strcpy_s(subscription->deviceName, sizeof(subscription->deviceName), &value[sizeof(topic) - 1]);
	subscription->deviceName[sizeof(subscription->deviceName) - 1] = '\0';

	end = &subscription->deviceName[sizeof(subscription->deviceName)];
	for(pos = subscription->deviceName; pos < end; pos++){
		if(*pos != '.')
			continue;

		*pos = '\0';
		subscription->proprtyName = pos + 1;
		break;
	}

	devType = kadecot_names_get_device_type(subscription->deviceName,
		strnlen(subscription->deviceName, sizeof(subscription->deviceName)));
	if(devType == 0){
		wamp_broker_delete_subscription(broker, subscription);
		return;
	}

	epc = kadecot_names_get_property_code(subscription->proprtyName,
		strnlen(subscription->proprtyName, subscription->proprtyName - subscription->deviceName), devType);
	if(epc == 0){
		wamp_broker_delete_subscription(broker, subscription);
		return;
	}

	subscription->devType = devType;
	subscription->epc = epc;

	broker->current_subscription = subscription;
}

static void wamp_broker_subscribe_res(wamp_broker_t *broker);

void wamp_broker_subscribe_close(struct wamp_state *s)
{
	wamp_broker_t *broker = &s->broker;
	ER ret;

	broker->error = internal_error;

	// send RESULT
	wamp_broker_subscribe_res(broker);

	if (broker->error != NULL){
		ret = wamp_send_error_res(s, WAMP_CODE_SUBSCRIBE, broker->requestId, broker->error);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "wamp_broker_subscribe_close() : wamp_send_error_res() result = %d", ret);
		}
		broker->error = NULL;
	}
}

static void wamp_broker_subscribe_res(wamp_broker_t *broker)
{
	struct wamp_state *s = broker->s;
	wamp_subscription_t *subscription = broker->current_subscription;
	// send SUBSCRIBED or ERROR
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	if ((subscription == NULL) || (subscription->devType == 0) || (subscription->epc == 0))
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
		syslog(LOG_WARNING, "wamp_broker_subscribe_close() : main_send_message() result = %d", ret);
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
		if (subscription->subscriptionId == 0)
			continue;

		if ((subscription->devType != devType) || (subscription->epc != epc))
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
			subscription->proprtyName);
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
