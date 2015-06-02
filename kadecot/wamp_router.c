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
#include "wamp_router.h"
#include "main.h"
#include "fbs_string.h"
#include "kernel_cfg.h"

#ifndef _MSC_VER
#ifndef strncpy_s
#define strncpy_s(dst, dsz, src, sz) strncpy(dst, src, sz)
#endif
#endif

wamp_session_t *wamp_router_hello(wamp_router_t *router)
{
	wamp_session_t *result = NULL, *pos, *end = &router->sessions[sizeof(router->sessions) / sizeof(router->sessions[0])];

	for(pos = router->sessions; pos < end; pos++) {
		if(pos->router != NULL)
			continue;
		
		result = pos;
		result->router = router;
		router->current_session = result;
	}

	return result;
}

void wamp_router_hello_realm(wamp_router_t *router, const char *value)
{
	wamp_session_t *s = router->current_session;
	strncpy_s(s->realm, sizeof(s->realm), value, sizeof(s->realm));
}

void wamp_router_hello_details_param(wamp_router_t *router, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	static const char roles[] = "roles";
	static const char publisher[] = "publisher";
	static const char subscriber[] = "subscriber";
	static const char caller[] = "caller";
	static const char callee[] = "callee";
	wamp_session_t *s = router->current_session;

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strncmp(roles, router->s->jsn_buf, sizeof(roles) - 1) == 0){
					s->roles |= WAMP_ROUTER_ROLES_IN_GETTING;
				}
				break;
			default:
				break;
			}
			break;
		}
		break;
	case 4:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if((s->roles & WAMP_ROUTER_ROLES_IN_GETTING) !=0 ){
					if(strncmp(publisher, router->s->jsn_buf, sizeof(publisher) - 1) == 0){
						s->roles |= WAMP_ROUTER_ROLES_PUBLISHER;
					}
					else if(strncmp(subscriber, router->s->jsn_buf, sizeof(subscriber) - 1) == 0){
						s->roles |= WAMP_ROUTER_ROLES_SUBSCRIBER;
					}
					else if(strncmp(caller, router->s->jsn_buf, sizeof(caller) - 1) == 0){
						s->roles |= WAMP_ROUTER_ROLES_CALLER;
					}
					else if(strncmp(callee, router->s->jsn_buf, sizeof(callee) - 1) == 0){
						s->roles |= WAMP_ROUTER_ROLES_CALLEE;
					}
				}
				break;
			default:
				break;
			}
			break;
		}
		break;
	}
}

void wamp_router_hello_details(wamp_router_t *router)
{
	wamp_session_t *s = router->current_session;
	s->roles &= ~WAMP_ROUTER_ROLES_IN_GETTING;
}

void wamp_router_hello_close(struct wamp_state *s)
{
	wamp_router_t *router = &s->router;
	wamp_session_t *session = router->current_session;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	ER ret;

	router->session_id++;
	if(router->session_id == 0){
		router->session_id++;
	}
	session->id = router->session_id;

	// send WELLCOM or ABORT
	_ecn_fbs_cre(16, &buf);

	ret = fbs_printf(buf, &pos, "[2,%d,{\"roles\":{", session->id);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	start = pos;
	if(session->roles & WAMP_ROUTER_ROLES_PUBLISHER){
		ret = fbs_printf(buf, &pos, "\"publisher\":{},");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}
	if(session->roles & WAMP_ROUTER_ROLES_SUBSCRIBER){
		ret = fbs_printf(buf, &pos, "\"subscriber\":{},");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}
	if(session->roles & WAMP_ROUTER_ROLES_CALLER){
		ret = fbs_printf(buf, &pos, "\"caller\":{},");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}
	if(session->roles & WAMP_ROUTER_ROLES_CALLEE){
		ret = fbs_printf(buf, &pos, "\"callee\":{},");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "}}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_router_hello_close() : main_send_message() result = %d", ret);
	}
}

void wamp_router_authenticate(wamp_router_t *router)
{
}

void wamp_router_authenticate_signature(wamp_router_t *router, const char *value)
{
}

void wamp_router_authenticate_extra(wamp_router_t *router, const char *value)
{
}

void wamp_router_authenticate_extra_param(wamp_router_t *router, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_router_authenticate_close(struct wamp_state *s)
{
}

void wamp_router_goodbye(wamp_router_t *router)
{
}

void wamp_router_goodbye_details_param(wamp_router_t *router, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_router_goodbye_details(wamp_router_t *router)
{
}

void wamp_router_goodbye_reason(wamp_router_t *router, const char *value)
{
}

void wamp_router_goodbye_close(struct wamp_state *s)
{
	wamp_router_t *router = &s->router;
	wamp_session_t *session = router->current_session;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	if(session == NULL)
		return;

	// send GOODBYE
	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[6,{},\"wamp.error.goodbye_and_out\"]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_router_goodbye_close() : main_send_message() result = %d", ret);
	}

	memset(session, 0, sizeof(session));
}
