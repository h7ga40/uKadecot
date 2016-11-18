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
#include "wamp_dealer.h"
#include "main.h"
#include "fbs_string.h"
#include "kadecot_names.h"
#include "kernel_cfg.h"
#include "echonet_agent.h"
#include "arduino.h"
#include "data_flash.h"
#include "net/ip/uip.h"
#include "uip_adpt.h"

#ifndef _MSC_VER
#ifndef strcpy_s
#define strcpy_s(s1, s1m, s2) strcpy(s1, s2)
#endif

#ifndef strncpy_s
#define strncpy_s(dst, dsz, src, sz) strncpy(dst, src, sz)
#endif

size_t strnlen(const char *s, size_t maxlen);
#endif

static void wamp_dealer_call_dummy_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf);
static void wamp_dealer_call_kadecot_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf);
static void wamp_dealer_call_ecnl_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf);
static void wamp_dealer_call_arduino_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf);
static void wamp_dealer_call_init_params_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf);

static void wamp_dealer_none_close(struct wamp_dealer *dealer);
static void wamp_dealer_get_devicelist_close(struct wamp_dealer *dealer);
static void wamp_dealer_kadecot_get_close(struct wamp_dealer *dealer);
static void wamp_dealer_kadecot_set_close(struct wamp_dealer *dealer);
static void wamp_dealer_get_deviceinfo_close(struct wamp_dealer *dealer);
static void wamp_dealer_ecnl_set_close(struct wamp_dealer *dealer);
static void wamp_dealer_ecnl_get_close(struct wamp_dealer *dealer);
static void wamp_dealer_ecnl_setget_close(struct wamp_dealer *dealer);
static void wamp_dealer_pin_mode_close(struct wamp_dealer *dealer);
static void wamp_dealer_digital_write_close(struct wamp_dealer *dealer);
static void wamp_dealer_digital_read_close(struct wamp_dealer *dealer);
static void wamp_dealer_analog_read_close(struct wamp_dealer *dealer);
static void wamp_dealer_analog_write_close(struct wamp_dealer *dealer);
static void wamp_dealer_set_init_params_close(struct wamp_dealer *dealer);
static void wamp_dealer_get_init_params_close(struct wamp_dealer *dealer);

static echonetlite_device_t *wamp_dealer_get_device(wamp_dealer_t *dealer, int deviceId);

static bool_t wamp_dealer_get_epc(const char *propName, int nameLen,
	uint16_t devType, uint8_t *epc, uint8_t *pdc);
static void wamp_dealer_get_devicelist(struct wamp_dealer *dealer);

int wamp_stob(const char *str, uint8_t *dst, int dstc);

void wamp_dealer_register(wamp_dealer_t *dealer)
{
}

