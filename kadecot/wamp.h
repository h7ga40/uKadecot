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

#ifndef WAMP_H
#define WAMP_H

#include "jsonsl.h"
#include "echonet_fbs.h"
#include "wamp_router.h"
#include "wamp_client.h"
#include "wamp_broker.h"
#include "wamp_publisher.h"
#include "wamp_subscriber.h"
#include "wamp_dealer.h"
#include "wamp_caller.h"
#include "wamp_callee.h"

#define JSN_NLEVELS 10

/* パースエラー */
extern const char parse_error[];
/* 無効なリクエスト */
extern const char invalid_request[];
/* 存在しないメソッド */
extern const char method_not_found[];
/* echo objectが存在しない,　JSONArray形式になっていない */
extern const char invalid_params[];
/* タイムアウト発生, コールバックが　null*/
extern const char internal_error[];

typedef enum wamp_code{
	WAMP_CODE_NONE = 0,
	WAMP_CODE_HELLO = 1,
	WAMP_CODE_WELCOME = 2,
	WAMP_CODE_ABORT = 3,
	WAMP_CODE_CHALLENGE = 4,
	WAMP_CODE_AUTHENTICATE = 5,
	WAMP_CODE_GOODBYE = 6,
	WAMP_CODE_ERROR = 8,
	WAMP_CODE_PUBLISH = 16,
	WAMP_CODE_PUBLISHED = 17,
	WAMP_CODE_SUBSCRIBE = 32,
	WAMP_CODE_SUBSCRIBED = 33,
	WAMP_CODE_UNSUBSCRIBE = 34,
	WAMP_CODE_UNSUBSCRIBED = 35,
	WAMP_CODE_EVENT = 36,
	WAMP_CODE_CALL = 48,
	WAMP_CODE_CANCEL = 49,
	WAMP_CODE_RESULT = 50,
	WAMP_CODE_REGISTER = 64,
	WAMP_CODE_REGISTERED = 65,
	WAMP_CODE_UNREGISTER = 66,
	WAMP_CODE_UNREGISTERED = 67,
	WAMP_CODE_INVOCATION = 68,
	WAMP_CODE_INTERRUPT = 69,
	WAMP_CODE_YIELD = 70,
} wamp_code_t;

typedef enum wamp_parse_state{
	WAMP_PARSE_STATE_IDLE,
	WAMP_PARSE_STATE_CODE_PUSH,
	WAMP_PARSE_STATE_CODE_POP,
	WAMP_PARSE_STATE_ID_PUSH,
	WAMP_PARSE_STATE_LIST_PUSH,
	WAMP_PARSE_STATE_DICT_PUSH,
	WAMP_PARSE_STATE_URI_PUSH,
	WAMP_PARSE_STATE_INT_PUSH,
	WAMP_PARSE_STATE_STRING_PUSH,
	WAMP_PARSE_STATE_PARAM,
	WAMP_PARSE_STATE_NOMORE_PARAM,
	WAMP_PARSE_STATE_ERROR
} wamp_parse_state_t;

typedef enum wamp_param_state
{
	WAMP_PARAM_STATE_NONE,
	WAMP_PARAM_STATE_SESSION_ID,
	WAMP_PARAM_STATE_REQUEST_ID,
	WAMP_PARAM_STATE_REGISTRATION_ID,
	WAMP_PARAM_STATE_SUBSCRIPTION_ID,
	WAMP_PARAM_STATE_PUBLICATION_ID,
	WAMP_PARAM_STATE_ARGUMENTS_LIST,
	WAMP_PARAM_STATE_OPTIONS_DICT,
	WAMP_PARAM_STATE_DETAILS_DICT,
	WAMP_PARAM_STATE_ARGUMENTSKW_DICT,
	WAMP_PARAM_STATE_EXTRA_DICT,
	WAMP_PARAM_STATE_REALM_URI,
	WAMP_PARAM_STATE_PROCEDURE_URI,
	WAMP_PARAM_STATE_REASON_URI,
	WAMP_PARAM_STATE_TOPIC_URI,
	WAMP_PARAM_STATE_ERROR_URI,
	WAMP_PARAM_STATE_TYPE_INT,
	WAMP_PARAM_STATE_AUTHMETHOD_STRING,
	WAMP_PARAM_STATE_SIGNATURE_STRING,
} wamp_param_state_t;

typedef void (* wamp_proc_handler_t)(struct wamp_state *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);

typedef void (*wamp_param_handler_t)(struct wamp_state *s, jsonsl_action_t action,
	wamp_param_state_t param, const char *value);

typedef void (*wamp_message_close_handler_t)(struct wamp_state *s);

typedef struct wamp_state
{
	char jsn[sizeof(struct jsonsl_st) + ((JSN_NLEVELS - 1) * sizeof(struct jsonsl_state_st))];
	wamp_parse_state_t state;
	wamp_param_state_t prmst;
	wamp_code_t code;
	wamp_proc_handler_t proc_handler;
	wamp_param_handler_t param_handler;
	wamp_message_close_handler_t message_close_handler;
	ID wbsid;
	char jsn_buf[80];
	int jsn_buf_pos;
	wamp_router_t router;
	wamp_client_t client;
	wamp_broker_t broker;
	wamp_dealer_t dealer;
	wamp_publisher_t publisher;
	wamp_subscriber_t subscriber;
	wamp_caller_t caller;
	wamp_callee_t callee;
} wamp_state_t;

void wamp_init(wamp_state_t *s);
void wamp_put_msg(wamp_state_t *s, ECN_FBS_ID msg, ID wbsid);
void wamp_state_callback(jsonsl_t jsn, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
int wamp_error_callback(jsonsl_t jsn, jsonsl_error_t err,
	struct jsonsl_state_st *state, char *errat);
ER wamp_send_error_res(struct wamp_state *s, wamp_code_t code,
	int requestId, const char *error);

#endif  /* WAMP_H */
