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
#include "wamp.h"
#include "kernel_cfg.h"
#include "fbs_string.h"
#include "main.h"

/* パースエラー（定義のみ） */
const char parse_error[] = "com.sonycsl.kadecot.plugin.echonetlite.error.parse_error";
/* 無効なリクエスト（定義のみ） */
const char invalid_request[] = "com.sonycsl.kadecot.plugin.echonetlite.error.invalid_request";
/* 存在しないメソッド（定義のみ） */
const char method_not_found[] = "com.sonycsl.kadecot.plugin.echonetlite.error.method_not_found";
/* echo objectが存在しない,　JSONArray形式になっていない */
const char invalid_params[] = "com.sonycsl.kadecot.plugin.echonetlite.error.invalid_params";
/* タイムアウト発生, コールバックが　null*/
const char internal_error[] = "com.sonycsl.kadecot.plugin.echonetlite.error.internal_error";

static void wamp_idle_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_code_push_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_code_pop_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_param_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_nomore_param_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_id_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_list_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_dict_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_uri_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_int_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_string_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);

static void wamp_proc_hello_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_welcome_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_abort_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_challenge_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_authenticate_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_goodbye_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_error_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_publish_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_published_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_subscribe_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_subscribed_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_unsubscribe_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_unsubscribed_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_event_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_call_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_cancel_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_result_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_register_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_registered_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_unregister_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_unregistered_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_invocation_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_interrupt_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_proc_yield_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);

static void wamp_param_handler(struct wamp_state *s, jsonsl_action_t action,
	wamp_param_state_t param, const char *value);

void wamp_error(wamp_state_t *s);
void wamp_error_type(wamp_state_t *s, const char *value);
void wamp_error_request_id(wamp_state_t *s, const char *value);
void wamp_error_details(wamp_state_t *s, const char *value);
void wamp_error_details_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_error_uri(wamp_state_t *s, const char *value);
void wamp_error_arguments(wamp_state_t *s, const char *value);
void wamp_error_arguments_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_error_argumentskw(wamp_state_t *s, const char *value);
void wamp_error_argumentskw_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
static void wamp_error_close(wamp_state_t *s);

void wamp_init(wamp_state_t *s)
{
	jsonsl_t jsn;

	memset(s, 0, sizeof(wamp_state_t));
	s->jsn_buf_pos = -1;

	jsn = jsonsl_new((jsonsl_t)&s->jsn, JSN_NLEVELS);
	jsn->data = s;
	jsn->error_callback = wamp_error_callback;
	jsn->action_callback = wamp_state_callback;
	jsonsl_enable_all_callbacks(jsn);

	s->router.s = s;
	s->client.s = s;
	s->broker.s = s;
	s->dealer.s = s;
	s->publisher.s = s;
	s->subscriber.s = s;
	s->caller.s = s;
	s->callee.s = s;
	s->param_handler = wamp_param_handler;

	wamp_dealer_init(&s->dealer);
}

void wamp_put_msg(wamp_state_t *s, ECN_FBS_ID msg, ID wbsid)
{
	ECN_FBS_SSIZE_T len, pos;
	uint8_t data;

	s->wbsid = wbsid;

	len = _ecn_fbs_get_datalen(msg);

	for (pos = 0; pos < len; pos++) {
		data = _ecn_fbs_peek(msg, pos);

		if ((s->jsn_buf_pos >= 0) && (s->jsn_buf_pos < sizeof(s->jsn_buf)))
			s->jsn_buf[s->jsn_buf_pos++] = data;
		jsonsl_feed((jsonsl_t)s->jsn, &data, 1);
	}
}

