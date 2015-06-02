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

#ifndef WAMP_CALLEE_H
#define WAMP_CALLEE_H

typedef struct wamp_callee
{
	struct wamp_state *s;
	int id;
} wamp_callee_t;

void wamp_callee_registered(wamp_callee_t *callee);
void wamp_callee_registered_request_id(wamp_callee_t *s, const char *value);
void wamp_callee_registered_registration_id(wamp_callee_t *s, const char *value);
void wamp_callee_registered_close(struct wamp_state *s);

void wamp_callee_unregistered(wamp_callee_t *callee);
void wamp_callee_unregistered_request_id(wamp_callee_t *s, const char *value);
void wamp_callee_unregistered_close(struct wamp_state *s);

void wamp_callee_invocation(wamp_callee_t *callee);
void wamp_callee_invocation_request_id(wamp_callee_t *s, const char *value);
void wamp_callee_invocation_registration_id(wamp_callee_t *s, const char *value);
void wamp_callee_invocation_details(wamp_callee_t *s, const char *value);
void wamp_callee_invocation_details_param(wamp_callee_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_callee_invocation_arguments(wamp_callee_t *s, const char *value);
void wamp_callee_invocation_arguments_param(wamp_callee_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_callee_invocation_argumentskw(wamp_callee_t *s, const char *value);
void wamp_callee_invocation_argumentskw_param(wamp_callee_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_callee_invocation_close(struct wamp_state *s);

void wamp_callee_interrupt(wamp_callee_t *callee);
void wamp_callee_interrupt_request_id(wamp_callee_t *s, const char *value);
void wamp_callee_interrupt_options(wamp_callee_t *s, const char *value);
void wamp_callee_interrupt_options_param(wamp_callee_t *s, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_callee_interrupt_close(struct wamp_state *s);

#endif // WAMP_CALLEE_H
