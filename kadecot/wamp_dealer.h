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

#ifndef WAMP_DEALER_H
#define WAMP_DEALER_H

typedef enum wamp_dealer_state
{
	WAMP_DEALER_STATE_DEVICE_ID,
	WAMP_DEALER_STATE_DEOJID,
	WAMP_DEALER_STATE_PROPERTIES,
	WAMP_DEALER_STATE_PROPERTY_LIST,
	WAMP_DEALER_STATE_PROPERTY_NAME,
	WAMP_DEALER_STATE_PROPERTY_CODE,
	WAMP_DEALER_STATE_PROPERTY_VALUE,
	WAMP_DEALER_STATE_PIN,
	WAMP_DEALER_STATE_MODE,
	WAMP_DEALER_STATE_DIGITAL_VALUE,
	WAMP_DEALER_STATE_ANALOG_VALUE,
	WAMP_DEALER_STATE_MAC_ADDR_VALUE,
	WAMP_DEALER_STATE_DHCP_ENABLE_VALUE,
	WAMP_DEALER_STATE_IP_ADDR_VALUE,
	WAMP_DEALER_STATE_NETMASK_VALUE,
	WAMP_DEALER_STATE_GATEWAY_VALUE,
} wamp_dealer_state_t;

typedef enum wamp_dealer_procedure
{
	WAMP_DEALER_PROCEDURE_NONE,
	WAMP_DEALER_PROCEDURE_GET_DEVICELIST,
	WAMP_DEALER_PROCEDURE_KADECOT_GET,
	WAMP_DEALER_PROCEDURE_KADECOT_SET,
	WAMP_DEALER_PROCEDURE_GET_DEVICEINFO,
	WAMP_DEALER_PROCEDURE_ECNL_SET,
	WAMP_DEALER_PROCEDURE_ECNL_GET,
	WAMP_DEALER_PROCEDURE_ECNL_SETGET,
	WAMP_DEALER_PROCEDURE_PIN_MODE,
	WAMP_DEALER_PROCEDURE_DIGITAL_WRITE,
	WAMP_DEALER_PROCEDURE_DIGITAL_READ,
	WAMP_DEALER_PROCEDURE_ANALOG_READ,
	WAMP_DEALER_PROCEDURE_ANALOG_WRITE,
	WAMP_DEALER_PROCEDURE_SET_INIT_PARAMS,
	WAMP_DEALER_PROCEDURE_GET_INIT_PARAMS,
} wamp_dealer_procedure_t;

typedef enum wamp_dealer_pin_mode
{
	WAMP_DEALER_PIN_MODE_NONE,
	WAMP_DEALER_PIN_MODE_INPUT,
	WAMP_DEALER_PIN_MODE_OUTPUT,
	WAMP_DEALER_PIN_MODE_INPUT_PULLUP
} wamp_dealer_pin_mode_t;

typedef enum wamp_dealer_digital_value
{
	WAMP_DEALER_DIGITAL_VALUE_NONE,
	WAMP_DEALER_DIGITAL_VALUE_HIGH,
	WAMP_DEALER_DIGITAL_VALUE_LOW,
} wamp_dealer_digital_value_t;