void wamp_state_callback(jsonsl_t jsn, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	wamp_state_t *s = (wamp_state_t *)jsn->data;
	/*syslog(7, "@%5ld '%c' L%d %c%s",
			jsn->pos,
			*buf,
			state->level,
			action,
			jsonsl_strtype(state->type));*/

	switch (action) {
	case JSONSL_ACTION_PUSH:
		switch (state->type) {
		case JSONSL_T_SPECIAL:
			s->jsn_buf[0] = *buf;
			s->jsn_buf_pos = 1;
			break;
		case JSONSL_T_STRING:
		case JSONSL_T_HKEY:
			s->jsn_buf_pos = 0;
			break;
		default:
			s->jsn_buf_pos = -1;
		}
		break;
	case JSONSL_ACTION_POP:
		switch (state->type) {
		case JSONSL_T_SPECIAL:
		case JSONSL_T_STRING:
		case JSONSL_T_HKEY:
			s->jsn_buf_pos--;
			if (s->jsn_buf_pos < sizeof(s->jsn_buf)) {
				s->jsn_buf[s->jsn_buf_pos] = '\0';
			}
			break;
		default:
			s->jsn_buf[0] = '\0';
			break;
		}
		s->jsn_buf_pos = -1;
		break;
	default:
		s->jsn_buf_pos = -1;
		break;
	}

	switch (s->state) {
	case WAMP_PARSE_STATE_IDLE:
		wamp_idle_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_CODE_PUSH:
		wamp_code_push_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_CODE_POP:
		wamp_code_pop_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_ID_PUSH:
		wamp_id_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_LIST_PUSH:
		wamp_list_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_DICT_PUSH:
		wamp_dict_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_URI_PUSH:
		wamp_uri_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_INT_PUSH:
		wamp_int_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_STRING_PUSH:
		wamp_string_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_PARAM:
		wamp_param_parse_state(s, action, state, buf);
		break;
	case WAMP_PARSE_STATE_NOMORE_PARAM:
		wamp_nomore_param_parse_state(s, action, state, buf);
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}

	if (s->state == WAMP_PARSE_STATE_ERROR) {
		/* WAMPメッセージの最後だったら */
		if ((state->level == 1) && (action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_LIST)) {
			s->message_close_handler(s);
		}
		else {
			s->param_handler(s, JSONSL_ACTION_ERROR, s->prmst, NULL);
		}
		jsonsl_reset((jsonsl_t)s->jsn);
		s->state = WAMP_PARSE_STATE_IDLE;
	}
}

int wamp_error_callback(jsonsl_t jsn, jsonsl_error_t err,
struct jsonsl_state_st *state, char *errat)
{
	return 0;
}

static void wamp_idle_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* WAMPメッセージの先頭だったら */
	if ((state->level == 1) && (action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_LIST)) {
		/* メッセージコード取得開始状態に移る */
		s->state = WAMP_PARSE_STATE_CODE_PUSH;
	}
}

