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

#define CAST(type, val)		((type)(val))

#include <kernel.h>
#include <sil.h>
#include <t_syslog.h>
#include <string.h>
#include "kernel_cfg.h"
#include "kernel/kernel_impl.h"
#include "uip_target_config.h"
#include "uip_arp.h"
#include "if_rx62n.h"
#include "ether_phy.h"

extern uint8_t mac_addr[ETHER_ADDR_LEN];

/*
 *  ネットワークインタフェースに依存するソフトウェア情報 
 */

typedef struct t_rx62n_softc {
	T_RX62N_TX_DESC *tx_write;
	T_RX62N_RX_DESC *rx_read;
	PHY_STATE_T state;
} T_RX62N_SOFTC;

/*
 *  ネットワークインタフェースのソフトウェア情報
 */

/* ネットワークインタフェースに依存するソフトウェア情報 */

static T_RX62N_SOFTC rx62n_softc;

typedef struct t_rx62n_buf {
	uint8_t rx_buff[NUM_IF_RX62N_RXBUF][32 * ((IF_RX62N_BUF_PAGE_SIZE + 31) / 32)];
	uint8_t tx_buff[NUM_IF_RX62N_TXBUF][32 * ((IF_RX62N_BUF_PAGE_SIZE + 31) / 32)];
	T_RX62N_RX_DESC rx_desc[NUM_IF_RX62N_RXBUF];
	T_RX62N_TX_DESC tx_desc[NUM_IF_RX62N_TXBUF];
} T_RX62N_BUF;

#if defined(__RX)
#pragma	section	ETH_MEMORY
#endif
T_RX62N_BUF rx62n_buf;
#if defined(__RX)
#pragma	section
#endif

/* ネットワークインタフェースに依存しないソフトウェア情報 */

T_IF_SOFTC if_softc = {
//	{0,},						/* ネットワークインタフェースのアドレス	*/
//	0,							/* 送信タイムアウト			*/
	&rx62n_softc,				/* ディバイス依存のソフトウェア情報	*/
};

/*
 *  局所変数
 */

static void rx62n_stop (T_RX62N_SOFTC *sc);
static void rx62n_init_sub (T_IF_SOFTC *ic);
static void rx62n_set_ecmr (T_IF_SOFTC *ic, PHY_MODE_T mode);

/*
 *  rx62n_stop -- ネットワークインタフェースを停止する。
 * 
 *    注意: NIC 割り込み禁止状態で呼び出すこと。
 */

static void
rx62n_stop (T_RX62N_SOFTC *sc)
{
	/* 動作モードクリア */
	sil_wrw_mem(ETHERC_ECMR, 0x00000000);
}

/*
 *  rx62n_init_sub -- ネットワークインタフェースの初期化
 * 
 *    注意: NIC 割り込み禁止状態で呼び出すこと。
 */

static void
rx62n_init_sub (T_IF_SOFTC *ic)
{
	int i;
	PHY_STATE_T state = PHY_STATE_UNINIT;

	/* MAC部ソフトウエア・リセット */
	sil_wrw_mem(EDMAC_EDMR, sil_rew_mem(EDMAC_EDMR) | EDMAC_EDMR_SWR_BIT);

	/* リセット完了待ち */
	for (i = 0; i < 10000; i++) {
	}

	sil_wrw_mem(ETHERC_MAHR, ((uint32_t)mac_addr[0] << 24)
		| ((uint32_t)mac_addr[1] << 16) | ((uint32_t)mac_addr[2] << 8 )
		|  (uint32_t)mac_addr[3]);
	sil_wrw_mem(ETHERC_MALR, ((uint32_t)mac_addr[4] << 8 )
		| (uint32_t)mac_addr[5]);

	/* PHYリセット */
	while((state = phy_reset(state, 0)) != PHY_STATE_RESET);
	ic->sc->state = state;

	/* Clear all ETHERC status BFR, PSRTO, LCHNG, MPD, ICD */
	sil_wrw_mem(ETHERC_ECSR, 0x00000037);

	/* リンク変化割り込み有効 */
	sil_wrw_mem(ETHERC_ECSIPR, sil_rew_mem(ETHERC_ECSIPR) | ETHERC_ECSIPR_LCHNGIP);

	/* Clear all ETHERC and EDMAC status bits */
	sil_wrw_mem(EDMAC_EESR, 0x47FF0F9F);

	/* 送受信割り込み有効 */
	sil_wrw_mem(EDMAC_EESIPR, (EDMAC_EESIPR_TCIP | EDMAC_EESIPR_FRIP | EDMAC_EESIPR_RDEIP | EDMAC_EESIPR_FROFIP));

	/* 受信フレーム長上限（バッファサイズ） */
	sil_wrw_mem(ETHERC_RFLR, IF_RX62N_BUF_PAGE_SIZE);

	/* 96ビット時間（初期値） */
	sil_wrw_mem(ETHERC_IPGR, 0x00000014);

	/* Set little endian mode */
	sil_wrw_mem(EDMAC_EDMR, sil_rew_mem(EDMAC_EDMR) | EDMAC_EDMR_DE_BIT);

	/* Initialize Rx descriptor list address */
	sil_wrw_mem(EDMAC_RDLAR, (uint32_t)rx62n_buf.rx_desc);
	/* Initialize Tx descriptor list address */
	sil_wrw_mem(EDMAC_TDLAR, (uint32_t)rx62n_buf.tx_desc);
	/* Copy-back status is RFE & TFE only */
	sil_wrw_mem(EDMAC_TRSCER, 0x00000000);
	/* Threshold of Tx_FIFO */
	sil_wrw_mem(EDMAC_TFTR, 0x00000000);
	/* Transmit fifo & receive fifo is 2048 bytes */
	sil_wrw_mem(EDMAC_FDR, 0x00000707);
	/* RR in EDRRR is under driver control */
	sil_wrw_mem(EDMAC_RMCR, 0x00000001);

	/* PHYの初期化を促す */
	ic->link_pre = true;
	ic->link_now = false;

	/* ターゲット依存部の割込み初期化 */
	rx62n_inter_init();
}

