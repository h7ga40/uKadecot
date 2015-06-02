/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2006 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2007-2010 by Witz Corporation, JAPAN
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 */

/*
 *  t_stddef.hの開発環境依存部（ルネサス RXツールチェイン用）
 *
 *  このインクルードファイルは，t_stddef.h の先頭でインクルードされる．
 *  他のファイルからは直接インクルードすることはない．他のインクルード
 *  ファイルに先立って処理されるため，他のインクルードファイルに依存し
 *  てはならない．
 */

#if !defined( TOPPERS_TOOL_STDDEF_H )
#define TOPPERS_TOOL_STDDEF_H

/*
 *  コンパイラの拡張機能のためのマクロ定義
 */
/*
 *  inline 識別子は外部定義を生成しない.
 */
#define	Inline		static inline

/*
 *  インラインアセンブラは本コンパイラでは
 *  #pragma inline_asmのみのサポートとなるため,
 *  asm/Asm定義は行わない.
 */

/*
 *  NoReturn / alignofは本コンパイラではサポートされない.
 *  offsetofはstddef.hの定義を利用する.
 */
#define	NoReturn


/*
 *  開発環境の標準インクルードファイルの利用
 *
 *  NULLの定義をstddef.hから，INT_MAX，INT_MIN，UINT_MAX，LONG_MAX，
 *  LONG_MIN，ULONG_MAX，CHAR_BITの定義をlimits.hから取り込む．
 *
 *  C++/EC++では，標準仕様上はこれらのインクルードファイルが用意されて
 *  いるとは限らないので注意が必要である（ほとんどの開発環境で用意され
 *  ている）．
 */
#ifndef TOPPERS_MACRO_ONLY
#include <stddef.h>
#include <limits.h>
#endif /* TOPPERS_MACRO_ONLY */


/*
 *  コンパイラ依存のデータ型の定義
 *
 *  stdint.hをインクルードする
 */
#ifndef TOPPERS_MACRO_ONLY
#include <stdint.h>
#endif /* TOPPERS_MACRO_ONLY */


/*
 *  浮動小数点型に関する定義
 *
 *  下記の定義はdbl_size = 8オプションを指定した場合のみ有効とすること.
 */
#ifndef TOPPERS_MACRO_ONLY
typedef float		float32_t;			/* IEEE754準拠の単精度浮動小数点数 */
typedef double		double64_t;			/* IEEE754準拠の倍精度浮動小数点数 */
#endif /* TOPPERS_MACRO_ONLY */

#define FLOAT32_MIN		( 1.4012984643248171e-45F )
#define FLOAT32_MAX		( 3.4028234663852886e+38F )
#define DOUBLE64_MIN	( 4.9406564584124654e-324 )
#define DOUBLE64_MAX	( 1.7976931348623157e+308 )


/*
 *  空ラベルの定義
 *
 *  本コンパイラでは大きさゼロの配列はコンパイルエラーになるため,
 *  サイズ1にて定義する.
 */
#define TOPPERS_EMPTY_LABEL( type, symbol )		type symbol[ 1 ]


#endif /* TOPPERS_TOOL_STDDEF_H */