static void wamp_code_push_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_SPECIAL)) {
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_CODE_POP;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_code_pop_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	int code;
	wamp_parse_state_t prsst = WAMP_PARSE_STATE_ERROR;

	/* メッセージコードが取得でき */
	if ((action == JSONSL_ACTION_POP)
		&& (state->type == JSONSL_T_SPECIAL)) {
		/* メッセージコードを取得 */
		code = atoi(s->jsn_buf);
		switch (code) {
			/* [HELLO, Realm|uri, Details|dict] */
		case WAMP_CODE_HELLO:
			wamp_router_hello(&s->router);
			s->proc_handler = wamp_proc_hello_state;
			s->message_close_handler = wamp_router_hello_close;
			s->prmst = WAMP_PARAM_STATE_REALM_URI;
			prsst = WAMP_PARSE_STATE_URI_PUSH;
			break;
			/* [WELCOME, Session|id, Details|dict] */
		case WAMP_CODE_WELCOME:
			wamp_client_welcome(&s->client);
			s->proc_handler = wamp_proc_welcome_state;
			s->message_close_handler = wamp_client_welcome_close;
			s->prmst = WAMP_PARAM_STATE_SESSION_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [ABORT, Details|dict, Reason|uri] */
		case WAMP_CODE_ABORT:
			wamp_client_abort(&s->client);
			s->proc_handler = wamp_proc_abort_state;
			s->message_close_handler = wamp_client_abort_close;
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			prsst = WAMP_PARSE_STATE_DICT_PUSH;
			break;
			/* [CHALLENGE, AuthMethod|string, Extra|dict] */
		case WAMP_CODE_CHALLENGE:
			wamp_client_challenge(&s->client);
			s->proc_handler = wamp_proc_challenge_state;
			s->message_close_handler = wamp_client_challenge_close;
			s->prmst = WAMP_PARAM_STATE_AUTHMETHOD_STRING;
			prsst = WAMP_PARSE_STATE_STRING_PUSH;
			break;
			/* [AUTHENTICATE, Signature|string, Extra|dict] */
		case WAMP_CODE_AUTHENTICATE:
			wamp_router_authenticate(&s->router);
			s->proc_handler = wamp_proc_authenticate_state;
			s->message_close_handler = wamp_router_authenticate_close;
			s->prmst = WAMP_PARAM_STATE_SIGNATURE_STRING;
			prsst = WAMP_PARSE_STATE_STRING_PUSH;
			break;
			/* [GOODBYE, Details|dict, Reason|uri] */
		case WAMP_CODE_GOODBYE:
			wamp_router_goodbye(&s->router);
			s->proc_handler = wamp_proc_goodbye_state;
			s->message_close_handler = wamp_router_goodbye_close;
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			prsst = WAMP_PARSE_STATE_DICT_PUSH;
			break;
			/* [ERROR, REQUEST.Type|int, REQUEST.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict] */
		case WAMP_CODE_ERROR:
			wamp_error(s);
			s->proc_handler = wamp_proc_error_state;
			s->message_close_handler = wamp_error_close;
			s->prmst = WAMP_PARAM_STATE_TYPE_INT;
			prsst = WAMP_PARSE_STATE_INT_PUSH;
			break;
			/* [PUBLISH, Request|id, Options|dict, Topic|uri, Arguments|list, ArgumentsKw|dict] */
		case WAMP_CODE_PUBLISH:
			wamp_broker_publish(&s->broker);
			s->proc_handler = wamp_proc_publish_state;
			s->message_close_handler = wamp_broker_publish_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [PUBLISHED, PUBLISH.Request|id, Publication|id] */
		case WAMP_CODE_PUBLISHED:
			wamp_publisher_published(&s->publisher);
			s->proc_handler = wamp_proc_published_state;
			s->message_close_handler = wamp_publisher_published_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [SUBSCRIBE, Request|id, Options|dict, Topic|uri] */
		case WAMP_CODE_SUBSCRIBE:
			wamp_broker_subscribe(&s->broker);
			s->proc_handler = wamp_proc_subscribe_state;
			s->message_close_handler = wamp_broker_subscribe_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [SUBSCRIBED, SUBSCRIBE.Request|id, Subscription|id] */
		case WAMP_CODE_SUBSCRIBED:
			wamp_subscriber_subscribed(&s->subscriber);
			s->proc_handler = wamp_proc_subscribed_state;
			s->message_close_handler = wamp_subscriber_subscribed_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [UNSUBSCRIBE, Request|id, SUBSCRIBED.Subscription|id] */
		case WAMP_CODE_UNSUBSCRIBE:
			wamp_broker_unsubscribe(&s->broker);
			s->proc_handler = wamp_proc_unsubscribe_state;
			s->message_close_handler = wamp_broker_unsubscribe_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [UNSUBSCRIBED, UNSUBSCRIBE.Request|id] */
		case WAMP_CODE_UNSUBSCRIBED:
			wamp_subscriber_unsubscribed(&s->subscriber);
			s->proc_handler = wamp_proc_unsubscribed_state;
			s->message_close_handler = wamp_subscriber_unsubscribed_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list, PUBLISH.ArgumentsKw|dict] */
		case WAMP_CODE_EVENT:
			wamp_subscriber_event(&s->subscriber);
			s->proc_handler = wamp_proc_event_state;
			s->message_close_handler = wamp_subscriber_event_close;
			s->prmst = WAMP_PARAM_STATE_SUBSCRIPTION_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [CALL, Request|id, Options|dict, Procedure|uri, Arguments|list, ArgumentsKw|dict] */
		case WAMP_CODE_CALL:
			wamp_dealer_call(&s->dealer);
			s->proc_handler = wamp_proc_call_state;
			s->message_close_handler = wamp_dealer_call_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [CANCEL, CALL.Request|id, Options|dict] */
		case WAMP_CODE_CANCEL:
			wamp_dealer_cansel(&s->dealer);
			s->proc_handler = wamp_proc_cancel_state;
			s->message_close_handler = wamp_dealer_cansel_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list, YIELD.ArgumentsKw|dict] */
		case WAMP_CODE_RESULT:
			wamp_caller_result(&s->caller);
			s->proc_handler = wamp_proc_result_state;
			s->message_close_handler = wamp_caller_result_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [REGISTER, Request|id, Options|dict, Procedure|uri] */
		case WAMP_CODE_REGISTER:
			wamp_dealer_register(&s->dealer);
			s->proc_handler = wamp_proc_register_state;
			s->message_close_handler = wamp_dealer_register_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [REGISTERED, REGISTER.Request|id, Registration|id] */
		case WAMP_CODE_REGISTERED:
			wamp_callee_registered(&s->callee);
			s->proc_handler = wamp_proc_registered_state;
			s->message_close_handler = wamp_callee_registered_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [UNREGISTER, Request|id, REGISTERED.Registration|id] */
		case WAMP_CODE_UNREGISTER:
			wamp_dealer_unregister(&s->dealer);
			s->proc_handler = wamp_proc_unregister_state;
			s->message_close_handler = wamp_dealer_unregister_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [UNREGISTERED, UNREGISTER.Request|id] */
		case WAMP_CODE_UNREGISTERED:
			wamp_callee_unregistered(&s->callee);
			s->proc_handler = wamp_proc_unregistered_state;
			s->message_close_handler = wamp_callee_unregistered_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list, CALL.ArgumentsKw|dict] */
		case WAMP_CODE_INVOCATION:
			wamp_callee_invocation(&s->callee);
			s->proc_handler = wamp_proc_invocation_state;
			s->message_close_handler = wamp_callee_invocation_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [INTERRUPT, INVOCATION.Request|id, Options|dict] */
		case WAMP_CODE_INTERRUPT:
			wamp_callee_interrupt(&s->callee);
			s->proc_handler = wamp_proc_interrupt_state;
			s->message_close_handler = wamp_callee_interrupt_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
			/* [YIELD, INVOCATION.Request|id, Options|dict, Arguments|list, ArgumentsKw|dict] */
		case WAMP_CODE_YIELD:
			wamp_dealer_yield(&s->dealer);
			s->proc_handler = wamp_proc_yield_state;
			s->message_close_handler = wamp_dealer_yield_close;
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			prsst = WAMP_PARSE_STATE_ID_PUSH;
			break;
		default:
			s->proc_handler = NULL;
			break;
		}

		/* 知っているメッセージコードだったら */
		if (s->proc_handler != NULL) {
			/* パラメータ取得状態に移る */
			s->code = (wamp_code_t)code;
		}
	}

	/* メッセージコードが取得できなければエラー */
	s->state = prsst;
}