void wamp_dealer_register_request_id(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_register_options(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_register_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_register_procedure(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_register_close(struct wamp_state *s)
{
}

void wamp_dealer_unregister(wamp_dealer_t *dealer)
{
}

void wamp_dealer_unregister_request_id(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_unregister_registration_id(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_unregister_close(struct wamp_state *s)
{
}

void wamp_dealer_call(wamp_dealer_t *dealer)
{
	dealer->args_parse = wamp_dealer_call_dummy_param;
	dealer->call_close = wamp_dealer_none_close;

	memset(&dealer->params, 0, sizeof(dealer->params));
}

void wamp_dealer_call_request_id(wamp_dealer_t *dealer, const char *value)
{
	dealer->requestId = atoi(value);
}

void wamp_dealer_call_options(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_call_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	static const char deviceId[] = "deviceId";

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(deviceId, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_DEVICE_ID;
				}
				break;
			case JSONSL_T_SPECIAL:
				dealer->current_device = wamp_dealer_get_device(dealer, atoi(dealer->s->jsn_buf));
				break;
			default:
				break;
			}
			break;
		}
		break;
	}
}

void wamp_dealer_call_procedure(wamp_dealer_t *dealer, const char *value, int length)
{
	static const char comSonycslKadecot[] = "com.sonycsl.kadecot.";
	static const char jpToppersEcnl[] = "jp.toppers.ecnl.";
	const char *start = value;

	if (strncmp(comSonycslKadecot, value, sizeof(comSonycslKadecot) - 1) == 0) {
		static const char provider[] = "provider.procedure.";
		static const char echonetlite[] = "echonetlite.procedure.";
		static const char arduino[] = "arduino.";
		value = &value[sizeof(comSonycslKadecot) - 1];

		if (strncmp(provider, value, sizeof(provider) - 1) == 0) {
			static const char getDeviceList[] = "getDeviceList";
			value = &value[sizeof(provider) - 1];

			if (strncmp(getDeviceList, value, sizeof(getDeviceList) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_GET_DEVICELIST;
				dealer->args_parse = wamp_dealer_call_dummy_param;
				dealer->call_close = wamp_dealer_get_devicelist_close;
				return;
			}
		}
		else if (strncmp(echonetlite, value, sizeof(echonetlite) - 1) == 0) {
			static const char kadecotGet[] = "get";
			static const char kadecotSet[] = "set";
			value = &value[sizeof(echonetlite) - 1];

			if (strncmp(kadecotGet, value, sizeof(kadecotGet) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_KADECOT_GET;
				dealer->args_parse = wamp_dealer_call_kadecot_param;
				dealer->call_close = wamp_dealer_kadecot_get_close;
				return;
			}
			else if (strncmp(kadecotSet, value, sizeof(kadecotSet) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_KADECOT_SET;
				dealer->args_parse = wamp_dealer_call_kadecot_param;
				dealer->call_close = wamp_dealer_kadecot_set_close;
				return;
			}

			for (int i = value - start; i < length; i++) {
				if (*value == '.' || *value == '\0') {
					value++;
					break;
				}
				value++;
			}

			if (strncmp(kadecotGet, value, sizeof(kadecotGet) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_KADECOT_GET;
				dealer->args_parse = wamp_dealer_call_kadecot_param;
				dealer->call_close = wamp_dealer_kadecot_get_close;
				return;
			}
			else if (strncmp(kadecotSet, value, sizeof(kadecotSet) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_KADECOT_SET;
				dealer->args_parse = wamp_dealer_call_kadecot_param;
				dealer->call_close = wamp_dealer_kadecot_set_close;
				return;
			}
		}
		else if (strncmp(arduino, value, sizeof(arduino) - 1) == 0) {
			static const char pinMode[] = "pinMode";
			static const char digitalWrite[] = "digitalWrite";
			static const char digitalRead[] = "digitalRead";
			static const char analogRead[] = "analogRead";
			static const char analogWrite[] = "analogWrite";
			value = &value[sizeof(arduino) - 1];

			if (strncmp(pinMode, value, sizeof(pinMode) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_PIN_MODE;
				dealer->args_parse = wamp_dealer_call_arduino_param;
				dealer->call_close = wamp_dealer_pin_mode_close;
				return;
			}
			else if (strncmp(digitalWrite, value, sizeof(digitalWrite) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_DIGITAL_WRITE;
				dealer->args_parse = wamp_dealer_call_arduino_param;
				dealer->call_close = wamp_dealer_digital_write_close;
				return;
			}
			else if (strncmp(digitalRead, value, sizeof(digitalRead) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_DIGITAL_READ;
				dealer->args_parse = wamp_dealer_call_arduino_param;
				dealer->call_close = wamp_dealer_digital_read_close;
				return;
			}
			else if (strncmp(analogRead, value, sizeof(analogRead) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_ANALOG_READ;
				dealer->args_parse = wamp_dealer_call_arduino_param;
				dealer->call_close = wamp_dealer_analog_read_close;
				return;
			}
			else if (strncmp(analogWrite, value, sizeof(analogWrite) - 1) == 0) {
				dealer->procedure = WAMP_DEALER_PROCEDURE_ANALOG_WRITE;
				dealer->args_parse = wamp_dealer_call_arduino_param;
				dealer->call_close = wamp_dealer_analog_write_close;
				return;
			}
		}
	}
	else if (strncmp(jpToppersEcnl, value, sizeof(jpToppersEcnl) - 1) == 0) {
		static const char getDeviceInfo[] = "procedure.getDeviceInfo";
		static const char ecnlSet[] = "procedure.set";
		static const char ecnlGet[] = "procedure.get";
		static const char ecnlSetGet[] = "procedure.setget";
		static const char setInitParams[] = "procedure.setInitParams";
		static const char getInitParams[] = "procedure.getInitParams";

		if (strncmp(getDeviceInfo, value, sizeof(getDeviceInfo) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_GET_DEVICEINFO;
			dealer->args_parse = wamp_dealer_call_dummy_param;
			dealer->call_close = wamp_dealer_get_deviceinfo_close;
			return;
		}
		else if (strncmp(ecnlSet, value, sizeof(ecnlSet) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_ECNL_SET;
			dealer->args_parse = wamp_dealer_call_ecnl_param;
			dealer->call_close = wamp_dealer_ecnl_set_close;
			return;
		}
		else if (strncmp(ecnlGet, value, sizeof(ecnlGet) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_ECNL_GET;
			dealer->args_parse = wamp_dealer_call_ecnl_param;
			dealer->call_close = wamp_dealer_ecnl_get_close;
			return;
		}
		else if (strncmp(ecnlSetGet, value, sizeof(ecnlSetGet) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_ECNL_SETGET;
			dealer->args_parse = wamp_dealer_call_ecnl_param;
			dealer->call_close = wamp_dealer_ecnl_setget_close;
			return;
		}
		else if (strncmp(setInitParams, value, sizeof(setInitParams) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_SET_INIT_PARAMS;
			dealer->args_parse = wamp_dealer_call_init_params_param;
			dealer->call_close = wamp_dealer_set_init_params_close;
			return;
		}
		else if (strncmp(getInitParams, value, sizeof(getInitParams) - 1) == 0) {
			dealer->procedure = WAMP_DEALER_PROCEDURE_GET_INIT_PARAMS;
			dealer->args_parse = wamp_dealer_call_init_params_param;
			dealer->call_close = wamp_dealer_get_init_params_close;
			return;
		}
	}

	dealer->procedure = WAMP_DEALER_PROCEDURE_NONE;
	dealer->args_parse = wamp_dealer_call_dummy_param;
	dealer->call_close = wamp_dealer_none_close;
}

void wamp_dealer_call_arguments(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_call_arguments_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_call_argumentskw(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_call_argumentskw_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
	dealer->args_parse(dealer, action, state, buf);
}

static void wamp_dealer_call_dummy_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf)
{
}

static void wamp_dealer_call_kadecot_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf)
{
	static const char propertyName[] = "propertyName";
	static const char propertyValue[] = "propertyValue";

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(propertyName, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PROPERTY_NAME;
				}
				else if(strcmp(propertyValue, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PROPERTY_VALUE;
					dealer->params.pdc = 0;
					memset(dealer->params.edt, 0, sizeof(dealer->params.edt));
				}
				break;
			case JSONSL_T_STRING:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PROPERTY_NAME:
					memcpy(dealer->params.prop_name, dealer->s->jsn_buf, sizeof(dealer->params.prop_name));
					wamp_dealer_get_epc(dealer->params.prop_name, strnlen(dealer->params.prop_name,
						sizeof(dealer->params.prop_name)),
						dealer->current_device->deviceType,
						&dealer->params.epc, &dealer->params.pdc);
					break;
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
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PROPERTY_VALUE:
					dealer->params.edt[dealer->params.pdc++] = atoi(dealer->s->jsn_buf);
					break;
				}
				break;
			}
			break;
		}
		break;
	}
}

static void wamp_dealer_call_ecnl_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf)
{
	static const char deojId[] = "deojid";
	static const char properties[] = "properties";
	static const char propertyCode[] = "epc";
	static const char propertyValue[] = "edt";

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(deojId, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_DEOJID;
				}
				else if(strcmp(properties, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PROPERTIES;
				}
				break;
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_DEOJID:
					dealer->params.deojid = atoi(dealer->s->jsn_buf);
					break;
				}
				break;
			}
			break;
		case JSONSL_ACTION_PUSH:
			switch (state->type)
			{
			case JSONSL_T_LIST:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PROPERTIES:
					dealer->state = WAMP_DEALER_STATE_PROPERTY_LIST;
					break;
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
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				}
				break;
			default:
				break;
			}
			break;
		}
		break;
	case 5:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(propertyCode, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PROPERTY_CODE;
				}
				else if(strcmp(propertyValue, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PROPERTY_VALUE;
					dealer->params.pdc = 0;
					memset(dealer->params.edt, 0, sizeof(dealer->params.edt));
				}
				break;
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PROPERTY_CODE:
					dealer->params.epc = atoi(dealer->s->jsn_buf);
					switch(dealer->procedure){
					case WAMP_DEALER_PROCEDURE_ECNL_GET:
						if (dealer->ecnl_mag == NULL){
							ecn_esv_get(&dealer->ecnl_mag, dealer->params.deojid, dealer->params.epc);
						}
						else{
							ecn_add_epc(dealer->ecnl_mag, dealer->params.epc);
						}
						break;
					case WAMP_DEALER_PROCEDURE_ECNL_SETGET:
						ecn_add_epc(dealer->ecnl_mag, dealer->params.epc);
						break;
					}
					break;
				}
				break;
			case JSONSL_T_STRING:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PROPERTY_VALUE:
					dealer->params.pdc = wamp_stob(dealer->s->jsn_buf, dealer->params.edt, sizeof(dealer->params.edt));
					switch(dealer->procedure){
					case WAMP_DEALER_PROCEDURE_ECNL_SET:
						if (dealer->ecnl_mag == NULL){
							ecn_esv_setc(&dealer->ecnl_mag, dealer->params.deojid, dealer->params.epc,
								dealer->params.pdc, dealer->params.edt);
						}
						else{
							ecn_add_edt(dealer->ecnl_mag, dealer->params.epc,
								dealer->params.pdc, dealer->params.edt);
						}
						break;
					case WAMP_DEALER_PROCEDURE_ECNL_SETGET:
						if (dealer->ecnl_mag == NULL){
							ecn_esv_set_get(&dealer->ecnl_mag, dealer->params.deojid, dealer->params.epc,
								dealer->params.pdc, dealer->params.edt);
						}
						else{
							ecn_add_edt(dealer->ecnl_mag, dealer->params.epc,
								dealer->params.pdc, dealer->params.edt);
						}
						break;
					}
					break;
				}
				break;
			}
			break;
		}
		break;
	}
}

static void wamp_dealer_call_arduino_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf)
{
	static const char pin[] = "pin";
	static const char mode[] = "mode";
	static const char value[] = "value";
	static const char INPUT[] = "INPUT";
	static const char OUTPUT[] = "OUTPUT";
	static const char INPUT_PULLUP[] = "INPUT_PULLUP";
	static const char HIGH[] = "HIGH";
	static const char LOW[] = "LOW";

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(pin, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_PIN;
				}
				else if(strcmp(mode, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_MODE;
				}
				else if(strcmp(value, dealer->s->jsn_buf) == 0){
					if(dealer->procedure == WAMP_DEALER_PROCEDURE_DIGITAL_WRITE)
						dealer->state = WAMP_DEALER_STATE_DIGITAL_VALUE;
					else if(dealer->procedure == WAMP_DEALER_PROCEDURE_ANALOG_WRITE)
						dealer->state = WAMP_DEALER_STATE_ANALOG_VALUE;
				}
				break;
			case JSONSL_T_STRING:
				switch(dealer->state){
				case WAMP_DEALER_STATE_MODE:
					if(strcmp(INPUT, dealer->s->jsn_buf) == 0){
						dealer->arduino_params.mode = WAMP_DEALER_PIN_MODE_INPUT;
					}
					else if(strcmp(OUTPUT, dealer->s->jsn_buf) == 0){
						dealer->arduino_params.mode = WAMP_DEALER_PIN_MODE_OUTPUT;
					}
					else if(strcmp(INPUT_PULLUP, dealer->s->jsn_buf) == 0){
						dealer->arduino_params.mode = WAMP_DEALER_PIN_MODE_INPUT_PULLUP;
					}
					break;
				case WAMP_DEALER_STATE_DIGITAL_VALUE:
					if(strcmp(HIGH, dealer->s->jsn_buf) == 0){
						dealer->arduino_params.digval = WAMP_DEALER_DIGITAL_VALUE_HIGH;
					}
					else if(strcmp(LOW, dealer->s->jsn_buf) == 0){
						dealer->arduino_params.digval = WAMP_DEALER_DIGITAL_VALUE_LOW;
					}
					break;
				}
				break;
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_PIN:
					dealer->arduino_params.pin = atoi(dealer->s->jsn_buf);
					break;
				case WAMP_DEALER_STATE_ANALOG_VALUE:
					dealer->arduino_params.anaval = atoi(dealer->s->jsn_buf);
					break;
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

static void wamp_dealer_call_init_params_param(wamp_dealer_t *dealer,
	jsonsl_action_t action, struct jsonsl_state_st *state, const char *buf)
{
	static const char macAddr[] = "macAddr";
	static const char dhcpEnable[] = "dhcpEnable";
	static const char ipAddr[] = "ipAddr";
	static const char netmask[] = "netmask";
	static const char gateway[] = "gateway";

	switch (state->level) {
	case 3:
		switch (action){
		case JSONSL_ACTION_POP:
			switch (state->type)
			{
			case JSONSL_T_HKEY:
				if(strcmp(macAddr, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_MAC_ADDR_VALUE;
					dealer->init_params.pos = 0;
					memset(&dealer->init_params.mac_addr, 0, sizeof(dealer->init_params.mac_addr));
				}
				if(strcmp(dhcpEnable, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_DHCP_ENABLE_VALUE;
					dealer->init_params.dhcp_enable = false;
				}
				if(strcmp(ipAddr, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_IP_ADDR_VALUE;
					dealer->init_params.pos = 0;
					memset(&dealer->init_params.ip_addr, 0, sizeof(dealer->init_params.ip_addr));
				}
				if(strcmp(netmask, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_NETMASK_VALUE;
					dealer->init_params.pos = 0;
					memset(&dealer->init_params.netmask, 0, sizeof(dealer->init_params.netmask));
				}
				if(strcmp(gateway, dealer->s->jsn_buf) == 0){
					dealer->state = WAMP_DEALER_STATE_GATEWAY_VALUE;
					dealer->init_params.pos = 0;
					memset(&dealer->init_params.gateway, 0, sizeof(dealer->init_params.gateway));
				}
				break;
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_DHCP_ENABLE_VALUE:
					dealer->init_params.dhcp_enable = strcmp("true", dealer->s->jsn_buf) == 0;
					break;
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
			case JSONSL_T_SPECIAL:
				switch(dealer->state){
				case WAMP_DEALER_STATE_MAC_ADDR_VALUE:
					dealer->init_params.mac_addr[dealer->init_params.pos++] = atoi(dealer->s->jsn_buf);
					break;
				case WAMP_DEALER_STATE_IP_ADDR_VALUE:
					dealer->init_params.ip_addr[dealer->init_params.pos++] = atoi(dealer->s->jsn_buf);
					break;
				case WAMP_DEALER_STATE_NETMASK_VALUE:
					dealer->init_params.netmask[dealer->init_params.pos++] = atoi(dealer->s->jsn_buf);
					break;
				case WAMP_DEALER_STATE_GATEWAY_VALUE:
					dealer->init_params.gateway[dealer->init_params.pos++] = atoi(dealer->s->jsn_buf);
					break;
				}
				break;
			}
			break;
		}
		break;
	}
}

void wamp_dealer_call_close(struct wamp_state *s)
{
	wamp_dealer_t *dealer = &s->dealer;
	ER ret;

	dealer->error = internal_error;

	// send RESULT
	dealer->call_close(dealer);

	if (dealer->error != NULL){
		ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, dealer->error);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "wamp_dealer_call_close() : wamp_send_error_res() result = %d", ret);
		}
		dealer->error = NULL;
	}
}

static void wamp_dealer_none_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	dealer->error = NULL;
	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, invalid_request);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_none_close() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_get_devicelist_close(struct wamp_dealer *dealer)
{
	dealer->error = NULL;
	main_get_device_list(dealer->requestId, dealer);
}

void wamp_dealer_set_devicelist(struct wamp_dealer *dealer, ECN_FBS_ID msg)
{
	wamp_state_t *s = dealer->s;
	ecn_inm_get_device_item_t device;
	ECN_FBS_SSIZE_T rlen;
	echonetlite_device_t *dev, *end = &dealer->devices[sizeof(dealer->devices) / sizeof(dealer->devices[0])];
	ER ret;

	/* 機器情報をクリア */
	for(dev = dealer->devices; dev < end; dev++){
		dev->deviceId = 0;
	}

	/* メッセージから機器情報を作成 */
	for(dev = dealer->devices; dev < end; ){
		ret = _ecn_fbs_get_data(msg, &device, sizeof(device), &rlen);
		if ((ret != E_OK) || (rlen != sizeof(device)))
			break;

		if (device.enodid == 0)
			continue;

		dev->deviceId = device.eobjid;
		dev->nodeId = device.enodid;
		dev->addrId = device.addrid;
		dev->deviceType = (device.eojx1 << 8) | device.eojx2;
		dev->x3 = device.eojx3;
		dev->status = device.state != 0;

		dev++;
	}

	dealer->current_device = &dealer->devices[0];
	dealer->devices_pos = 0;
	main_get_device_ipaddr(dealer->requestId, dealer, dealer->current_device->addrId);
}

void wamp_dealer_get_devicelist_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_get_devicelist_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

void wamp_dealer_set_ipaddr(struct wamp_dealer *dealer, char *ipaddr)
{
	echonetlite_device_t *dev, *end = &dealer->devices[sizeof(dealer->devices) / sizeof(dealer->devices[0])];
	int i, devicePos;

	dev = dealer->current_device;
	if ((dev != NULL) && (ipaddr != NULL)){
		strcpy_s(dev->ip_addr, sizeof(dev->ip_addr), ipaddr);

		kadecot_names_get_device_type_name(dev->deviceType, dev->deviceTypeStr, sizeof(dev->deviceTypeStr));
		strcpy_s(dev->description, sizeof(dev->description), dev->deviceTypeStr);
		strcpy_s(dev->nickname, sizeof(dev->nickname), dev->deviceTypeStr);
	}

	devicePos = sizeof(dealer->devices) / sizeof(dealer->devices[0]);
	for(i = dealer->devices_pos + 1, dev = &dealer->devices[i]; dev < end; i++, dev++){
		if(dev->deviceId == 0)
			continue;
		
		devicePos = i;
		break;
	}

	if (devicePos == sizeof(dealer->devices) / sizeof(dealer->devices[0])){
		dealer->current_device = NULL;
		dealer->devices_pos = 0;
		wamp_dealer_get_devicelist(dealer);
	}
	else{
		dealer->current_device = &dealer->devices[devicePos];
		dealer->devices_pos = devicePos;
		main_get_device_ipaddr(dealer->requestId, dealer, dealer->current_device->addrId);
	}
}

void wamp_dealer_get_ipaddr_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_get_ipaddr_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_get_devicelist(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	echonetlite_device_t *dev, *end = &dealer->devices[sizeof(dealer->devices) / sizeof(dealer->devices[0])];
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	char ip_addr[16];
	ER ret;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{},[],", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "{\"deviceList\":["
		"{\"description\":\"GR-SAKURA\",\"deviceId\":1,\"deviceType\":\"GrSakura\",\"ip_addr\":\"%s\",\"nickname\":\"GR-SAKURA\",\"protocol\":\"arduino\",\"status\":true},",
		ip2str(ip_addr, uip_hostaddr));
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	start = pos;
	for(dev = dealer->devices; dev < end; dev++){
		if(dev->deviceId == 0)
			continue;

		ret = fbs_printf(buf, &pos, "{\"description\":\"%s\",\"deviceId\":%d,\"deviceType\":\"%s\",\"ip_addr\":\"%s\",\"nickname\":\"%s\",\"protocol\":\"echonetlite\",\"status\":%s},",
			dev->description, dev->deviceId, dev->deviceTypeStr, dev->ip_addr, dev->nickname, dev->status ? "true" : "false");
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_get_devicelist() : main_send_message() result = %d", ret);
	}
}

static void wamp_dealer_kadecot_get_close(struct wamp_dealer *dealer)
{
	echonetlite_device_t *device = dealer->current_device;
	uint16_t devType;

	if(device == NULL)
		return;

	devType = device->deviceType;
	if(devType == 0)
		return;

	if (dealer->params.epc == 0) {
		dealer->error = method_not_found;
		syslog(7, method_not_found);
		return;
	}

	dealer->error = NULL;
	main_kadecot_get(dealer->requestId, dealer, device->deviceId, dealer->params.epc);
}

void wamp_dealer_kadecot_get(struct wamp_dealer *dealer, int deviceId, uint8_t epc,
	uint8_t pdc, uint8_t *edt)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	int i;
	const char *propName;
	ER ret;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	propName = dealer->params.prop_name;
	if(propName[0] != '\0'){
		ret = fbs_printf(buf, &pos, "{\"propertyName\":\"%s\",\"propertyValue\":[",
			propName);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}
	else{
		ret = fbs_printf(buf, &pos, "{\"propertyName\":\"0x%02x\",\"propertyValue\":[",
			epc);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	start = pos;
	for(i = 0; i < pdc; i++){
		ret = fbs_printf(buf, &pos, "%d,", edt[i]);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_kadecot_get() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_kadecot_set_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_kadecot_set_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_kadecot_set_close(struct wamp_dealer *dealer)
{
	echonetlite_device_t *device = dealer->current_device;
	uint16_t devType;

	if(device == NULL)
		return;

	devType = device->deviceType;
	if(devType == 0)
		return;

	if(dealer->params.epc == 0){
		dealer->error = method_not_found;
		return;
	}

	dealer->error = NULL;
	main_kadecot_set(dealer->requestId, dealer, device->deviceId, dealer->params.epc,
		dealer->params.pdc, dealer->params.edt);
}

void wamp_dealer_kadecot_set(struct wamp_dealer *dealer, int deviceId, uint8_t epc)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	int i;
	const char *propName;
	ER ret;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	propName = dealer->params.prop_name;
	if(propName[0] != '\0'){
		ret = fbs_printf(buf, &pos, "{\"propertyName\":\"%s\",\"propertyValue\":[",
			propName);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}
	else{
		ret = fbs_printf(buf, &pos, "{\"propertyName\":\"0x%02x\",\"propertyValue\":[",
			epc);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	start = pos;
	for(i = 0; i < dealer->params.pdc; i++){
		ret = fbs_printf(buf, &pos, "%d,", dealer->params.edt[i]);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_kadecot_set() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_kadecot_get_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_kadecot_get_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_get_deviceinfo_close(struct wamp_dealer *dealer)
{
	echonetlite_device_t *device = dealer->current_device;
	if(device == NULL)
		return;

	dealer->error = NULL;
	main_get_device_info(dealer->requestId, dealer, device->deviceId);
}

void wamp_dealer_set_deviceinfo(struct wamp_dealer *dealer, int deviceId, uint8_t *pmset,
	uint8_t *pmget, uint8_t *pmanno)
{
	wamp_state_t *s = dealer->s;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	int i, j, m, prop;
	ER ret;

	if(device == NULL)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "{\"x1\":%d,\"x2\":%d,\"x3\":%d,\"enodid\":%d,\"properties\":[",
		device->deviceType >> 8, device->deviceType & 0xFF, device->x3, device->nodeId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	prop = pos;
	for(i = 0; i < 16; i++){
		uint8_t pms = pmset[i], pmg = pmget[i], pma = pmanno[i], exs = pms | pmg | pma;

		for(j = 0; j < 8; j++){
			m = 1 << j;
			if(!(exs & m))
				continue;

			ret = fbs_printf(buf, &pos, "{\"epc\":%d,\"flag\":\"", 0x80 | (j << 4) | i);
			if (ret != E_OK) {
				syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
				_ecn_fbs_del(buf);
				return;
			}
			if (pms & m) {
				ret = fbs_printf(buf, &pos, "RULE_SET,");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			if (pmg & m) {
				ret = fbs_printf(buf, &pos, "RULE_GET,");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			if (pma & m) {
				ret = fbs_printf(buf, &pos, "ANNOUNCE,");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			/* 最後の","を消す */
			pos--;

			ret = fbs_printf(buf, &pos, "\"},");
			if (ret != E_OK) {
				syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
				_ecn_fbs_del(buf);
				return;
			}
		}
	}

	/* 最後の","を消す */
	if(prop != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_set_deviceinfo() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_get_deviceinfo_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_get_deviceinfo_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_ecnl_set_close(struct wamp_dealer *dealer)
{
	ER ret;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf = { (T_ECN_FST_BLK *)dealer->ecnl_mag };

	if (dealer->ecnl_mag == NULL)
		return;

	if(device == NULL){
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_ecnl_set(dealer->requestId, dealer, device->deviceId, dealer->ecnl_mag);
	if (ret == E_OK) {
		dealer->error = NULL;
	}
	else {
		_ecn_fbs_del(buf);
	}

	dealer->ecnl_mag = NULL;
}

void wamp_dealer_ecnl_set_res(struct wamp_dealer *dealer, T_EDATA *esv)
{
	wamp_state_t *s = dealer->s;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;
	int j;
	ER ret;

	if(device == NULL)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, device->deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* プロパティ値書き込み応答 0x71 */
	ret = fbs_printf(buf, &pos, "{\"esv\":113,\"sender\":%d,\"seoj\":\"%02x%02x%02x\",\"deoj\":\"%02x%02x%02x\",\"properties\":[",
		ecn_get_enod(esv), esv->hdr.edata.seoj.eojx1, esv->hdr.edata.seoj.eojx2, esv->hdr.edata.seoj.eojx3,
		esv->hdr.edata.deoj.eojx1, esv->hdr.edata.deoj.eojx2, esv->hdr.edata.deoj.eojx3);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK){
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	start = pos;
	for(;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			if (pdc == 0) {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d},", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			else {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d,\"edt\":\"", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
				for(j = 0; j < pdc; j++){
					ret = fbs_printf(buf, &pos, "%02x", p_edt[j]);
					if (ret != E_OK) {
						syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
						_ecn_fbs_del(buf);
						return;
					}
				}
				ret = fbs_printf(buf, &pos, "\"},");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
		}
		if(ret != E_BOVR){
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if(enm.is_eof)
			break;
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_set_res() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_ecnl_set_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_set_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_ecnl_get_close(struct wamp_dealer *dealer)
{
	ER ret;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf = { (T_ECN_FST_BLK *)dealer->ecnl_mag };

	if (dealer->ecnl_mag == NULL)
		return;

	if(device == NULL){
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_ecnl_get(dealer->requestId, dealer, device->deviceId, dealer->ecnl_mag);
	if (ret == E_OK) {
		dealer->error = NULL;
	}
	else {
		_ecn_fbs_del(buf);
	}

	dealer->ecnl_mag = NULL;
}

void wamp_dealer_ecnl_get_res(struct wamp_dealer *dealer, T_EDATA *esv)
{
	wamp_state_t *s = dealer->s;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;
	int j;
	ER ret;

	if(device == NULL)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, device->deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* プロパティ値読み込み応答 0x72 */
	ret = fbs_printf(buf, &pos, "{\"esv\":114,\"sender\":%d,\"seoj\":\"%02x%02x%02x\",\"deoj\":\"%02x%02x%02x\",\"properties\":[",
		ecn_get_enod(esv), esv->hdr.edata.seoj.eojx1, esv->hdr.edata.seoj.eojx2, esv->hdr.edata.seoj.eojx3,
		esv->hdr.edata.deoj.eojx1, esv->hdr.edata.deoj.eojx2, esv->hdr.edata.deoj.eojx3);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK){
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	start = pos;
	for(;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			if (pdc == 0) {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d},", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			else {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d,\"edt\":\"", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
				for(j = 0; j < pdc; j++){
					ret = fbs_printf(buf, &pos, "%02x", p_edt[j]);
					if (ret != E_OK) {
						syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
						_ecn_fbs_del(buf);
						return;
					}
				}
				ret = fbs_printf(buf, &pos, "\"},");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
		}
		if(ret != E_BOVR){
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if(enm.is_eof)
			break;
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_get_res() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_ecnl_get_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_get_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_ecnl_setget_close(struct wamp_dealer *dealer)
{
	ER ret;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf = { (T_ECN_FST_BLK *)dealer->ecnl_mag };

	if (dealer->ecnl_mag == NULL)
		return;

	if(device == NULL){
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_ecnl_setget(dealer->requestId, dealer, device->deviceId, dealer->ecnl_mag);
	if (ret == E_OK) {
		dealer->error = NULL;
	}
	else {
		_ecn_fbs_del(buf);
	}

	dealer->ecnl_mag = NULL;
}

void wamp_dealer_ecnl_setget_res(struct wamp_dealer *dealer, T_EDATA *esv)
{
	wamp_state_t *s = dealer->s;
	echonetlite_device_t *device = dealer->current_device;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0, start;
	uint8_t epc;
	uint8_t pdc;
	uint8_t p_edt[256];
	T_ENUM_EPC enm;
	int j;
	ER ret;

	if(device == NULL)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":%d},[],",
		dealer->requestId, device->deviceId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* プロパティ値書き込み・読み込み応答 0x7E */
	ret = fbs_printf(buf, &pos, "{\"esv\":126,\"sender\":%d,\"seoj\":\"%02x%02x%02x\",\"deoj\":\"%02x%02x%02x\",\"properties\":[",
		ecn_get_enod(esv), esv->hdr.edata.seoj.eojx1, esv->hdr.edata.seoj.eojx2, esv->hdr.edata.seoj.eojx3,
		esv->hdr.edata.deoj.eojx1, esv->hdr.edata.deoj.eojx2, esv->hdr.edata.deoj.eojx3);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = ecn_itr_ini(&enm, esv);
	if (ret != E_OK){
		syslog(LOG_ERROR, "ecn_itr_ini");
		return;
	}

	start = pos;
	for(;;) {
		while ((ret = ecn_itr_nxt(&enm, &epc, &pdc, p_edt)) == E_OK) {
			if (pdc == 0) {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d},", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
			else {
				ret = fbs_printf(buf, &pos, "{\"epc\":%d,\"edt\":\"", epc);
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
				for(j = 0; j < pdc; j++){
					ret = fbs_printf(buf, &pos, "%02x", p_edt[j]);
					if (ret != E_OK) {
						syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
						_ecn_fbs_del(buf);
						return;
					}
				}
				ret = fbs_printf(buf, &pos, "\"},");
				if (ret != E_OK) {
					syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
					_ecn_fbs_del(buf);
					return;
				}
			}
		}
		if(ret != E_BOVR){
			syslog(LOG_ERROR, "ecn_itr_nxt");
			break;
		}
		if(enm.is_eof)
			break;
	}

	/* 最後の","を消す */
	if(start != pos)
		pos--;

	ret = fbs_printf(buf, &pos, "]}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_setget_res() : main_send_message() result = %d", ret);
	}
}

void wamp_dealer_ecnl_setget_timeout(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ER ret;

	ret = wamp_send_error_res(s, WAMP_CODE_CALL, dealer->requestId, internal_error);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_ecnl_setget_timeout() : wamp_send_error_res() result = %d", ret);
	}
}

static void wamp_dealer_pin_mode_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret = E_PAR;

	switch(dealer->arduino_params.mode){
	case WAMP_DEALER_PIN_MODE_INPUT:
		ret = arduino_pinMode(dealer->arduino_params.pin, ARDUINO_PIN_MODE_INPUT);
		break;
	case WAMP_DEALER_PIN_MODE_OUTPUT:
		ret = arduino_pinMode(dealer->arduino_params.pin, ARDUINO_PIN_MODE_OUTPUT);
		break;
	case WAMP_DEALER_PIN_MODE_INPUT_PULLUP:
		ret = arduino_pinMode(dealer->arduino_params.pin, ARDUINO_PIN_MODE_INPUT_PULLUP);
		break;
	}

	if (ret != E_OK)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{}]", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static void wamp_dealer_digital_write_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret = E_PAR;

	switch(dealer->arduino_params.digval){
	case WAMP_DEALER_DIGITAL_VALUE_HIGH:
		ret = arduino_digitalWrite(dealer->arduino_params.pin, ARDUINO_DIGITAL_VALUE_HIGH);
		break;
	case WAMP_DEALER_DIGITAL_VALUE_LOW:
		ret = arduino_digitalWrite(dealer->arduino_params.pin, ARDUINO_DIGITAL_VALUE_LOW);
		break;
	}

	if (ret != E_OK)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{}]", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static void wamp_dealer_digital_read_close(struct wamp_dealer *dealer)
{
	const static char *values[] = { "", "HIGH", "LOW" };
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	arduino_digital_value_t value;
	ER ret;

	ret = arduino_digitalRead(dealer->arduino_params.pin, &value);
	if (ret != E_OK)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "\"pin\":%d,\"value\":\"%s\"",
		dealer->arduino_params.pin, values[value]);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static void wamp_dealer_analog_read_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	int value;
	ER ret;

	ret = arduino_analogRead(dealer->arduino_params.pin, &value);
	if (ret != E_OK)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "\"pin\":%d,\"value\":%d",
		dealer->arduino_params.pin, value);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = fbs_printf(buf, &pos, "}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static void wamp_dealer_analog_write_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;

	ret = arduino_analogWrite(dealer->arduino_params.pin, dealer->arduino_params.anaval);
	if (ret != E_OK)
		return;

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{}]", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static void wamp_dealer_set_init_params_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;
	uint8_t data[32];

	memset(data, 0, sizeof(data));
	memcpy(&data[0], dealer->init_params.mac_addr, sizeof(dealer->init_params.mac_addr));
	data[6] = dealer->init_params.dhcp_enable ? 1 : 0;
	memcpy(&data[7], dealer->init_params.ip_addr, sizeof(dealer->init_params.ip_addr));
	memcpy(&data[11], dealer->init_params.netmask, sizeof(dealer->init_params.netmask));
	memcpy(&data[15], dealer->init_params.gateway, sizeof(dealer->init_params.gateway));

	ret = data_flash_write(0, data);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "data_flash_write() result = %d", ret);
		return;
	}

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{\"deviceId\":1},[],{}]", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

static ER wamp_dealer_write_ip_addr(struct wamp_dealer *dealer, ECN_FBS_ID buf,
	ECN_FBS_SSIZE_T *pos, uint8_t *ip_addr, char *name)
{
	ER ret;
	int i;

	/* IPアドレス */
	ret = fbs_printf(buf, pos, "\"%s\":[", name);
	if (ret != E_OK) {
		return ret;
	}

	for (i = 0; i < 4; i++) {
		ret = fbs_printf(buf, pos, "%d,", ip_addr[i]);
		if (ret != E_OK) {
			return ret;
		}
	}

	/* 最後の","を消す */
	*pos = *pos - 1;

	ret = fbs_printf(buf, pos, "],");
	if (ret != E_OK) {
		return ret;
	}

	return E_OK;
}

static void wamp_dealer_get_init_params_close(struct wamp_dealer *dealer)
{
	wamp_state_t *s = dealer->s;
	ECN_FBS_ID buf;
	ECN_FBS_SSIZE_T pos = 0;
	ER ret;
	int i;
	uint8_t data[32];

	ret = data_flash_read(0, data);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "data_flash_read() result = %d", ret);
		return;
	}

	memcpy(dealer->init_params.mac_addr, &data[0], sizeof(dealer->init_params.mac_addr));
	dealer->init_params.dhcp_enable = data[6] != 0;
	memcpy(dealer->init_params.ip_addr, &data[7], sizeof(dealer->init_params.ip_addr));
	memcpy(dealer->init_params.netmask, &data[11], sizeof(dealer->init_params.netmask));
	memcpy(dealer->init_params.gateway, &data[15], sizeof(dealer->init_params.gateway));

	ret = _ecn_fbs_cre(1, &buf);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "_ecn_fbs_cre() result = %d", ret);
		return;
	}

	ret = fbs_printf(buf, &pos, "[50,%d,{},[],{", dealer->requestId);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* MACアドレス */
	ret = fbs_printf(buf, &pos, "\"macAddr\":[");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	for (i = 0; i < 6; i++) {
		ret = fbs_printf(buf, &pos, "%d,", dealer->init_params.mac_addr[i]);
		if (ret != E_OK) {
			syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
			_ecn_fbs_del(buf);
			return;
		}
	}

	/* 最後の","を消す */
	pos--;

	ret = fbs_printf(buf, &pos, "],");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* DHCP有効／無効 */
	ret = fbs_printf(buf, &pos, "\"dhcpEnable\":%s,", dealer->init_params.dhcp_enable ? "true" : "false");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* IPアドレス */
	ret = wamp_dealer_write_ip_addr(dealer, buf, &pos, dealer->init_params.ip_addr, "ipAddr");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_write_ip_addr() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* サブネットマスク */
	ret = wamp_dealer_write_ip_addr(dealer, buf, &pos, dealer->init_params.netmask, "netmask");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_write_ip_addr() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* デフォルトゲートウェイ */
	ret = wamp_dealer_write_ip_addr(dealer, buf, &pos, dealer->init_params.gateway, "gateway");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "wamp_dealer_write_ip_addr() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	/* 最後の","を消す */
	pos--;

	ret = fbs_printf(buf, &pos, "}]");
	if (ret != E_OK) {
		syslog(LOG_WARNING, "fbs_printf() result = %d", ret);
		_ecn_fbs_del(buf);
		return;
	}

	ret = main_send_message(buf, s->wbsid);
	if (ret != E_OK) {
		syslog(LOG_WARNING, "main_send_message() result = %d", ret);
		return;
	}

	dealer->error = NULL;
}

void wamp_dealer_yield(wamp_dealer_t *dealer)
{
}

void wamp_dealer_yield_request_id(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_yield_options(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_yield_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_yield_arguments(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_yield_arguments_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_yield_argumentskw(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_yield_argumentskw_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_yield_close(struct wamp_state *s)
{
}

void wamp_dealer_cansel(wamp_dealer_t *dealer)
{
}

void wamp_dealer_cansel_request_id(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_cansel_options(wamp_dealer_t *dealer, const char *value)
{
}

void wamp_dealer_cansel_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf)
{
}

void wamp_dealer_cansel_close(struct wamp_state *s)
{
}

static bool_t wamp_dealer_get_epc(const char *propName, int nameLen,
	uint16_t devType, uint8_t *epc, uint8_t *pdc)
{
	if(propName[0] == '0' && propName[1] == 'x'){
		if((wamp_stob(&propName[2], epc, sizeof(*epc)) != 1)
			|| (propName[4] != '\0')){
			*epc = 0;
			*pdc = 0;
			return false;
		}

		*pdc = 1;
		return true;
	}

	*epc = kadecot_names_get_property_code(propName, nameLen, devType);
	if(*epc == 0){
		*pdc = 0;
		return false;
	}
	*pdc = 1;

	return true;
}

static echonetlite_device_t *wamp_dealer_get_device(wamp_dealer_t *dealer, int deviceId)
{
	echonetlite_device_t *pos, *end = &dealer->devices[sizeof(dealer->devices) / sizeof(dealer->devices[0])];

	if(deviceId <= 0)
		return NULL;

	for(pos = dealer->devices; pos < end; pos++){
		if(pos->deviceId != deviceId)
			continue;

		return pos;
	}

	return NULL;
}

int wamp_stob(const char *str, uint8_t *dst, int dstc)
{
	uint8_t *start = dst, *end = &dst[dstc];
	int size = 0, i;
	char c;
	uint8_t data;

	for(i = 0; dst < end; dst++){
		c = str[i];
		if(c == '\0')
			break;

		data = 0;

		if(c >= '0' && c <= '9'){
			data |= (c - '0');
		}
		else if(c >= 'A' && c <= 'F'){
			data |= (0xA + c - 'A');
		}
		else if(c >= 'a' && c <= 'f'){
			data |= (0xA + c - 'a');
		}
		else
			break;

		i++;
		c = str[i];
		data <<= 4;

		if(c >= '0' && c <= '9'){
			data |= (c - '0');
		}
		else if(c >= 'A' && c <= 'F'){
			data |= (0xA + c - 'A');
		}
		else if(c >= 'a' && c <= 'f'){
			data |= (0xA + c - 'a');
		}
		else
			break;

		i++;
		*dst = data;
	}

	return dst - start;
}

void wamp_dealer_init(wamp_dealer_t *dealer)
{
}