/*
 *  rx62n_set_ecmr -- ECMRレジスタの設定
 */

static void
rx62n_set_ecmr (T_IF_SOFTC *ic, PHY_MODE_T mode)
{
	uint32_t ecmr;

	ecmr = ETHERC_ECMR_RE | ETHERC_ECMR_TE/* | ETHERC_ECMR_PRM*/;

	if ((mode & 0x01) != 0)
		ecmr |= ETHERC_ECMR_DM;
	if ((mode & 0x02) != 0)
		ecmr |= ETHERC_ECMR_RTM;

	/* 動作モード設定 */
	sil_wrw_mem(ETHERC_ECMR, ecmr);
}

/*
 * rx62n_reset -- ネットワークインタフェースをリセットする。
 */

void
rx62n_reset (T_IF_SOFTC *ic)
{
	/* NIC からの割り込みを禁止する。*/
	dis_int(INTNO_IF_RX62N_TRX);

	rx62n_stop(ic->sc);
	rx62n_init_sub(ic);

	/* NIC からの割り込みを許可する。*/
	ena_int(INTNO_IF_RX62N_TRX);
}

/*
 *  get_rx62n_softc -- ネットワークインタフェースのソフトウェア情報を返す。
 */

T_IF_SOFTC *
rx62n_get_softc (void)
{
	return &if_softc;
}

/*
 * rx62n_watchdog -- ネットワークインタフェースのワッチドッグタイムアウト
 */

void
rx62n_watchdog (T_IF_SOFTC *ic)
{
	rx62n_reset(ic);
}

/*
 * rx62n_probe -- ネットワークインタフェースの検出
 */

void
rx62n_probe (T_IF_SOFTC *ic)
{
	/* ターゲット依存部のバスの初期化 */
	rx62n_bus_init();

	memcpy(uip_ethaddr.addr, mac_addr, sizeof(uip_ethaddr.addr));
}

/*
 * rx62n_init -- ネットワークインタフェースの初期化
 */

void
rx62n_init (T_IF_SOFTC *ic)
{
	T_RX62N_SOFTC *sc = ic->sc;
	T_RX62N_TX_DESC *tdsc;
	T_RX62N_RX_DESC *rdsc;
	int i;

	/* NIC からの割り込みを禁止する。*/
	dis_int(INTNO_IF_RX62N_TRX);

	tdsc = (T_RX62N_TX_DESC *)rx62n_buf.tx_desc;
	sc->tx_write = tdsc;
	for ( i=0 ; i < NUM_IF_RX62N_TXBUF ; i++ ) {
		memset(tdsc, 0, sizeof(*tdsc));
		tdsc->tbl = 0;
		tdsc->tba = (uint32_t)&rx62n_buf.tx_buff[i];
		tdsc++;
	}
	tdsc--;
	tdsc->tdle = 1;

	rdsc = (T_RX62N_RX_DESC *)rx62n_buf.rx_desc;
	sc->rx_read = rdsc;
	for ( i=0 ; i < NUM_IF_RX62N_RXBUF ; i++ ) {
		memset(rdsc, 0, sizeof(*rdsc));
		rdsc->rbl = IF_RX62N_BUF_PAGE_SIZE;
		rdsc->rba = (uint32_t)&rx62n_buf.rx_buff[i];
		rdsc->rfl = 0;
		rdsc->ract = 1;
		rdsc++;
	}
	rdsc--;
	rdsc->rdle = 1;

	/* rx62n_init 本体を呼び出す。*/
	rx62n_init_sub(ic);

	if (sil_rew_mem(EDMAC_EDRRR) == 0) {
		sil_wrw_mem(EDMAC_EDRRR, EDMAC_EDRRR_RR);
	}

	/* NIC からの割り込みを許可する。*/
	ena_int(INTNO_IF_RX62N_TRX);
}

/* 
 * rx62n_link -- リンク状態の変化に対する処理
 */
