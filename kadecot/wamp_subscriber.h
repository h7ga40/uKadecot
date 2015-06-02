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

#ifndef WAMP_SUBSCRIBER_H
#define WAMP_SUBSCRIBER_H

typedef struct wamp_subscriber
{
	struct wamp_state *s;
	int id;
} wamp_subscriber_t;

void wamp_subscriber_subscribed(wamp_subscriber_t *subscriber);
void wamp_subscriber_subscribed_request_id(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_subscribed_subscription_id(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_subscribed_close(struct wamp_state *s);

void wamp_subscriber_unsubscribed(wamp_subscriber_t *subscriber);
void wamp_subscriber_unsubscribed_request_id(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_unsubscribed_close(struct wamp_state *s);

void wamp_subscriber_event(wamp_subscriber_t *subscriber);
void wamp_subscriber_event_subscription_id(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_event_publication_id(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_event_details(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_event_details_param(wamp_subscriber_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_subscriber_event_arguments(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_event_arguments_param(wamp_subscriber_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_subscriber_event_argumentskw(wamp_subscriber_t *s, const char *value);
void wamp_subscriber_event_argumentskw_param(wamp_subscriber_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_subscriber_event_close(struct wamp_state *s);

#endif // WAMP_SUBSCRIBER_H
