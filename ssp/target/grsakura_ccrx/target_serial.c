/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
 *  Copyright (C) 2003-2004 by Platform Development Center
 *                                          RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2008-2010 by Witz Corporation, JAPAN
 *  Copyright (C) 2013      by Mitsuhiro Matsuura
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
 */

/*
 *	RX630 UART用シリアルI/Oモジュール
 */
#include "kernel_impl.h"
#include <sil.h>
#include "target_serial.h"

/*
 *  SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	rx630_uart_initialize();
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB  *p_siopcb = NULL;
	ER      ercd;
	INTNO	intno_sio_tx, intno_sio_rx;
	
	/*
	 *  シリアルI/O割込みをマスクする．
	 *  (dis_int関数は、"\kernel\interrupt.c"に記述)
	 */
	p_siopcb = rx630_uart_get_siopcb(siopid);
	intno_sio_tx = rx630_uart_intno_tx(p_siopcb);
	intno_sio_rx = rx630_uart_intno_rx(p_siopcb);
	ercd = dis_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = dis_int(intno_sio_rx);
	assert(ercd == E_OK);
	
	p_siopcb = 
		rx630_uart_opn_por(siopid , exinf , UART_BAUDRATE , UART_CLKSRC);

	/*
	 *  シリアルI/O割込みをマスク解除する．
	 *  (ena_int関数は、"\kernel\interrupt.c"に記述)
	 */
	ercd = ena_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = ena_int(intno_sio_rx);
	assert(ercd == E_OK);

	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	ER        ercd;
	INTNO	intno_sio_tx, intno_sio_rx;

	/*
	 *  デバイス依存のクローズ処理．
	 */
	rx630_uart_cls_por(p_siopcb);
	
	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	intno_sio_tx = rx630_uart_intno_tx(p_siopcb);
	intno_sio_rx = rx630_uart_intno_rx(p_siopcb);
	ercd = dis_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = dis_int(intno_sio_rx);
	assert(ercd == E_OK);
}

/*
 *  SIOの割込みハンドラ
 */
void sio_tx_isr(intptr_t exinf)
{
	rx630_uart_tx_isr(exinf);
}

/*
 *  SIOの割込みハンドラ
 */
void sio_rx_isr(intptr_t exinf)
{
	rx630_uart_rx_isr(exinf);
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char c)
{
	return(rx630_uart_snd_chr(siopcb, c));
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
	return(rx630_uart_rcv_chr(siopcb));
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	rx630_uart_ena_cbr(siopcb, cbrtn);
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	rx630_uart_dis_cbr(siopcb, cbrtn);
}

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
void
rx630_uart_irdy_snd(intptr_t exinf)
{
	/* 共通部（syssvc\serial.c）にあるsio_irdy_snd関数を呼び出し*/
	sio_irdy_snd(exinf);
}

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
void
rx630_uart_irdy_rcv(intptr_t exinf)
{
	/* 共通部（syssvc\serial.c）にあるsio_irdy_rcv関数を呼び出し*/
	sio_irdy_rcv(exinf);
}

