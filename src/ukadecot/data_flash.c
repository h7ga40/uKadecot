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
#include "data_flash.h"
#ifdef __RX
#include "rx630_ccrx/rx630.h"
#else
#include "rx630_msvc/rx630.h"
#endif
#include "grsakura.h"

#define FLASH_FWEPROR_ADDR				( ( volatile uint8_t __evenaccess * )0x0008C296 )

#define FLASH_FMODR_ADDR				( ( volatile uint8_t __evenaccess * )0x007FC402 )
#define FLASH_FMODR_FRDMD_BIT			( 0x01U << 4U )

#define FLASH_DFLRE0_ADDR				( ( volatile uint16_t __evenaccess * )0x007FC440 )
#define FLASH_DFLRE1_ADDR				( ( volatile uint16_t __evenaccess * )0x007FC442 )
#define FLASH_DFLWE0_ADDR				( ( volatile uint16_t __evenaccess * )0x007FC450 )
#define FLASH_DFLWE1_ADDR				( ( volatile uint16_t __evenaccess * )0x007FC452 )
#define FLASH_FCURAME_ADDR				( ( volatile uint16_t __evenaccess * )0x007FC454 )

#define FLASH_FASTAT_ADDR				( ( volatile uint8_t __evenaccess * )0x007FC410 )
#define FLASH_FASTAT_DFLWPE_BIT			( 0x01U << 0 )
#define FLASH_FASTAT_DFLRPE_BIT			( 0x01U << 1 )
#define FLASH_FASTAT_DFLAE_BIT			( 0x01U << 3 )
#define FLASH_FASTAT_CMDLK_BIT			( 0x01U << 4 )
#define FLASH_FASTAT_ROMAE_BIT			( 0x01U << 7 )

#define FLASH_FENTRYR_ADDR				( ( volatile uint16_t __evenaccess * )0x007FFFB2 )
#define FLASH_FENTRYR_FENRTY0_BIT		( 0x0001U << 0U )
#define FLASH_FENTRYR_FENRTY1_BIT		( 0x0001U << 1U )
#define FLASH_FENTRYR_FENRTY2_BIT		( 0x0001U << 2U )
#define FLASH_FENTRYR_FENRTY3_BIT		( 0x0001U << 3U )
#define FLASH_FENTRYR_FENTRYD_BIT		( 0x0001U << 7U )

#define FLASH_FRESETR_ADDR				( ( volatile uint16_t __evenaccess * )0x007FFFB6 )
#define FLASH_FRESETR_FRESET_BIT		( 0x0001U << 0 )

#define FLASH_DFLBCCNT_ADDR				( ( volatile uint16_t __evenaccess * )0x007FFFCA )
#define FLASH_DFLBCCNT_BCSIZE_BIT		( 0x0001U << 15U )
#define FLASH_DFLBCCNT_BCADR_OFFSET		( 0U )
#define FLASH_DFLBCCNT_BCADR_MASK		( 0x7FFU << FLASH_DFLBCCNT_BCADR_OFFSET )

#define FLASH_DFLBCSTAT_ADDR			( ( volatile uint16_t __evenaccess * )0x007FFFCE )
#define FLASH_DFLBCSTAT_BCST_BIT		( 0x0001U << 0U )

#define FLASH_FSTATR0_ADDR				( ( volatile uint8_t __evenaccess * )0x007FFFB0 )
#define FLASH_FSTATR0_PRGSPD_BIT		( 0x01U << 0 )
#define FLASH_FSTATR0_ERSSPD_BIT		( 0x01U << 1 )
#define FLASH_FSTATR0_SUSRDY_BIT		( 0x01U << 3 )
#define FLASH_FSTATR0_PRGERR_BIT		( 0x01U << 4 )
#define FLASH_FSTATR0_ERSERR_BIT		( 0x01U << 5 )
#define FLASH_FSTATR0_ILGLERR_BIT		( 0x01U << 6 )
#define FLASH_FSTATR0_FRDY_BIT			( 0x01U << 7 )