static void wamp_id_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_SPECIAL)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_list_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_LIST)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_dict_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_OBJECT)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_uri_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_STRING)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_int_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_SPECIAL)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_string_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* メッセージコードの取得開始 */
	if ((action == JSONSL_ACTION_PUSH)
		&& (state->type == JSONSL_T_STRING)) {
		s->param_handler(s, action, s->prmst, NULL);
		/* メッセージコード取得状態に移る */
		s->state = WAMP_PARSE_STATE_PARAM;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

static void wamp_param_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* コードごとの処理を実行 */
	s->proc_handler(s, action, state, buf);

	/* WAMPメッセージの末尾だったら */
	if ((state->level == 1) && (action == JSONSL_ACTION_POP)
		&& (state->type == JSONSL_T_LIST)) {
		s->prmst = WAMP_PARAM_STATE_NONE;
		s->message_close_handler(s);
		jsonsl_reset((jsonsl_t)s->jsn);
		/* 待機状態に移る */
		s->state = WAMP_PARSE_STATE_IDLE;
	}
}

static void wamp_nomore_param_parse_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	/* WAMPメッセージの末尾だったら */
	if ((state->level == 1) && (action == JSONSL_ACTION_POP)
		&& (state->type == JSONSL_T_LIST)) {
		s->message_close_handler(s);
		jsonsl_reset((jsonsl_t)s->jsn);
		/* 待機状態に移る */
		s->state = WAMP_PARSE_STATE_IDLE;
	}
	else
		/* メッセージコードが取得できなければエラー */
		s->state = WAMP_PARSE_STATE_ERROR;
}

