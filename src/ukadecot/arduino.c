/*
 *  TOPPERS ECHONET Lite Communication Middleware
 * 
 *  Copyright (C) 2014 Cores Co., Ltd. Japan
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

/* 
 *  サンプルプログラム(1)の本体
 */

#include <kernel.h>
#include <t_syslog.h>
#include <sil.h>
#include "arduino.h"
#ifdef __RX
#include "rx630_ccrx/rx630.h"
#else
#include "rx630_msvc/rx630.h"
#endif

/* PWM出力(490Hz) */
#define TPU_BASE_COUNTER	(48000000 / 4 / 490)

void arduino_init()
{
	/*
	 * モジュールストップ機能の設定(S12AD)
	 */
	sil_wrh_mem((uint16_t *)SYSTEM_PRCR_ADDR, (uint16_t)0xA502);	/* 書込み許可 */
	sil_wrw_mem((uint32_t *)SYSTEM_MSTPCRA_ADDR,
		sil_rew_mem((uint32_t *)SYSTEM_MSTPCRA_ADDR) & ~SYSTEM_MSTPCRA_MSTPA17_BIT);
	sil_wrh_mem((uint16_t *)SYSTEM_PRCR_ADDR, (uint16_t)0xA500);	/* 書込み禁止 */

	/* 12bitADC初期化 */
	sil_wrh_mem((uint16_t *)S12AD_ADEXICR_ADDR, 0x0000);	/* 温度センサ出力、内部基準電圧非選択 */
	sil_wrh_mem((uint16_t *)S12AD_ADANS0_ADDR, 0x00FF);		/* 変換ポートAN000～AN007選択、AN008～AN015非選択 */
	sil_wrh_mem((uint16_t *)S12AD_ADANS1_ADDR, 0x0000);		/* 変換ポートAN016～AN020非選択 */

	/* PWM出力(490Hz) */
	sil_wrh_mem(SYSTEM_PRCR_ADDR, (uint16_t)0xA502);	/* 書込み許可 */
	sil_wrw_mem(SYSTEM_MSTPCRA_ADDR,
		sil_rew_mem(SYSTEM_MSTPCRA_ADDR) & ~SYSTEM_MSTPCRA_MSTPA13_BIT); /* TPU0～TPU5 */
	sil_wrh_mem(SYSTEM_PRCR_ADDR, (uint16_t)0xA500);	/* 書込み禁止 */

	/* カウンタクロックの選択、カウンタクリア要因の選択 */
	sil_wrb_mem(TPU0_TCR_ADDR, (1 << TPU_TCR_TPSC_OFFSET) | (1 << TPU_TCR_CKEG_OFFSET)
		| (1 << TPU_TCR_CCLR_OFFSET));
	sil_wrb_mem(TPU3_TCR_ADDR, (1 << TPU_TCR_TPSC_OFFSET) | (1 << TPU_TCR_CKEG_OFFSET)
		| (3 << TPU_TCR_CCLR_OFFSET));
	sil_wrb_mem(TPU4_TCR_ADDR, (1 << TPU_TCR_TPSC_OFFSET) | (1 << TPU_TCR_CKEG_OFFSET)
		| (3 << TPU_TCR_CCLR_OFFSET));
	/* 波形出力レベルの選択 */
	sil_wrb_mem(TPU0_TIORL_ADDR, (5 << TPU_TIORL_IOC_OFFSET) | (5 << TPU_TIORL_IOD_OFFSET));
	sil_wrb_mem(TPU3_TIORH_ADDR, (5 << TPU_TIORL_IOA_OFFSET) | (5 << TPU_TIORL_IOB_OFFSET));
	sil_wrb_mem(TPU3_TIORL_ADDR, (5 << TPU_TIORL_IOC_OFFSET) | (5 << TPU_TIORL_IOD_OFFSET));
	sil_wrb_mem(TPU4_TIOR_ADDR, (5 << TPU_TIORL_IOA_OFFSET) | (5 << TPU_TIORL_IOB_OFFSET));
	/* TGRyの設定 */
	sil_wrh_mem(TPU0_TGRA_ADDR, TPU_BASE_COUNTER);
	sil_wrh_mem(TPU0_TGRC_ADDR, 0);
	sil_wrh_mem(TPU0_TGRD_ADDR, 0);
	sil_wrh_mem(TPU3_TGRA_ADDR, 0);
	sil_wrh_mem(TPU3_TGRB_ADDR, 0);
	sil_wrh_mem(TPU3_TGRC_ADDR, 0);
	sil_wrh_mem(TPU3_TGRD_ADDR, 0);
	sil_wrh_mem(TPU4_TGRA_ADDR, 0);
	sil_wrh_mem(TPU4_TGRB_ADDR, 0);
	/* PWMモード2の設定 */
	sil_wrb_mem(TPU0_TMDR_ADDR, 3);
	sil_wrb_mem(TPU3_TMDR_ADDR, 3);
	sil_wrb_mem(TPU4_TMDR_ADDR, 3);
	/* 同期動作設定 */
	sil_wrb_mem(TPUA_TSYR_ADDR, TPU_TSYR_SYNC0_BIT | TPU_TSYR_SYNC3_BIT | TPU_TSYR_SYNC4_BIT);
	/* カウント動作開始 */
	sil_wrb_mem(TPUA_TSTR_ADDR, TPU_TSTR_CST0_BIT | TPU_TSTR_CST3_BIT | TPU_TSTR_CST4_BIT);
}