#define FLASH_PCKAR_ADDR				( ( volatile uint16_t __evenaccess * )0x007FFFE8 )

#define DATA_FLASH_BLOCK_SIZE 32
#ifndef _MSC_VER
#define DATA_FLASH_BASE_ADDR 0x00100000
#define FCU_FIRMWARE_ADDR 0xFEFFE000
#define FCU_RAM_ADDR 0x007F8000
#else
uint8_t DATA_FLASH_BASE_ADDR[DATA_FLASH_BLOCK_SIZE * 1024];
uint8_t FCU_FIRMWARE_ADDR[0x2000];
uint8_t FCU_RAM_ADDR[0x2000];
#endif

static void fcu_init();
static void fcu_copy_firm();
static bool_t fcu_notify_clock();
static void fcu_read_mode();
static void fcu_pe_mode();
static bool_t fcu_write(int blockno, void *data);
static bool_t fcu_erase(int blockno);
static bool_t fcu_check_blank(int blockno);
static bool_t fcu_check_valid(int blockno);
static bool_t fcu_check_frdy(int tWAIT);
static bool_t fcu_check_error();

ER data_flash_init()
{
	ER ret;

	/* E2データフラッシュの読み込み許可 */
	sil_wrh_mem(FLASH_DFLRE0_ADDR, 0x2DFF);
	sil_wrh_mem(FLASH_DFLRE1_ADDR, 0xD2FF);
	/* E2データフラッシュの書き込み許可 */
	sil_wrh_mem(FLASH_DFLWE0_ADDR, 0x1EFF);
	sil_wrh_mem(FLASH_DFLWE1_ADDR, 0xE1FF);

	/* tE16K = 240ms*/
	fcu_check_frdy(240000);

	/* エラー確認 */
	fcu_check_error();

	/* FCUファームウェアのコピー */
	fcu_copy_firm();

	/* P/Eモード移行 */
	fcu_pe_mode();

	/* P/E ノーマルモード移行 */
	*((uint8_t *)DATA_FLASH_BASE_ADDR) = 0xFF;

	/* エラー確認 */
	fcu_check_error();

	/* 周辺クロック通知コマンド発行 */
	ret = fcu_notify_clock();

	/* リードモード移行 */
	fcu_read_mode();

	return ret ? E_OK : E_SYS;
}

ER data_flash_read(int blockno, void *data)
{
	uint16_t *wa, *end;
	uint16_t *dst = (uint16_t *)data;

	/* P/Eモード移行 */
	fcu_pe_mode();

	/* 有効データチェック */
	if(!fcu_check_valid(blockno)){
		/* リードモード移行 */
		fcu_read_mode();
		return E_OBJ;
	}

	/* リードモード移行 */
	fcu_read_mode();

	wa = (uint16_t *)(DATA_FLASH_BLOCK_SIZE * blockno + (intptr_t)DATA_FLASH_BASE_ADDR);
	end = &wa[DATA_FLASH_BLOCK_SIZE/sizeof(uint16_t)];

	/* 読み出し */
	for(; wa < end; wa++, dst++){
		*dst = *wa;
	}

	return E_OK;
}

ER data_flash_write(int blockno, void *data)
{
	bool_t ret = true;

	/* P/Eモード移行 */
	fcu_pe_mode();

	/* ブランクチェック */
	if(!fcu_check_blank(blockno)){
		/* ブランクでないならブロック消去 */
		ret = fcu_erase(blockno);
		if(!ret)
			syslog(LOG_DEBUG, "fcu_erase() result = %d", ret);
	}

	if(ret){
		/* 書き込み */
		ret = fcu_write(blockno, data);
		if(!ret)
			syslog(LOG_DEBUG, "fcu_write() result = %d", ret);
	}

	/* リードモード移行 */
	fcu_read_mode();

	return ret ? E_OK : E_SYS;
}

