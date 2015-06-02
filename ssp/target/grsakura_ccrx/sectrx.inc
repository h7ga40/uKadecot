;*
;*  TOPPERS/SSP Kernel
;*      Smallest Set Profile Kernel
;* 
;*  Copyright (C) 2008-2010 by Witz Corporation, JAPAN
;* 
;*  上記著作権者は，Free Software Foundation によって公表されている 
;*  GNU General Public License の Version 2 に記述されている条件か，以
;*  下の(1)～(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
;*  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
;*  利用と呼ぶ）することを無償で許諾する．
;*  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
;*      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
;*      スコード中に含まれていること．
;*  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
;*      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
;*      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
;*      この利用条件および下記の無保証規定を掲載すること．
;*  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
;*      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
;*    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
;*        権表示，この利用条件および下記の無保証規定を掲載すること．
;*    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
;*        こと．
;*  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
;*      害からも，上記著作権者を免責すること．
;* 
;*  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
;*  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
;*  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
;*  かなる損害に関しても，その責任を負わない．
;*


;
;  ターゲットシステム依存セクション定義ファイル(GR-SAKURA用)
;

;
;  セクションの定義
;
;  セクションのアドレス定義はHEW上の最適化リンカの
;  オプションにて実施する.
;

	;
	;  初期値有りデータ領域のセクション
	;
	;  00000000h - 00020000h
	;
	.section	D, ROMDATA
	.section	D_2, ROMDATA, ALIGN = 2
	.section	D_1, ROMDATA


	;
	;  初期値無しデータ領域のセクション
	;
	;  00000000h - 00020000h
	;
	.section	B, DATA, ALIGN = 4
	.section	B_2, DATA, ALIGN = 2
	.section	B_1, DATA
	.section	W, ROMDATA,ALIGN = 4
	.section	W_1, ROMDATA
	.section	W_2, ROMDATA, ALIGN = 2


	;
	;  カーネルスタック領域のセクション
	;
	;  00000000h - 00020000h
	;
	;  以下.blkb命令で確保しているカーネルスタック領域のサイズを
	;  変更する場合は, target_config.hのDEFAULT_ISTKSZの値を, 同じ
	;  値に変更すること.
	;
	.section	SI, DATA, ALIGN = 4
	.glb _DEFAULT_ISTACK
_DEFAULT_ISTACK:
	.blkb		0800H
	.glb DEFAULT_ISTACK_TOP
DEFAULT_ISTACK_TOP:

	;
	;  定数領域のセクション
	;
	;  00100000h - 00107FFFh
	;
	.section	C, ROMDATA, ALIGN = 4
	.section	C_2, ROMDATA, ALIGN = 2
	.section	C_1, ROMDATA

	;
	;  初期値有りデータ領域の初期値
	;
	;  00100000h - 00107FFFh
	;
	.section	R, DATA, ALIGN = 4
	.section	R_2, DATA, ALIGN = 2
	.section	R_1, DATA

	;
	;  可変ベクタテーブル領域のセクション
	;
	;  00100000h - 00107FFFh
	;
	.section	C$VECT, ROMDATA, ALIGN = 4
	.glb VVECTOR_TOP
VVECTOR_TOP:

	;
	;  プログラム領域のセクション
	;
	;  FFE80000h - FFFFFF7Fh
	;
	.section	P, CODE

	;
	;  固定ベクタテーブル領域のセクション
	;
	;  FFFFFF80h - FFFFFFFFh
	;
	.section	FVECT, ROMDATA, ALIGN = 4