bool_t
rx62n_link(T_IF_SOFTC *ic)
{
	T_RX62N_SOFTC *sc = ic->sc;
	PHY_MODE_T mode;

	if(sc->state == PHY_STATE_NEGOTIATED){
		ic->link_now = phy_is_link(0);
		if(!ic->link_now)
			sc->state = PHY_STATE_RESET;
		return true;
	}

	/* PHYの初期化 */
	sc->state = phy_initialize(sc->state, 0, &mode);
	if(sc->state != PHY_STATE_NEGOTIATED){
		return false;
	}

	/* ECMRレジスタの設定 */
	rx62n_set_ecmr(ic, mode);
	return true;
}

/*
 * rx62n_read -- フレームの読み込み
 */

int
rx62n_read (T_IF_SOFTC *ic, void **input)
{
	T_RX62N_SOFTC *sc = ic->sc;
	T_RX62N_RX_DESC *desc;
	uint16_t len;

	if (ic->over_flow) {
		ic->over_flow = false;
	}

	desc = sc->rx_read;

	if (desc->ract != 0) {
		return 0;
	}

	len = desc->rfl;
	memcpy(*input, (void *)desc->rba, len);

	desc->rfp = 0;
	desc->ract = 1;

	desc++;
	if (desc == &rx62n_buf.rx_desc[NUM_IF_RX62N_RXBUF]) {
		desc = rx62n_buf.rx_desc;
	}
	sc->rx_read = desc;

	if (sil_rew_mem(EDMAC_EDRRR) == 0) {
		sil_wrw_mem(EDMAC_EDRRR, EDMAC_EDRRR_RR);
	}

	if ((ic->rxb_read == ic->rxb_write) && (desc->ract == 0))
		ic->rxb_read--;

	return len;
}

/*
 * rx62n_start -- 送信フレームをバッファリングする。
 */

bool_t
rx62n_start (T_IF_SOFTC *ic, void *output, int size)
{
	T_RX62N_SOFTC *sc = ic->sc;
	T_RX62N_TX_DESC *desc, *next;
	uint8_t *buf = NULL;
	int32_t len, res, pos;
	uint32_t tfp;

	for ( res = size, pos = 0; res > 0; res -= len, pos += len ) {
		desc = sc->tx_write;

		if (desc->tact != 0) {
			return false;
		}

		buf = (uint8_t *)desc->tba;

		next = desc + 1;
		if (next == &rx62n_buf.tx_desc[NUM_IF_RX62N_TXBUF]) {
			next = rx62n_buf.tx_desc;
		}
		sc->tx_write = next;

		len = res;
		if ( len > IF_RX62N_BUF_PAGE_SIZE ) {
			len = IF_RX62N_BUF_PAGE_SIZE;
			tfp = 0x0;
		}
		else
			tfp = 0x1;

		if (pos == 0)
			tfp |= 0x2;

		memcpy(buf, (uint8_t *)output + pos, len);

		desc->tbl = len;
		desc->tfp = tfp;
		desc->tact = 1;
	}

	if (sil_rew_mem(EDMAC_EDTRR) == 0) {
		sil_wrw_mem(EDMAC_EDTRR, EDMAC_EDTRR_TR);
	}

	return true;
}

/*
 *  RX62N Ethernet Controler 送受信割り込みハンドラ
 */

void
if_rx62n_trx_handler (void)
{
	T_IF_SOFTC *ic;
	T_RX62N_SOFTC *sc;
	uint32_t ecsr, eesr, psr;
	bool_t acttsk = false;

	i_begin_int(INTNO_IF_RX62N_TRX);

	ic = &if_softc;
	sc = ic->sc;

	ecsr = sil_rew_mem(ETHERC_ECSR);

	if (ecsr & ETHERC_ECSR_LCHNG) {
		/* ETHERC部割り込み要因クリア */
		sil_wrw_mem(ETHERC_ECSR, ETHERC_ECSR_LCHNG);

		psr = sil_rew_mem(ETHERC_PSR);
		ic->link_now = (psr & ETHERC_PSR_LMON) != 0;

		/* リンク状態に変化あり */
		if (ic->link_pre != ic->link_now) {
			/* 受信割り込み処理 */
			acttsk = true;
		}
	}

	eesr = sil_rew_mem(EDMAC_EESR);

	if (eesr & EDMAC_EESR_FR) {
		/* DMA部割り込み要因クリア */
		sil_wrw_mem(EDMAC_EESR, EDMAC_EESR_FR);

		/* 受信割り込み処理 */
		acttsk = true;
		ic->rxb_write++;
	}
	if (eesr & EDMAC_EESR_TC) {
		/* DMA部割り込み要因クリア */
		sil_wrw_mem(EDMAC_EESR, EDMAC_EESR_TC);

		/* 送信割り込み処理 */
		acttsk = true;
	}
	if (eesr & (EDMAC_EESR_FROF | EDMAC_EESR_RDE)) {
		/* DMA部割り込み要因クリア */
		sil_wrw_mem(EDMAC_EESR, EDMAC_EESR_FROF | EDMAC_EESR_RDE);

		ic->over_flow = true;

		/* 受信割り込み処理 */
		acttsk = true;
		ic->rxb_write++;
	}

	if(acttsk)
		iact_tsk(UIP_TASK);

	i_end_int(INTNO_IF_RX62N_TRX);
}