/*
 * [HELLO, Realm|uri, Details|dict]
 */
static void wamp_proc_hello_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REALM_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_router_hello_realm(&s->router, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_router_hello_details(&s->router);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_router_hello_details_param(&s->router, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [WELCOME, Session|id, Details|dict]
 */
static void wamp_proc_welcome_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_SESSION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_client_welcome_client_id(&s->client, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_client_welcome_details(&s->client, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_client_welcome_details_param(&s->client, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [ABORT, Details|dict, Reason|uri]
 */
static void wamp_proc_abort_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_client_abort_details(&s->client, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_REASON_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_client_abort_details_param(&s->client, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_REASON_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_client_abort_reason(&s->client, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [CHALLENGE, AuthMethod|string, Extra|dict]
 */
static void wamp_proc_challenge_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_AUTHMETHOD_STRING:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_client_challenge_authmethod(&s->client, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_EXTRA_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_EXTRA_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_client_challenge_extra(&s->client, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_client_challenge_extra_param(&s->client, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [AUTHENTICATE, Signature|string, Extra|dict]
 */
static void wamp_proc_authenticate_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_SIGNATURE_STRING:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_router_authenticate_signature(&s->router, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_EXTRA_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_EXTRA_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_router_authenticate_extra(&s->router, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_router_authenticate_extra_param(&s->router, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [GOODBYE, Details|dict, Reason|uri]
 */
static void wamp_proc_goodbye_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_router_goodbye_details(&s->router);
				s->prmst = WAMP_PARAM_STATE_REASON_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_router_goodbye_details_param(&s->router, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_REASON_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_router_goodbye_reason(&s->router, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [ERROR, REQUEST.Type|int, REQUEST.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict]
 */
static void wamp_proc_error_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_TYPE_INT:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_error_type(s, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_REQUEST_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_error_request_id(s, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_error_details(s, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ERROR_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_error_details_param(s, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ERROR_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_error_uri(s, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
			s->state = WAMP_PARSE_STATE_LIST_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_error_arguments(s, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_error_arguments_param(s, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_error_argumentskw(s, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_error_argumentskw_param(s, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [PUBLISH, Request|id, Options|dict, Topic|uri, Arguments|list, ArgumentsKw|dict]
 */
static void wamp_proc_publish_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_broker_publish_request_id(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_broker_publish_options(&s->broker, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_TOPIC_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_broker_publish_options_param(&s->broker, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_TOPIC_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_broker_publish_topic(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
			s->state = WAMP_PARSE_STATE_LIST_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_broker_publish_arguments(&s->broker, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_broker_publish_arguments_param(&s->broker, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_broker_publish_argumentskw(&s->broker, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_broker_publish_argumentskw_param(&s->broker, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [PUBLISHED, PUBLISH.Request|id, Publication|id]
 */
static void wamp_proc_published_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_publisher_published_request_id(&s->publisher, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_PUBLICATION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_PUBLICATION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_publisher_published_publication_id(&s->publisher, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [SUBSCRIBE, Request|id, Options|dict, Topic|uri]
 */
static void wamp_proc_subscribe_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_broker_subscribe_request_id(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_broker_subscribe_options(&s->broker);
				s->prmst = WAMP_PARAM_STATE_TOPIC_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_broker_subscribe_options_param(&s->broker, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_TOPIC_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_broker_subscribe_topic(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [SUBSCRIBED, SUBSCRIBE.Request|id, Subscription|id]
 */
static void wamp_proc_subscribed_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_subscriber_subscribed_request_id(&s->subscriber, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_SUBSCRIPTION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_SUBSCRIPTION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_subscriber_subscribed_subscription_id(&s->subscriber, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [UNSUBSCRIBE, Request|id, SUBSCRIBED.Subscription|id]
 */
static void wamp_proc_unsubscribe_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_broker_unsubscribe_request_id(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_SUBSCRIPTION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_SUBSCRIPTION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_broker_unsubscribe_subscription_id(&s->broker, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [UNSUBSCRIBED, UNSUBSCRIBE.Request|id]
 */
static void wamp_proc_unsubscribed_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_subscriber_unsubscribed_request_id(&s->subscriber, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list, PUBLISH.ArgumentsKw|dict]
 */
static void wamp_proc_event_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_SUBSCRIPTION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_subscriber_event_subscription_id(&s->subscriber, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_PUBLICATION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_PUBLICATION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_subscriber_event_publication_id(&s->subscriber, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_subscriber_event_details(&s->subscriber, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
				s->state = WAMP_PARSE_STATE_LIST_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_subscriber_event_details_param(&s->subscriber, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_subscriber_event_arguments(&s->subscriber, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_subscriber_event_arguments_param(&s->subscriber, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_subscriber_event_argumentskw(&s->subscriber, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_subscriber_event_argumentskw_param(&s->subscriber, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [CALL, Request|id, Options|dict, Procedure|uri, Arguments|list, ArgumentsKw|dict]
 */
static void wamp_proc_call_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_call_request_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_call_options(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_PROCEDURE_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_call_options_param(&s->dealer, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_PROCEDURE_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_dealer_call_procedure(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
			s->state = WAMP_PARSE_STATE_LIST_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_dealer_call_arguments(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_call_argumentskw_param(&s->dealer, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_call_argumentskw(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_call_argumentskw_param(&s->dealer, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [CANCEL, CALL.Request|id, Options|dict]
 */
static void wamp_proc_cancel_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_cansel_request_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_cansel_options(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_cansel_options_param(&s->dealer, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list, YIELD.ArgumentsKw|dict]
 */
static void wamp_proc_result_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_caller_result_request_id(&s->caller, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_caller_result_details(&s->caller, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
				s->state = WAMP_PARSE_STATE_LIST_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_caller_result_details_param(&s->caller, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_caller_result_arguments(&s->caller, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_caller_result_arguments_param(&s->caller, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_caller_result_argumentskw(&s->caller, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_caller_result_argumentskw_param(&s->caller, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [REGISTER, Request|id, Options|dict, Procedure|uri]
 */
static void wamp_proc_register_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_register_request_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_register_options(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_PROCEDURE_URI;
				s->state = WAMP_PARSE_STATE_URI_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_register_options_param(&s->dealer, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_PROCEDURE_URI:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_STRING)) {
			wamp_dealer_register_procedure(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [REGISTERED, REGISTER.Request|id, Registration|id]
 */
static void wamp_proc_registered_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_registered_request_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_REGISTRATION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_REGISTRATION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_registered_registration_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [UNREGISTER, Request|id, REGISTERED.Registration|id]
 */
static void wamp_proc_unregister_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_unregister_request_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_REGISTRATION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_REGISTRATION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_unregister_registration_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [UNREGISTERED, UNREGISTER.Request|id]
 */
static void wamp_proc_unregistered_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_unregistered_request_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_NONE;
			s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list, CALL.ArgumentsKw|dict]
 */
static void wamp_proc_invocation_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_invocation_request_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_REGISTRATION_ID;
			s->state = WAMP_PARSE_STATE_ID_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_REGISTRATION_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_invocation_registration_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_DETAILS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_DETAILS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_callee_invocation_details(&s->callee, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
				s->state = WAMP_PARSE_STATE_LIST_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_callee_invocation_details_param(&s->callee, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_callee_invocation_arguments(&s->callee, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_callee_invocation_arguments_param(&s->callee, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_callee_invocation_argumentskw(&s->callee, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_callee_invocation_argumentskw_param(&s->callee, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [INTERRUPT, INVOCATION.Request|id, Options|dict]
 */
static void wamp_proc_interrupt_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_callee_interrupt_request_id(&s->callee, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_callee_interrupt_options(&s->callee, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_callee_interrupt_options_param(&s->callee, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

/*
 * [YIELD, INVOCATION.Request|id, Options|dict, Arguments|list, ArgumentsKw|dict]
 */
static void wamp_proc_yield_state(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	switch (s->prmst) {
	case WAMP_PARAM_STATE_REQUEST_ID:
		if ((action == JSONSL_ACTION_POP)
			&& (state->type == JSONSL_T_SPECIAL)) {
			wamp_dealer_yield_request_id(&s->dealer, s->jsn_buf);
			s->prmst = WAMP_PARAM_STATE_OPTIONS_DICT;
			s->state = WAMP_PARSE_STATE_DICT_PUSH;
		}
		else
			s->state = WAMP_PARSE_STATE_ERROR;
		break;
	case WAMP_PARAM_STATE_OPTIONS_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_yield_options(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTS_LIST;
				s->state = WAMP_PARSE_STATE_LIST_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_yield_options_param(&s->dealer, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTS_LIST:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_LIST)) {
				wamp_dealer_yield_arguments(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_ARGUMENTSKW_DICT;
				s->state = WAMP_PARSE_STATE_DICT_PUSH;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_yield_arguments_param(&s->dealer, action, state, buf);
		}
		break;
	case WAMP_PARAM_STATE_ARGUMENTSKW_DICT:
		if (state->level == 2) {
			if ((action == JSONSL_ACTION_POP)
				&& (state->type == JSONSL_T_OBJECT)) {
				wamp_dealer_yield_argumentskw(&s->dealer, s->jsn_buf);
				s->prmst = WAMP_PARAM_STATE_NONE;
				s->state = WAMP_PARSE_STATE_NOMORE_PARAM;
			}
			else
				s->state = WAMP_PARSE_STATE_ERROR;
		}
		else {
			wamp_dealer_yield_argumentskw_param(&s->dealer, action, state, buf);
		}
		break;
	default:
		s->state = WAMP_PARSE_STATE_ERROR;
		break;
	}
}

void wamp_error(wamp_state_t *s)
{
}

void wamp_error_type(wamp_state_t *s, const char *value)
{
}

void wamp_error_request_id(wamp_state_t *s, const char *value)
{
}

void wamp_error_details(wamp_state_t *s, const char *value)
{
}

void wamp_error_details_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_error_uri(wamp_state_t *s, const char *value)
{
}

void wamp_error_arguments(wamp_state_t *s, const char *value)
{
}

void wamp_error_arguments_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_error_argumentskw(wamp_state_t *s, const char *value)
{
}

void wamp_error_argumentskw_param(wamp_state_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

static void wamp_error_close(wamp_state_t *s)
{
}

static void wamp_param_handler(struct wamp_state *s, jsonsl_action_t action,
	wamp_param_state_t param, const char *value)
{
}

ER wamp_send_error_res(struct wamp_state *s, wamp_code_t code, int requestId, const char *error)
{
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return ret;
	}

	ret = fbs_printf(buf, &pos, "[8,%d,%d,{},\"%s\"]", (int)code, requestId, error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return ret;
	}

	return main_send_message(buf, s->wbsid);
}