static void fcu_copy_firm()
{
	unsigned int *src, *dst, *end;

	/* リードモード移行 */
	if(sil_reh_mem(FLASH_FENTRYR_ADDR) != 0)
		sil_wrh_mem(FLASH_FENTRYR_ADDR, 0xAA00);

	/* FCU RAMアクセス許可 */
	sil_wrh_mem(FLASH_FCURAME_ADDR, 0xC401);

	/* FCUファームウェアをFCU RAMにコピー */
	src = (unsigned int *)FCU_FIRMWARE_ADDR;
	dst = (unsigned int *)FCU_RAM_ADDR;
	end = &dst[0x2000 / sizeof(unsigned int)];
	for(; dst < end; dst++, src++)
		*dst = *src;
}

static bool_t fcu_notify_clock()
{
	volatile uint8_t *ra;
	volatile uint16_t *wa;

	ra = (volatile uint8_t *)(DATA_FLASH_BASE_ADDR);
	wa = (volatile uint16_t *)ra;

	/* 周波数を設定 */
	sil_wrh_mem(FLASH_PCKAR_ADDR, 48/*FREQ_PCLK / 1000*/);

	/* 周辺クロック通知コマンド発行 */
	*ra = 0xE9;
	*ra = 0x03;
	*wa = 0x0F0F;
	*wa = 0x0F0F;
	*wa = 0x0F0F;
	*ra = 0xD0;

	/* 120μs */
	fcu_check_frdy(120);

	/* エラー確認 */
	return !fcu_check_error();
}

/*
 * リードモード移行
 */
static void fcu_read_mode()
{
	int i;

	for(;;){
		for(i = 0; i < 1000; i++){
			sil_wrh_mem(FLASH_FENTRYR_ADDR, 0xAA00);

			if(sil_reh_mem(FLASH_FENTRYR_ADDR) != 0)
				continue;

			sil_wrb_mem(FLASH_FWEPROR_ADDR, 0x02);
			return;
		}

		syslog(LOG_WARNING, "fcu_read_mode");
	}
}

/*
 * P/Eモード移行
 */
static void fcu_pe_mode()
{
	sil_wrh_mem(FLASH_FENTRYR_ADDR, 0xAA80);
	sil_wrb_mem(FLASH_FWEPROR_ADDR, 0x01);
}

static bool_t fcu_write(int blockno, void *data)
{
	volatile uint8_t *ra;
	volatile uint16_t *wa, *end;
	uint16_t *src = (uint16_t *)data;

	ra = (volatile uint8_t *)(DATA_FLASH_BLOCK_SIZE * blockno + (intptr_t)DATA_FLASH_BASE_ADDR);
	end = (volatile uint16_t *)(((intptr_t)ra) + DATA_FLASH_BLOCK_SIZE);

	for(wa = (volatile uint16_t *)ra; wa < end; wa++, src++)
	{
		/* プログラムコマンド発行 */
		*ra = 0xE8;
		*ra = 0x01;
		*wa = *src;
		*ra = 0xD0;

		/* 2ms×1.1 */
		fcu_check_frdy(2200);

		/* エラー確認 */
		if(fcu_check_error())
			return false;
	}

	return true;
}

static bool_t fcu_erase(int blockno)
{
	volatile uint8_t *ra;

	ra = (volatile uint8_t *)(DATA_FLASH_BLOCK_SIZE * blockno + (intptr_t)DATA_FLASH_BASE_ADDR);

	/* イレースコマンド発行 */
	*ra = 0x20;
	*ra = 0xD0;

	/* 20ms×1.1 */
	fcu_check_frdy(22000);

	/* エラー確認 */
	return !fcu_check_error();
}

