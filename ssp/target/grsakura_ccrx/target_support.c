/*-----------------------------------------------------------------------------
 *  TOPPERS/SSP Windows Debug Environment
 *  Copyright (C) 2014 Cores Co., Ltd. Japan
 *-----------------------------------------------------------------------------
 * $Id$
 */

/*
 *	ターゲットシステム依存モジュール（AP-RX62N-0A用）
 */

#include "kernel_impl.h"
#include <sil.h>

void hardware_init_hook()
{
	/*
	 * 動作モード設定
	 *
	 * 動作モードはボード上の端子によって決定されるため,
	 * ここでは特別設定しない.
	 * ただし, リトルエンディアン/シングルチップモード
	 * であることを前提とする.
	 *
	 * またリセット後, 内蔵ROM：有効, 内蔵RAM：有効, 外部バス：無効
	 * である.
	 */

	/*
	 * クロック設定
	 *
	 * 入力：12MHz
	 * PLL回路：12 x 8 = 96MHz
	 * システムクロック：96MHz
	 * 周辺モジュールクロック：48MHz
	 * 外部バスクロック：96MHz
	 * とする.
	 */
	sil_wrb_mem(PORT3_PDR_ADDR,
		sil_reb_mem(PORT3_PDR_ADDR) & ~PORT_PDR_B6_BIT);
	sil_wrb_mem(PORT3_PDR_ADDR,
		sil_reb_mem(PORT3_PDR_ADDR) & ~PORT_PDR_B7_BIT);
	sil_wrb_mem(PORT3_PMR_ADDR,
		sil_reb_mem(PORT3_PMR_ADDR) & ~PORT_PDR_B6_BIT);
	sil_wrb_mem(PORT3_PMR_ADDR,
		sil_reb_mem(PORT3_PMR_ADDR) & ~PORT_PDR_B7_BIT);

	sil_wrh_mem(SYSTEM_PRCR_ADDR, 0xa503);

	/* メインクロック発振器強制発振 */
	while((sil_reb_mem(SYSTEM_MOFCR_ADDR) & SYSTEM_MOFCR_MOFXIN) == 0)
		sil_wrb_mem(SYSTEM_MOFCR_ADDR,
			sil_reb_mem(SYSTEM_MOFCR_ADDR) | SYSTEM_MOFCR_MOFXIN);

	/* メインクロック発振器動作 */
	while((sil_reb_mem(SYSTEM_MOSCCR_ADDR) & SYSTEM_MOSCCR_MOSTP) != 0)
		sil_wrb_mem(SYSTEM_MOSCCR_ADDR,
			sil_reb_mem(SYSTEM_MOSCCR_ADDR) & ~SYSTEM_MOSCCR_MOSTP);

	sil_wrh_mem(SYSTEM_PLLCR_ADDR,
		(sil_reh_mem(SYSTEM_PLLCR_ADDR) & ~SYSTEM_PLLCR_STC_MASK)
		| (0x0f << SYSTEM_PLLCR_STC_OFFSET));

	sil_wrb_mem(SYSTEM_PLLCR2_ADDR,
		sil_reb_mem(SYSTEM_PLLCR2_ADDR) & ~SYSTEM_PLLCR2_PLLEN);

	sil_wrw_mem(SYSTEM_SCKCR_ADDR, 0x21021211);

	/* PLL回路選択 */
	sil_wrh_mem(SYSTEM_SCKCR3_ADDR,
		(sil_reh_mem(SYSTEM_SCKCR3_ADDR) & ~SYSTEM_SCKCR3_CKSEL_MASK)
		| (4 << SYSTEM_SCKCR3_CKSEL_OFFSET));

	/*
	 * モジュールストップ
	 *
	 * リセット後, 動作している内蔵周辺モジュールはDMAC, DTC,
	 * 内蔵RAMのみである.
	 * それ以外の内蔵周辺モジュールに関しては使用する側で
	 * 設定すること.
	 */
	sil_wrh_mem(SYSTEM_PRCR_ADDR, 0xa500);
}

void software_init_hook()
{
	/*
	 * 本コンパイラではweak definitionの機能が無いため,
	 * 特に必要な処理はないが, 必ず呼び出すこととする.
	 */
}
