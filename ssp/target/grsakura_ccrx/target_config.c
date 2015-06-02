/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2008-2010 by Witz Corporation, JAPAN
 *  Copyright (C) 2013      by Mitsuhiro Matsuura
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)～(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 */

/*
 *	ターゲットシステム依存モジュール（GR-SAKURA用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "grsakura.h"
#include "rx600/rx630_uart.h"

/*
 *  ターゲットシステム依存 初期化ルーチン
 */
void
target_initialize( void )
{
	/*
	 *  プロセッサ依存の初期化
	 */
	prc_initialize();

	/* IOポート初期化 */
	/* P25を出力ポートにする */
	sil_wrb_mem(PORT2_PDR_ADDR, 
		sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B5_BIT);
	/* PA0, PA1, PA2, PA6を出力ポートにする */
	sil_wrb_mem(PORTA_PDR_ADDR,
		sil_reb_mem(PORTA_PDR_ADDR) | PORT_PDR_B0_BIT | PORT_PDR_B1_BIT | PORT_PDR_B2_BIT | PORT_PDR_B6_BIT);
	/* PA7を入力ポートにする */
	sil_wrb_mem(PORTA_PDR_ADDR,
		sil_reb_mem(PORTA_PDR_ADDR) & ~PORT_PDR_B7_BIT);

	/*
	 *  シリアルポートの設定
	 */
	rx630_uart_init(TARGET_PUTC_PORTID, UART_BAUDRATE, UART_CLKSRC);

	/* ポートP20をTxD0, ポートP21をRxD0に */
	sil_wrb_mem(PORT2_PMR_ADDR, 0x03);

	/* データディレクションレジスタ(PDR)の設定 P21(RxD0)を入力ポートにする */
	sil_wrb_mem(PORT2_PDR_ADDR, 
		sil_reb_mem(PORT2_PDR_ADDR) & ~PORT_PDR_B1_BIT);

	/* データディレクションレジスタ(PDR)の設定 P20(TxD0)を出力ポートにする */
	sil_wrb_mem(PORT2_PDR_ADDR, 
		sil_reb_mem(PORT2_PDR_ADDR) | PORT_PDR_B0_BIT);

	/* ポートP40～P47を周辺機器に */
	sil_wrb_mem(PORT4_PMR_ADDR, 0xFF);

	/* ポートP40～P47を入力ポートにする */
	sil_wrb_mem(PORT4_PDR_ADDR, 0x00);

	/* 書き込みプロテクトレジスタの設定 PFSWEビットへの書き込みを許可 */
	sil_wrb_mem(MPC_PWPR_ADDR, 0x00);
	/* 書き込みプロテクトレジスタの設定 PxxFSレジスタへの書き込みを許可 */
	sil_wrb_mem(MPC_PWPR_ADDR, 0x40);

	/* P21端子機能制御レジスタ RXD0とする */
	sil_wrb_mem(MPC_P21PFS_ADDR, 0x0a);
	/* P20端子機能制御レジスタ TXD0とする */
	sil_wrb_mem(MPC_P20PFS_ADDR, 0x0a);

	/* P40端子機能制御レジスタ アナログ入力AN000とする */
	sil_wrb_mem(MPC_P40PFS_ADDR, 0x80);
	/* P41端子機能制御レジスタ アナログ入力AN001とする */
	sil_wrb_mem(MPC_P41PFS_ADDR, 0x80);
	/* P42端子機能制御レジスタ アナログ入力AN002とする */
	sil_wrb_mem(MPC_P42PFS_ADDR, 0x80);
	/* P43端子機能制御レジスタ アナログ入力AN003とする */
	sil_wrb_mem(MPC_P43PFS_ADDR, 0x80);
	/* P44端子機能制御レジスタ アナログ入力AN004とする */
	sil_wrb_mem(MPC_P44PFS_ADDR, 0x80);
	/* P45端子機能制御レジスタ アナログ入力AN005とする */
	sil_wrb_mem(MPC_P45PFS_ADDR, 0x80);
	/* P46端子機能制御レジスタ アナログ入力AN006とする */
	sil_wrb_mem(MPC_P46PFS_ADDR, 0x80);
	/* P47端子機能制御レジスタ アナログ入力AN007とする */
	sil_wrb_mem(MPC_P47PFS_ADDR, 0x80);

	/* 書き込みプロテクトレジスタの設定 書き込みを禁止 */
	sil_wrb_mem(MPC_PWPR_ADDR, 0x80);
}


/*
 *  ターゲットシステムの終了ルーチン
 */
void
target_exit( void )
{
	/*
	 *	プロセッサ依存の終了処理
	 */
	prc_terminate();

	while ( 1 );
}


/*
 *  システムログの低レベル出力のための文字出力
 */
void
target_fput_log( char c )
{
	if( c == '\n' ){
		rx630_uart_pol_putc( '\r' , TARGET_PUTC_PORTID );
	}

	rx630_uart_pol_putc( c , TARGET_PUTC_PORTID );
}