static bool_t pin_function_table[8] = {
	true, true, true, true, true, true, true, true
};

static ER change_pin_function(int pin, bool_t gpio)
{
	ER result = E_PAR;

	if((pin < 0) || (pin >= 8))
		return E_PAR;

	if(pin_function_table[pin] == gpio)
		return E_OK;

	/* 汎用入出力ポートに設定 */
	switch(pin){
	case 0:
		/* P21 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B1_BIT);
		break;
	case 1:
		/* P20 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B0_BIT);
		break;
	case 2:
		/* P22 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B2_BIT);
		break;
	case 3:
		/* P23 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B3_BIT);
		break;
	case 4:
		/* P24 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B4_BIT);
		break;
	case 5:
		/* P25 */
		sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) & ~PORT_PMR_B5_BIT);
		break;
	case 6:
		/* P32 */
		sil_wrb_mem(PORT3_PMR_ADDR, sil_reb_mem(PORT3_PMR_ADDR) & ~PORT_PMR_B2_BIT);
		break;
	case 7:
		/* P33 */
		sil_wrb_mem(PORT3_PMR_ADDR, sil_reb_mem(PORT3_PMR_ADDR) & ~PORT_PMR_B3_BIT);
		break;
	}

	/* 書き込みプロテクトレジスタの設定 PFSWEビットへの書き込みを許可 */
	sil_wrb_mem((uint8_t *)MPC_PWPR_ADDR, 0x00);
	/* 書き込みプロテクトレジスタの設定 PxxFSレジスタへの書き込みを許可 */
	sil_wrb_mem((uint8_t *)MPC_PWPR_ADDR, 0x40);

	switch(pin){
	/* P21/TIOCA3 */
	case 0:
		if(gpio){
			/* P21端子機能制御レジスタ P21とする */
			sil_wrb_mem((uint8_t *)MPC_P21PFS_ADDR, 0x00);
		}
		else{
			/* P21端子機能制御レジスタ TIOCA3とする */
			sil_wrb_mem((uint8_t *)MPC_P21PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P20/TIOCB3 */
	case 1:
		if(gpio){
			/* P20端子機能制御レジスタ P20とする */
			sil_wrb_mem((uint8_t *)MPC_P20PFS_ADDR, 0x00);
		}
		else{
			/* P20端子機能制御レジスタ TIOCB3とする */
			sil_wrb_mem((uint8_t *)MPC_P20PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P22/TIOCC3 */
	case 2:
		if(gpio){
			/* P22端子機能制御レジスタ P22とする */
			sil_wrb_mem((uint8_t *)MPC_P22PFS_ADDR, 0x00);
		}
		else{
			/* P22端子機能制御レジスタ TIOCC3とする */
			sil_wrb_mem((uint8_t *)MPC_P22PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P23/TIOCD3 */
	case 3:
		if(gpio){
			/* P23端子機能制御レジスタ P23とする */
			sil_wrb_mem((uint8_t *)MPC_P23PFS_ADDR, 0x00);
		}
		else{
			/* P23端子機能制御レジスタ TIOCD3とする */
			sil_wrb_mem((uint8_t *)MPC_P23PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P24/TIOCB4 */
	case 4:
		if(gpio){
			/* P24端子機能制御レジスタ P24とする */
			sil_wrb_mem((uint8_t *)MPC_P24PFS_ADDR, 0x00);
		}
		else{
			/* P24端子機能制御レジスタ TIOCB4とする */
			sil_wrb_mem((uint8_t *)MPC_P24PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P25/TIOCA4 */
	case 5:
		if(gpio){
			/* P25端子機能制御レジスタ P25とする */
			sil_wrb_mem((uint8_t *)MPC_P25PFS_ADDR, 0x00);
		}
		else{
			/* P25端子機能制御レジスタ TIOCA4とする */
			sil_wrb_mem((uint8_t *)MPC_P25PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P32/TIOCC0 */
	case 6:
		if(gpio){
			/* P32端子機能制御レジスタ P32とする */
			sil_wrb_mem((uint8_t *)MPC_P32PFS_ADDR, 0x00);
		}
		else{
			/* P32端子機能制御レジスタ TIOCC0とする */
			sil_wrb_mem((uint8_t *)MPC_P32PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	/* P33/TIOCD0 */
	case 7:
		if(gpio){
			/* P33端子機能制御レジスタ P33とする */
			sil_wrb_mem((uint8_t *)MPC_P33PFS_ADDR, 0x00);
		}
		else{
			/* P33端子機能制御レジスタ TIOCD0とする */
			sil_wrb_mem((uint8_t *)MPC_P33PFS_ADDR, 0x03);
		}
		result =  E_OK;
		break;
	}

	/* 書き込みプロテクトレジスタの設定 書き込みを禁止 */
	sil_wrb_mem((uint8_t *)MPC_PWPR_ADDR, 0x80);

	/* 機能ポートに設定 */
	if(!gpio){
		switch(pin){
		/* P21/TIOCA3 */
		case 0:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B1_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B1_BIT);
			break;
		/* P20/TIOCB3 */
		case 1:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B0_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B0_BIT);
			break;
		/* P22/TIOCC3 */
		case 2:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B2_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B2_BIT);
			break;
		/* P23/TIOCD3 */
		case 3:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B3_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B3_BIT);
			break;
		/* P24/TIOCB4 */
		case 4:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B4_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B4_BIT);
			break;
		/* P25/TIOCA4 */
		case 5:
			sil_wrb_mem(PORT2_PDR_ADDR, sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B5_BIT);
			sil_wrb_mem(PORT2_PMR_ADDR, sil_reb_mem(PORT2_PMR_ADDR) | PORT_PMR_B5_BIT);
			break;
		/* P32/TIOCC0 */
		case 6:
			sil_wrb_mem(PORT3_PDR_ADDR, sil_reb_mem(PORT3_PDR_ADDR) | PORT_PDR_B2_BIT);
			sil_wrb_mem(PORT3_PMR_ADDR, sil_reb_mem(PORT3_PMR_ADDR) | PORT_PMR_B2_BIT);
			break;
		/* P33/TIOCD0 */
		case 7:
			sil_wrb_mem(PORT3_PDR_ADDR, sil_reb_mem(PORT3_PDR_ADDR) | PORT_PDR_B3_BIT);
			sil_wrb_mem(PORT3_PMR_ADDR, sil_reb_mem(PORT3_PMR_ADDR) | PORT_PMR_B3_BIT);
			break;
		}
	}

	pin_function_table[pin] = gpio;

	return result;
}

static ER change_pin_mode(arduino_pin_mode_t mode, volatile uint8_t *pdr, int bit)
{
	volatile uint8_t *pcr = pdr + (PORT0_PCR_ADDR - PORT0_PDR_ADDR);

	switch(mode){
	case ARDUINO_PIN_MODE_INPUT:
		sil_wrb_mem(pdr, sil_reb_mem(pdr) & ~bit);
		sil_wrb_mem(pcr, sil_reb_mem(pcr) & ~bit);
		return E_OK;
	case ARDUINO_PIN_MODE_OUTPUT:
		sil_wrb_mem(pdr, sil_reb_mem(pdr) | bit);
		return E_OK;
	case ARDUINO_PIN_MODE_INPUT_PULLUP:
		sil_wrb_mem(pdr, sil_reb_mem(pdr) & ~bit);
		sil_wrb_mem(pcr, sil_reb_mem(pcr) | bit);
		return E_OK;
	}

	return E_PAR;
}

ER arduino_pinMode(int pin, arduino_pin_mode_t mode)
{
	/* 汎用入出力ポートに設定 */
	change_pin_function(pin, true);

	switch(pin){
	case 0:
		/* P21 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B1_BIT);
	case 1:
		/* P20 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B0_BIT);
	case 2:
		/* P22 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B2_BIT);
	case 3:
		/* P23 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B3_BIT);
	case 4:
		/* P24 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B4_BIT);
	case 5:
		/* P25 */
		return change_pin_mode(mode, PORT2_PDR_ADDR, PORT_PDR_B5_BIT);
	case 6:
		/* P32 */
		return change_pin_mode(mode, PORT3_PDR_ADDR, PORT_PDR_B2_BIT);
	case 7:
		/* P33 */
		return change_pin_mode(mode, PORT3_PDR_ADDR, PORT_PDR_B3_BIT);
	case 8:
		/* PC2 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B2_BIT);
	case 9:
		/* PC3 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B3_BIT);
	case 10:
		/* PC4 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B4_BIT);
	case 11:
		/* PC6 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B6_BIT);
	case 12:
		/* PC7 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B7_BIT);
	case 13:
		/* PC5 */
		return change_pin_mode(mode, PORTC_PDR_ADDR, PORT_PDR_B5_BIT);
	}

	return E_PAR;
}

static ER write_gpio(arduino_digital_value_t value, volatile uint8_t *podr, int bit)
{
	switch(value){
	case ARDUINO_DIGITAL_VALUE_HIGH:
		sil_wrb_mem(podr, sil_reb_mem(podr) | bit);
		return E_OK;
	case ARDUINO_DIGITAL_VALUE_LOW:
		sil_wrb_mem(podr, sil_reb_mem(podr) & ~bit);
		return E_OK;
	}

	return E_PAR;
}

ER arduino_digitalWrite(int pin, arduino_digital_value_t value)
{
	switch(pin){
	case 0:
		/* P21 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B1_BIT);
	case 1:
		/* P20 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B0_BIT);
	case 2:
		/* P22 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B2_BIT);
	case 3:
		/* P23 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B3_BIT);
	case 4:
		/* P24 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B4_BIT);
	case 5:
		/* P25 */
		return write_gpio(value, PORT2_PODR_ADDR, PORT_PODR_B5_BIT);
	case 6:
		/* P32 */
		return write_gpio(value, PORT3_PODR_ADDR, PORT_PODR_B2_BIT);
	case 7:
		/* P33 */
		return write_gpio(value, PORT3_PODR_ADDR, PORT_PODR_B3_BIT);
	case 8:
		/* PC2 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B2_BIT);
	case 9:
		/* PC3 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B3_BIT);
	case 10:
		/* PC4 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B4_BIT);
	case 11:
		/* PC6 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B6_BIT);
	case 12:
		/* PC7 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B7_BIT);
	case 13:
		/* PC5 */
		return write_gpio(value, PORTC_PODR_ADDR, PORT_PODR_B5_BIT);
	}

	return E_PAR;
}

static ER read_gpio(volatile uint8_t *pidr, int bit, arduino_digital_value_t *result)
{
	if ((sil_reb_mem(pidr) & bit) != 0) {
		*result = ARDUINO_DIGITAL_VALUE_HIGH;
		return E_OK;
	}
	else {
		*result = ARDUINO_DIGITAL_VALUE_LOW;
		return E_OK;
	}
}

ER arduino_digitalRead(int pin, arduino_digital_value_t *result)
{
	switch(pin){
	case 0:
		/* P21 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B1_BIT, result);
	case 1:
		/* P20 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B0_BIT, result);
	case 2:
		/* P22 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B2_BIT, result);
	case 3:
		/* P23 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B3_BIT, result);
	case 4:
		/* P24 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B4_BIT, result);
	case 5:
		/* P25 */
		return read_gpio(PORT2_PIDR_ADDR, PORT_PIDR_B5_BIT, result);
	case 6:
		/* P32 */
		return read_gpio(PORT3_PIDR_ADDR, PORT_PIDR_B2_BIT, result);
	case 7:
		/* P33 */
		return read_gpio(PORT3_PIDR_ADDR, PORT_PIDR_B3_BIT, result);
	case 8:
		/* PC2 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B2_BIT, result);
	case 9:
		/* PC3 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B3_BIT, result);
	case 10:
		/* PC4 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B4_BIT, result);
	case 11:
		/* PC6 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B6_BIT, result);
	case 12:
		/* PC7 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B7_BIT, result);
	case 13:
		/* PC5 */
		return read_gpio(PORTC_PIDR_ADDR, PORT_PIDR_B5_BIT, result);
	}

	return E_PAR;
}

#define arduino_ad_table_count 64
static uint16_t arduino_ad_table[8][arduino_ad_table_count];
static int arduino_ad_pos = 0;
static uint32_t arduino_ad_avelage[8];

ER arduino_analogRead(int pin, int *value)
{
	switch(pin){
	case 14:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR0_ADDR); */
		*value = 1023 - (arduino_ad_avelage[0] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 15:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR1_ADDR); */
		*value = 1023 - (arduino_ad_avelage[1] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 16:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR2_ADDR); */
		*value = 1023 - (arduino_ad_avelage[2] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 17:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR3_ADDR); */
		*value = 1023 - (arduino_ad_avelage[3] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 18:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR4_ADDR); */
		*value = 1023 - (arduino_ad_avelage[4] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 19:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR5_ADDR); */
		*value = 1023 - (arduino_ad_avelage[5] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 20:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR6_ADDR); */
		*value = 1023 - (arduino_ad_avelage[6] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	case 21:
		/* *value = sil_reh_mem((uint16_t *)S12AD_ADDR7_ADDR); */
		*value = 1023 - (arduino_ad_avelage[7] / (arduino_ad_table_count * 4/*12bit→10bit*/));
		return E_OK;
	}

	return E_PAR;
}

ER arduino_analogWrite(int pin, int value)
{
	change_pin_function(pin, false);

	value = (value * TPU_BASE_COUNTER) / 255;

	switch(pin){
	case 0:
		/* P21/TIOCA3 */
		sil_wrh_mem(TPU3_TGRA_ADDR, value);
		return E_OK;
	case 1:
		/* P20/TIOCB3 */
		sil_wrh_mem(TPU3_TGRB_ADDR, value);
		return E_OK;
	case 2:
		/* P22/TIOCC3 */
		sil_wrh_mem(TPU3_TGRC_ADDR, value);
		return E_OK;
	case 3:
		/* P23/TIOCD3 */
		sil_wrh_mem(TPU3_TGRD_ADDR, value);
		return E_OK;
	case 4:
		/* P24/TIOCB4 */
		sil_wrh_mem(TPU4_TGRB_ADDR, value);
		return E_OK;
	case 5:
		/* P25/TIOCA4 */
		sil_wrh_mem(TPU4_TGRA_ADDR, value);
		return E_OK;
	case 6:
		/* P32/TIOCC0 */
		sil_wrh_mem(TPU0_TGRC_ADDR, value);
		return E_OK;
	case 7:
		/* P33/TIOCD0 */
		sil_wrh_mem(TPU0_TGRD_ADDR, value);
		return E_OK;
	}

	return E_PAR;
}

void arduino_tick()
{
	static volatile uint16_t *const regs[8] = {
		S12AD_ADDR0_ADDR,
		S12AD_ADDR1_ADDR,
		S12AD_ADDR2_ADDR,
		S12AD_ADDR3_ADDR,
		S12AD_ADDR4_ADDR,
		S12AD_ADDR5_ADDR,
		S12AD_ADDR6_ADDR,
		S12AD_ADDR7_ADDR
	};
	uint16_t ad_value;
	int i;

	/* ADCの変換結果取得 */
	if((sil_reb_mem((uint8_t *)S12AD_ADCSR_ADDR) & S12AD_ADCSR_ADST_BIT) == 0){
		for (i = 0; i < 8; i++){
			arduino_ad_avelage[i] -= arduino_ad_table[i][arduino_ad_pos];
			ad_value = sil_reh_mem(regs[i]);
			arduino_ad_table[i][arduino_ad_pos] = ad_value;
			arduino_ad_avelage[i] += ad_value;
		}

		arduino_ad_pos++;
		if(arduino_ad_pos >= arduino_ad_table_count){
			arduino_ad_pos = 0;
		}

		/* 変換開始（シングルスキャンモード） */
		sil_wrb_mem((uint8_t *)S12AD_ADCSR_ADDR, S12AD_ADCSR_ADST_BIT);
	}
}
