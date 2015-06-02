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
#ifndef _MSC_VER
#include <t_stddef.h>
#else
#include <stdint.h>
typedef int bool_t;
#define false ((bool_t)0)
#define true ((bool_t)1)
#endif
#include "kadecot_names.h"

uint16_t kadecot_names_get_device_type(const char *name, int nameLen);
bool_t kadecot_names_get_device_type_name(uint16_t deviceType, char *buf, int bufLen);
uint8_t kadecot_names_get_property_code(const char *name, int nameLen,
	uint16_t devType);
static uint16_t get_device_type(uint8_t *table, int len,
	const char *name, int nameLen);
static bool_t get_device_type_name(uint8_t *table, int len, uint16_t deviceType,
	uint8_t *strtable, int stablelen, char *buf, int bufLen);
static uint8_t get_property_code(uint8_t *table, int len,
	const char *name, int nameLen, uint16_t devType);
static uint8_t get_prop_code(uint8_t *table, int len, const char *name, int nameLen,
	uint16_t devType, int pos);

#ifndef _MSC_VER
#define KADECOT_NAMES_TABLE ((uint8_t *)0xFFFE0000)
#else
#undef SIZE
#include <windows.h>
#include <tchar.h>
#include <winioctl.h>

uint8_t KADECOT_NAMES_TABLE[0x5000];
#endif
static int devTableLen;
static uint8_t *devTable;
static int devCodeTableLen;
static uint8_t *devCodeTable;
static int propTableLen;
static uint8_t *propTable;

void kadecot_names_init()
{
	int pos;
#ifdef _MSC_VER
	HANDLE h;
	DWORD br;

	h = CreateFile(_T("..\\..\\..\\..\\kadecot\\kadecot_names.bin"), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (h == INVALID_HANDLE_VALUE) {
		return;
	}

	ReadFile(h, KADECOT_NAMES_TABLE, sizeof(KADECOT_NAMES_TABLE), &br, 0);
	CloseHandle(h);
#endif
	devTableLen = (KADECOT_NAMES_TABLE[0] << 8) | KADECOT_NAMES_TABLE[1];
	devTable = &KADECOT_NAMES_TABLE[2];
	pos = devTableLen + 2;
	devCodeTableLen = (KADECOT_NAMES_TABLE[pos] << 8) | KADECOT_NAMES_TABLE[pos + 1];
	devCodeTable = &KADECOT_NAMES_TABLE[pos + 2];
	pos += devCodeTableLen + 2;
	propTableLen = (KADECOT_NAMES_TABLE[pos] << 8) | KADECOT_NAMES_TABLE[pos + 1];
	propTable = &KADECOT_NAMES_TABLE[pos + 2];
}

uint16_t kadecot_names_get_device_type(const char *name, int nameLen)
{
	return get_device_type(devTable, devTableLen, name, nameLen);
}

bool_t kadecot_names_get_device_type_name(uint16_t deviceType, char *buf, int bufLen)
{
	return get_device_type_name(devCodeTable, devCodeTableLen, deviceType,
		devTable, devTableLen, buf, bufLen);
}

uint8_t kadecot_names_get_property_code(const char *name, int nameLen,
	uint16_t devType)
{
	return get_property_code(propTable, propTableLen, name, nameLen, devType);
}

static uint16_t get_device_type(uint8_t *table, int len, const char *name, int nameLen)
{
	int pos = 0, nextpos, i = 0;
	uint8_t count;
	uint16_t DeviceType;

	while (pos < len) {
		/* branch count */
		count = table[pos++];

		/* 末端のノード */
		if (count == 0) {
			/* NULL文字も含めたサイズ */
			if(i != (nameLen + 1))
				return 0;

			DeviceType = table[pos++] << 8;
			DeviceType |= table[pos++];

			return DeviceType;
		}

		for (;;) {
			int length, start = pos, j, k;
			/* string length */
			length = table[pos++];
			for (j = 0, k = i; j < length; j++, k++) {
				/* NULL文字も含めたチェック */
				if((k > nameLen) || (table[pos++] != name[k])){
					pos = start + 1 + length + 2;
					goto nextnode;
				}
			}
			i += length;

			/* nextpos */
			nextpos = table[pos++] << 8;
			nextpos |= table[pos++];

			pos = nextpos;
			break;
		nextnode:
			count--;
			if (count == 0)
				return 0;
			continue;
		}
	}

	return 0;
}

static bool_t get_device_type_name(uint8_t *table, int len, uint16_t deviceType,
	uint8_t *strtable, int stablelen, char *buf, int bufLen)
{
	int pos = 0, i, unmatchpos;

	while (pos < len) {
		uint16_t mask, ptrn;
		/* bit mask */
		mask = table[pos++] << 8;
		mask |= table[pos++];
		/* bit pattarn */
		ptrn = table[pos++] << 8;
		ptrn |= table[pos++];
		/* unmatchpos */
		unmatchpos = table[pos++] << 8;
		unmatchpos |= table[pos++];

		if ((mask & deviceType) != ptrn) {
			if (unmatchpos == 0)
				return false;
			pos = unmatchpos;
			continue;
		}

		/* 末端のノード */
		if ((mask & 0x0001) != 0) {
			int bufpos = 0;
			int count = table[pos++];
			for (i = 0; i < count; i++) {
				int substr, strlen, end;

				substr = table[pos++] << 8;
				substr |= table[pos++];

				if (substr > stablelen)
					return false;

				strlen = strtable[substr++];
				if ((substr + strlen) > stablelen)
					return false;

				end = bufpos + strlen;
				if (end > bufLen)
					return false;

				for (; bufpos < end; bufpos++) {
					buf[bufpos] = (char)strtable[substr++];
				}
			}

			return true;
		}
	}

	return false;
}

static uint8_t get_property_code(uint8_t *table, int len, const char *name,
	int nameLen, uint16_t devType)
{
	int pos = 0, nextpos, i = 0;
	uint8_t count;
	uint8_t epc;

	while (pos < len) {
		/* branch count */
		count = table[pos++];

		/* 末端のノード */
		if (count == 0) {
			/* NULL文字も含めたサイズ */
			if(i != (nameLen + 1))
				return 0;

			epc = get_prop_code(table, len, name, nameLen, devType, pos);
			return epc;
		}

		for (;;) {
			int length, start = pos, j, k;
			/* string length */
			length = table[pos++];
			for (j = 0, k = i; j < length; j++, k++) {
				/* NULL文字も含めたチェック */
				if((k > nameLen) || (table[pos++] != name[k])){
					pos = start + 1 + length + 2;
					goto nextnode;
				}
			}
			i += length;

			/* nextpos */
			nextpos = table[pos++] << 8;
			nextpos |= table[pos++];

			pos = nextpos;
			break;
		nextnode:
			count--;
			if (count == 0)
				return 0;
			continue;
		}
	}

	return 0;
}

static uint8_t get_prop_code(uint8_t *table, int len, const char *name,
	int nameLen, uint16_t devType, int pos)
{
	int i, start;
	uint8_t count;
	uint8_t epc;
	uint16_t type;

	while (pos < len) {
		/* branch count */
		count = table[pos++];

		/* 末端のノード */
		if (count == 0) {
			epc = table[pos++];
			return epc;
		}

		start = pos;
		for (i = 0; i < count; i++) {
			type = table[pos++] << 8;
			type |= table[pos++];
			if (type == devType) {
				epc = table[start + 2 * count];
				return epc;
			}
		}
		pos++;
	}

	epc = table[pos];
	return epc;
}