static bool_t fcu_check_blank(int blockno)
{
	volatile uint8_t *ra;
	volatile uint16_t *wa, *end;

	sil_wrb_mem(FLASH_FMODR_ADDR, sil_reb_mem(FLASH_FMODR_ADDR) | FLASH_FMODR_FRDMD_BIT);

	wa = (volatile uint16_t *)(DATA_FLASH_BLOCK_SIZE * blockno + (intptr_t)DATA_FLASH_BASE_ADDR);
	end = (volatile uint16_t *)(((intptr_t)wa) + DATA_FLASH_BLOCK_SIZE);

	for(; wa < end; wa++)
	{
		sil_wrh_mem(FLASH_DFLBCCNT_ADDR, (((intptr_t)wa) & FLASH_DFLBCCNT_BCADR_MASK) << FLASH_DFLBCCNT_BCADR_OFFSET);

		ra = (volatile uint8_t *)wa;
		*ra = 0x71;
		*ra = 0xD0;

		/* 30μs×1.1 */
		fcu_check_frdy(33);

		/* エラー確認 */
		if(fcu_check_error())
			return false;

		/* ブランク確認 */
		if((sil_reh_mem(FLASH_DFLBCSTAT_ADDR) & FLASH_DFLBCSTAT_BCST_BIT) != 0)
			return false;
	}

	return true;
}

static bool_t fcu_check_valid(int blockno)
{
	volatile uint8_t *ra;
	volatile uint16_t *wa, *end;

	sil_wrb_mem(FLASH_FMODR_ADDR, sil_reb_mem(FLASH_FMODR_ADDR) | FLASH_FMODR_FRDMD_BIT);

	wa = (volatile uint16_t *)(DATA_FLASH_BLOCK_SIZE * blockno + (intptr_t)DATA_FLASH_BASE_ADDR);
	end = (volatile uint16_t *)(((intptr_t)wa) + DATA_FLASH_BLOCK_SIZE);

	for(; wa < end; wa++)
	{
		sil_wrh_mem(FLASH_DFLBCCNT_ADDR, (((intptr_t)wa) & FLASH_DFLBCCNT_BCADR_MASK) << FLASH_DFLBCCNT_BCADR_OFFSET);

		ra = (volatile uint8_t *)wa;
		*ra = 0x71;
		*ra = 0xD0;

		/* 30μs×1.1 */
		fcu_check_frdy(33);

		/* エラー確認 */
		if(fcu_check_error())
			return false;

		/* ブランク確認 */
		if((sil_reh_mem(FLASH_DFLBCSTAT_ADDR) & FLASH_DFLBCSTAT_BCST_BIT) == 0)
			return false;
	}

	return true;
}

static bool_t fcu_check_error()
{
	uint8_t status;

	status = sil_reb_mem(FLASH_FSTATR0_ADDR);

	if((status & (FLASH_FSTATR0_ILGLERR_BIT | FLASH_FSTATR0_ERSERR_BIT | FLASH_FSTATR0_PRGERR_BIT)) == 0)
		return false;

	if((status & FLASH_FSTATR0_ILGLERR_BIT) != 0){
		if(sil_reb_mem(FLASH_FASTAT_ADDR) != 0x10){
			sil_wrb_mem(FLASH_FASTAT_ADDR, 0x10);
		}

		/* ステータスクリアコマンド発行 */
		*((uint8_t *)DATA_FLASH_BASE_ADDR) = 0x50;
	}

	syslog(LOG_WARNING, "fcu_check_error %02x", status);

	return true;
}

static bool_t fcu_check_frdy(int tWAIT)
{
	int i, j;

	for(i = 0; i < tWAIT; i++){
		/* 準備完了なら終了 */
		if((sil_reb_mem(FLASH_FSTATR0_ADDR) & FLASH_FSTATR0_FRDY_BIT) != 0)
			return true;

		/* 1μs? */
		for(j = 0; j < 100; j++);
	}

	syslog(LOG_WARNING, "fcu_check_frdy timeout");

	/* FCU初期化 */
	fcu_init();

	return false;
}

static void fcu_init()
{
	int j;

	sil_wrh_mem(FLASH_FRESETR_ADDR, 0xCC01);

	// 200μs? */
	for(j = 0; j < 20000; j++);

	sil_wrh_mem(FLASH_FRESETR_ADDR, 0xCC00);
}