typedef void (*wamp_dealer_args_parse_t)(struct wamp_dealer *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
typedef void (*wamp_dealer_call_close_t)(struct wamp_dealer *dealer);

typedef struct wamp_dealer_params{
	uint8_t epc;
	uint8_t pdc;
	uint8_t edt[256];
	uint32_t deojid;
	char prop_name[80];
} wamp_dealer_params_t;

typedef struct wamp_dealer_arduino_params{
	int pin;
	wamp_dealer_pin_mode_t mode;
	wamp_dealer_digital_value_t digval;
	int anaval;
} wamp_dealer_arduino_params_t;

typedef struct wamp_dealer_init_params{
	int pos;
	uint8_t mac_addr[6];
	bool_t dhcp_enable;
	uint8_t ip_addr[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
} wamp_dealer_init_params_t;

typedef struct echonetlite_device
{
	int deviceId;
	int nodeId;
	int addrId;
	uint16_t deviceType;
	uint8_t x3;
	char deviceTypeStr[32];
	char description[32];
	char ip_addr[16];
	char nickname[32]; 
	bool_t status;
} echonetlite_device_t;

typedef struct wamp_dealer
{
	struct wamp_state *s;
	wamp_dealer_state_t state;
	wamp_dealer_procedure_t procedure;
	unsigned int requestId;
	union{
		wamp_dealer_params_t params;
		wamp_dealer_arduino_params_t arduino_params;
		wamp_dealer_init_params_t init_params;
	};
	T_EDATA *ecnl_mag;
	echonetlite_device_t devices[20];
	int devices_pos;
	echonetlite_device_t *current_device;
	wamp_dealer_args_parse_t args_parse;
	wamp_dealer_call_close_t call_close;
	const char *error;
} wamp_dealer_t;

void wamp_dealer_init(wamp_dealer_t *dealer);

void wamp_dealer_register(wamp_dealer_t *dealer);
void wamp_dealer_register_request_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_register_options(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_register_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_register_procedure(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_register_close(struct wamp_state *s);

void wamp_dealer_unregister(wamp_dealer_t *dealer);
void wamp_dealer_unregister_request_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_unregister_registration_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_unregister_close(struct wamp_state *s);

void wamp_dealer_call(wamp_dealer_t *dealer);
void wamp_dealer_call_request_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_call_options(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_call_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_call_procedure(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_call_arguments(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_call_arguments_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_call_argumentskw(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_call_argumentskw_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_call_close(struct wamp_state *s);

void wamp_dealer_yield(wamp_dealer_t *dealer);
void wamp_dealer_yield_request_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_yield_options(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_yield_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_yield_arguments(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_yield_arguments_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_yield_argumentskw(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_yield_argumentskw_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_yield_close(struct wamp_state *s);

void wamp_dealer_cansel(wamp_dealer_t *dealer);
void wamp_dealer_cansel_request_id(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_cansel_options(wamp_dealer_t *dealer, const char *value);
void wamp_dealer_cansel_options_param(wamp_dealer_t *dealer, jsonsl_action_t action,
	struct jsonsl_state_st *state, const char *buf);
void wamp_dealer_cansel_close(struct wamp_state *s);

void wamp_dealer_set_devicelist(struct wamp_dealer *dealer, ECN_FBS_ID msg);
void wamp_dealer_get_devicelist_timeout(struct wamp_dealer *dealer);
void wamp_dealer_set_ipaddr(struct wamp_dealer *dealer, char *ipaddr);
void wamp_dealer_get_ipaddr_timeout(struct wamp_dealer *dealer);
void wamp_dealer_set_deviceinfo(struct wamp_dealer *dealer, int deviceId,
	uint8_t *pmapSet, uint8_t *pmapGet, uint8_t *pmapAnno);
void wamp_dealer_get_deviceinfo_timeout(struct wamp_dealer *dealer);
void wamp_dealer_kadecot_set(struct wamp_dealer *dealer, int deviceId, uint8_t epc);
void wamp_dealer_kadecot_set_timeout(struct wamp_dealer *dealer);
void wamp_dealer_kadecot_get(struct wamp_dealer *dealer, int deviceId, uint8_t epc,
	uint8_t pdc, uint8_t *edt);
void wamp_dealer_kadecot_get_timeout(struct wamp_dealer *dealer);
void wamp_dealer_ecnl_set_res(struct wamp_dealer *dealer, T_EDATA *res);
void wamp_dealer_ecnl_set_timeout(struct wamp_dealer *dealer);
void wamp_dealer_ecnl_get_res(struct wamp_dealer *dealer, T_EDATA *res);
void wamp_dealer_ecnl_get_timeout(struct wamp_dealer *dealer);
void wamp_dealer_ecnl_setget_res(struct wamp_dealer *dealer, T_EDATA *res);
void wamp_dealer_ecnl_setget_timeout(struct wamp_dealer *dealer);

#endif // WAMP_DEALER_H
