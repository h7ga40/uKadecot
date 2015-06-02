$ =====================================================================
$ 
$  TOPPERS ECHONET Lite Communication Middleware
$ 
$  Copyright (C) 2014 Cores Co., Ltd. Japan
$ 
$  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
$  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
$  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
$  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
$      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
$      スコード中に含まれていること．
$  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
$      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
$      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
$      の無保証規定を掲載すること．
$  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
$      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
$      と．
$    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
$        作権表示，この利用条件および下記の無保証規定を掲載すること．
$    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
$        報告すること．
$  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
$      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
$      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
$      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
$      免責すること．
$ 
$  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
$  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
$  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
$  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
$  の責任を負わない．
$ 
$  @(#) $Id$
$ 
$ =====================================================================

$ =====================================================================
$ echonet_cfg.hの生成
$ =====================================================================

$FILE "echonet_cfg.h"$
/* echonet_cfg.h */$NL$
#ifndef TOPPERS_ECHONET_CFG_H$NL$
#define TOPPERS_ECHONET_CFG_H$NL$
$NL$

#include "echonet_app_config.h"$NL$
$NL$

$ ノードリストの作成
$ENOD.LIST = {}$
$LOCAL_NODE = {}$
$REMOTE_NODE = {}$

$FOREACH eobjid EOBJ.ORDER_LIST$
$	// ノードプロファイルオブジェクトの場合
	$IF (EOBJ.EOBJATR[eobjid] & EOBJ_LOCAL_NODE) != 0$
		$LOCAL_NODE = APPEND(LOCAL_NODE, eobjid)$
$	// 機器オブジェクトの場合
	$ELIF (EOBJ.EOBJATR[eobjid] & (EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
		$REMOTE_NODE = APPEND(REMOTE_NODE, eobjid)$
	$END$
$END$

$	// ローカルのノードプロファイルは１つだけ
$IF LENGTH(LOCAL_NODE) != 1$
	$ERROR$Local node must be only one.$END$
$END$

$ENOD.LIST = APPEND(LOCAL_NODE, REMOTE_NODE)$

#define TNUM_ENODID	$LENGTH(ENOD.LIST)$$NL$
$NL$

#define TNUM_EOBJID	$LENGTH(EOBJ.ID_LIST)$$NL$
$NL$
$FOREACH id EOBJ.ID_LIST$
	#define $id$	$+id$$NL$
$END$
$NL$

$FOREACH eobjid EOBJ.ID_LIST$
	$eprp_list = {}$
	$prp_map_cnt = 3$
$	// ノードプロファイルの場合
	$IF (EOBJ.EOBJATR[eobjid] & (EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
		$prp_map_cnt = prp_map_cnt + 5$
	$END$

	$FOREACH eprpno EPRP.ID_LIST$
		$IF EQ(EPRP.EOBJID[eprpno], eobjid)$
			$eprp_list = APPEND(eprp_list, eprpno)$
		$END$

		$IF (EPRP.EPRPCD[eprpno] == 0x9D) || (EPRP.EPRPCD[eprpno] == 0x9E) || (EPRP.EPRPCD[eprpno] == 0x9F)$
			$prp_map_cnt = prp_map_cnt - 1$
		$END$

$		// ノードプロファイルの場合
		$IF (EOBJ.EOBJATR[eobjid] & (EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
			$IF (EPRP.EPRPCD[eprpno] >= 0xD3) && (EPRP.EPRPCD[eprpno] <= 0xD7)$
				$prp_map_cnt = prp_map_cnt - 1$
			$END$
		$END$
	$END$

	#define TNUM_$EOBJ.EOBJID[eobjid]$_EPRPNO	$LENGTH(eprp_list) + prp_map_cnt$$NL$
$END$$NL$

$FOREACH enodid ENOD.LIST$
	$eobj_count = 0$

	$FOREACH eobjid EOBJ.ID_LIST$
		$IF EQ(EOBJ.ENODID[eobjid], enodid)$
			$eobj_count = eobj_count + 1$
		$END$
	$END$

	#define TNUM_$EOBJ.EOBJID[enodid]$_EOBJNO	$eobj_count$$NL$
$END$$NL$

#ifndef TNUM_ENODADR$NL$
#define TNUM_ENODADR 100$NL$
#endif$NL$
$NL$

#endif /* TOPPERS_ECHONET_CFG_H */$NL$

$ =====================================================================
$ echonet_cfg.cの生成
$ =====================================================================

$FILE "echonet_cfg.c"$
/* echonet_cfg.c */$NL$
#include "echonet.h"$NL$
#include "echonet_cfg.h"$NL$
$NL$

$ 
$  インクルードディレクティブ（#include）
$ 
/*$NL$
$SPC$*  Include Directives (#include)$NL$
$SPC$*/$NL$
$NL$
$INCLUDES$
$NL$

$ 
$  オブジェクトのID番号を保持する変数
$ 
$IF USE_EXTERNAL_ID$
	/*$NL$
	$SPC$*  Variables for Object ID$NL$
	$SPC$*/$NL$
	$NL$
	$FOREACH id EOBJ.ID_LIST$
		const ID $id$_id$SPC$=$SPC$$+id$;$NL$
	$END$
$END$

$ 
$  ECHONET Liteプロパティ
$ 
/*$NL$
$SPC$*  ECHONET Lite Property Functions$NL$
$SPC$*/$NL$
$NL$

$INCLUDE "echonet_chk.tf"$

$ ECHONET Liteプロパティ初期化ブロックの生成
$IF LENGTH(EOBJ.ID_LIST) && LENGTH(EPRP.ID_LIST)$
	$FOREACH enodid ENOD.LIST$
		$INST_LIST = {}$
		$CLASS_LIST = {}$

		$FOREACH eobjid EOBJ.ID_LIST$
			$IF EQ(EOBJ.ENODID[eobjid], enodid)$
				$INST_LIST = APPEND(INST_LIST, eobjid)$

				$M = -1$
				$FOREACH id CLASS_LIST$
					$IF (EOBJ.EOJX1[eobjid] == EOBJ.EOJX1[id]) && (EOBJ.EOJX2[eobjid] == EOBJ.EOJX2[id])$
						$M = id$
					$END$
				$END$
				$IF M == -1$
					$CLASS_LIST = APPEND(CLASS_LIST, eobjid)$
				$END$
			$END$
		$END$

		/*$NL$
		$SPC$* インスタンス数$NL$
		$SPC$*/$NL$
		$inst_list_len = LENGTH(INST_LIST)$
		const uint8_t _echonet_$EOBJ.EOBJID[enodid]$_inst_count[] = {$NL$
			$TAB$$FORMAT("0x%1$x, 0x%2$x, 0x%3$x", (inst_list_len & 0xFF0000) >> 16, (inst_list_len & 0xFF00) >> 8, inst_list_len & 0xFF)$$NL$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* クラス数$NL$
		$SPC$*/$NL$
$		// ノードプロファイルも含める（ + 1）
		$class_list_len = LENGTH(CLASS_LIST) + 1$
		const uint8_t _echonet_$EOBJ.EOBJID[enodid]$_class_count[] = {$NL$
			$TAB$$FORMAT("0x%1$x, 0x%2$x", (class_list_len & 0xFF00) >> 8, class_list_len & 0xFF)$$NL$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* インスタンスリスト$NL$
		$SPC$*/$NL$
		$pos = 0$
		$IF inst_list_len > 84$
			$ERROR EPRP.TEXT_LINE[eprpno]$E_PAR: $FORMAT(_("instance list count overflow, %1% %2% > 84"), enodid, inst_list_len)$$END$
		$END$
		const uint8_t _echonet_$EOBJ.EOBJID[enodid]$_inst_list[] = {$NL$
		$IF inst_list_len > 0xFE$
			$TAB$0xFF, 
		$ELSE$
			$TAB$$FORMAT("0x%1$x, ", inst_list_len)$
		$END$
		$JOINEACH id INST_LIST ", "$
			$FORMAT("0x%1$x, 0x%2$x, 0x%3$x", +EOBJ.EOJX1[id], +EOBJ.EOJX2[id], +EOBJ.EOJX3[id])$
		$END$$NL$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* クラスリスト$NL$
		$SPC$*/$NL$
		$pos = 0$
		$class_list_len = LENGTH(CLASS_LIST)$
		$IF class_list_len > 84$
			$ERROR EPRP.TEXT_LINE[eprpno]$E_PAR: $FORMAT(_("class list count overflow, %1% %2% > 84"), enodid, class_list_len)$$END$
		$END$
		const uint8_t _echonet_$EOBJ.EOBJID[enodid]$_class_list[] = {$NL$
		$IF class_list_len > 0xFE$
			$TAB$0xFF, 
		$ELSE$
			$TAB$$FORMAT("0x%1$x, ", class_list_len)$
		$END$
		$JOINEACH id CLASS_LIST ", "$
			$FORMAT("0x%1$x, 0x%2$x", +EOBJ.EOJX1[id], +EOBJ.EOJX2[id])$
		$END$$NL$
		};$NL$
		$NL$
	$END$

	$FOREACH eobjid EOBJ.ID_LIST$
		$eprp_list = {}$
		$anno_list = {}$
		$set_list = {}$
		$get_list = { 0x9D, 0x9E, 0x9F }$
		$FOREACH i { 0 , 1 , ... , 15 }$
			$anno_map[i] = 0x00$
			$set_map[i] = 0x00$
			$get_map[i] = 0x00$
		$END$
$		// 0x9D、0x9E、0x9FはONにしておく
		$FOREACH i { 13 , 14 , 15 }$
			$anno_map[i] = anno_map[i] | 0x02$
			$get_map[i] = get_map[i] | 0x02$
		$END$
$		// ノードプロファイルの場合
		$IF (EOBJ.EOBJATR[eobjid] & (EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
			$anno_map[5] = anno_map[5] | 0x20$
			$anno_list = APPEND(anno_list, { 0xD5 })$
			$FOREACH i { 3 , 4 , 6 , 7 }$
				$get_map[i] = get_map[i] | 0x20$
			$END$
			$get_list = APPEND(get_list, { 0xD3, 0xD4, 0xD6, 0xD7 })$
		$END$

		$FOREACH eprpno EPRP.ID_LIST$
			$IF EQ(EPRP.EOBJID[eprpno], eobjid)$
$				// (0x80 <= eprpcd && eprpcd <= 0xFF)でない場合（E_PAR）
				$IF !(0x80 <= EPRP.EPRPCD[eprpno] && EPRP.EPRPCD[eprpno] <= 0xFF)$
					$ERROR EPRP.TEXT_LINE[eprpno]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eprpcd", EPRP.EPRPCD[eprpno], eprpno, "ECN_DEF_EPRP")$$END$
				$END$

$				// eprpatrが（［EPC_RULE_SET] | [EPC_RULE_GET] | [EPC_RULE_ANNO] | [EPC_ANNOUNCE］| [EPC_VARIABLE]）でない場合（E_RSATR）
				$IF (EPRP.EPRPATR[eprpno] & ~(EPC_RULE_SET|EPC_RULE_GET|EPC_RULE_ANNO|EPC_ANNOUNCE|EPC_VARIABLE)) != 0$
					$ERROR EPRP.TEXT_LINE[eprpno]$E_RSATR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eprpatr", EPRP.EPRPATR[eprpno], eprpno, "ECN_DEF_EPRP")$$END$
				$END$

$				// (0 <= eprpsz && eprpsz <= 255)でない場合（E_PAR）
				$IF !(0 <= EPRP.EPRPSZ[eprpno] && EPRP.EPRPSZ[eprpno] <= 255)$
					$ERROR EPRP.TEXT_LINE[eprpno]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eprpsz", EPRP.EPRPSZ[eprpno], eprpno, "ECN_DEF_EPRP")$$END$
				$END$

$				// プロパティマップに設定
				$pos = EPRP.EPRPCD[eprpno] & 0xF$
				$bit = (EPRP.EPRPCD[eprpno] >> 4) - 8$
				$IF (EPRP.EPRPATR[eprpno] & EPC_ANNOUNCE) != 0$
					$anno_list = APPEND(anno_list, EPRP.EPRPCD[eprpno])$
					$anno_map[pos] = anno_map[pos] | (1 << bit)$
				$END$
				$IF (EPRP.EPRPATR[eprpno] & EPC_RULE_SET) != 0$
					$set_list = APPEND(set_list, EPRP.EPRPCD[eprpno])$
					$set_map[pos] = set_map[pos] | (1 << bit)$
				$END$
				$IF (EPRP.EPRPATR[eprpno] & EPC_RULE_GET) != 0$
					$get_list = APPEND(get_list, EPRP.EPRPCD[eprpno])$
					$get_map[pos] = get_map[pos] | (1 << bit)$
				$END$

				$eprp_list = APPEND(eprp_list, eprpno)$
			$END$
		$END$

$		// 必須アナウンスプロパティがあるかチェック
		$must_anno_map = GetPropertyInfo(EOBJ.EOJX1[eobjid], EOBJ.EOJX2[eobjid], EPC_ANNOUNCE)$
		$i = 0$
		$WHILE i < LENGTH(ResultOfGetPropertyInfo)$
			$j = FIND(anno_list, +AT(ResultOfGetPropertyInfo, i))$
			$IF !LENGTH(j)$
				$ERROR$$FORMAT("Required announce property (0x%1$x) of %2% is not defined.", +AT(ResultOfGetPropertyInfo, i), eobjid)$$END$
			$END$
			$i = i + 1$
		$END$

$		// 必須SETプロパティがあるかチェック
		$must_set_map = GetPropertyInfo(EOBJ.EOJX1[eobjid], EOBJ.EOJX2[eobjid], EPC_RULE_SET)$
		$i = 0$
		$WHILE i < LENGTH(ResultOfGetPropertyInfo)$
			$j = FIND(set_list, +AT(ResultOfGetPropertyInfo, i))$
			$IF !LENGTH(j)$
				$ERROR$$FORMAT("Required set property (0x%1$x) of %2% is not defined.", +AT(ResultOfGetPropertyInfo, i), eobjid)$$END$
			$END$
			$i = i + 1$
		$END$

$		// 必須GETプロパティがあるかチェック
		$must_get_map = GetPropertyInfo(EOBJ.EOJX1[eobjid], EOBJ.EOJX2[eobjid], EPC_RULE_GET)$
		$i = 0$
		$WHILE i < LENGTH(ResultOfGetPropertyInfo)$
			$j = FIND(get_list, +AT(ResultOfGetPropertyInfo, i))$
			$IF !LENGTH(j)$
				$ERROR$$FORMAT("Required get property (0x%1$x) of %2% is not defined.", +AT(ResultOfGetPropertyInfo, i), eobjid)$$END$
			$END$
			$i = i + 1$
		$END$

		/*$NL$
		$SPC$* アナウンスプロパティマップ$NL$
		$SPC$*/$NL$
		$anno_list = LSORT(anno_list, "compare")$
		$anno_len = LENGTH(anno_list)$
		const uint8_t _echonet_$EOBJ.EOBJID[eobjid]$_anno_prpmap[] = {$NL$
		$TAB$$FORMAT("0x%1$x" , +anno_len)$, 
		$IF anno_len < 16$
			$JOINEACH eprpcd anno_list ", "$
				$FORMAT("0x%1$x" , +eprpcd)$
			$END$$NL$
			$anno_len = anno_len + 1$
		$ELSE$
			$JOINEACH i anno_map ", "$
				$FORMAT("0x%1$x" , +anno_map[i])$
			$END$$NL$
			$anno_len = 17$
		$END$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* SETプロパティマップ$NL$
		$SPC$*/$NL$
		$set_list = LSORT(set_list, "compare")$
		$set_len = LENGTH(set_list)$
		const uint8_t _echonet_$EOBJ.EOBJID[eobjid]$_set_prpmap[] = {$NL$
		$TAB$$FORMAT("0x%1$x" , +set_len)$, 
		$IF set_len < 16$
			$JOINEACH eprpcd set_list ", "$
				$FORMAT("0x%1$x" , +eprpcd)$
			$END$$NL$
			$set_len = set_len + 1$
		$ELSE$
			$JOINEACH i set_map ", "$
				$FORMAT("0x%1$x" , +set_map[i])$
			$END$$NL$
			$set_len = 17$
		$END$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* GETプロパティマップ$NL$
		$SPC$*/$NL$
		$get_list = LSORT(get_list, "compare")$
		$get_len = LENGTH(get_list)$
		const uint8_t _echonet_$EOBJ.EOBJID[eobjid]$_get_prpmap[] = {$NL$
		$TAB$$FORMAT("0x%1$x" , +get_len)$, 
		$IF get_len < 16$
			$JOINEACH eprpcd get_list ", "$
				$FORMAT("0x%1$x" , +eprpcd)$
			$END$$NL$
			$get_len = get_len + 1$
		$ELSE$
			$JOINEACH i { 0 , 1 , ... , 15 } ", "$
				$FORMAT("0x%1$x" , +get_map[i])$
			$END$$NL$
			$get_len = 17$
		$END$
		};$NL$
		$NL$

		/*$NL$
		$SPC$* プロパティ定義$NL$
		$SPC$*/$NL$
		const EPRPINIB _echonet_$EOBJ.EOBJID[eobjid]$_eprpinib_table[TNUM_$EOBJ.EOBJID[eobjid]$_EPRPNO] = {$NL$
		$FOREACH eprpno eprp_list$
$			// ECHONET Liteプロパティ初期化ブロック
			$TAB${ ($EPRP.EPRPCD[eprpno]$), ($EPRP.EPRPATR[eprpno]$), ($EPRP.EPRPSZ[eprpno]$), ($EPRP.EXINF[eprpno]$), ($EPRP.EPRPSET[eprpno]$), ($EPRP.EPRPGET[eprpno]$) },$NL$
		$END$
$		// 状態アナウンスプロパティマップ
		$TAB${ (0x9D), (EPC_RULE_GET), $FORMAT("0x%1$x" , +anno_len)$, (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_anno_prpmap, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$		// Setプロパティマップ
		$TAB${ (0x9E), (EPC_RULE_GET), $FORMAT("0x%1$x" , +set_len)$, (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_set_prpmap, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$		// Getプロパティマップ
		$TAB${ (0x9F), (EPC_RULE_GET), $FORMAT("0x%1$x" , +get_len)$, (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_get_prpmap, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) }
$		// ノードプロファイルの場合
		$IF (EOBJ.EOBJATR[eobjid] & (EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
			,$NL$
$			// インスタンス数
			$TAB${ (0xD3), (EPC_RULE_GET), (3), (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_inst_count, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$			// クラス数
			$TAB${ (0xD4), (EPC_RULE_GET), (2), (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_class_count, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$			// インスタンスリスト通知
			$TAB${ (0xD5), (EPC_RULE_ANNO), sizeof(_echonet_$EOBJ.EOBJID[eobjid]$_inst_list), (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_inst_list, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$			// インスタンスリストS
			$TAB${ (0xD6), (EPC_RULE_GET), sizeof(_echonet_$EOBJ.EOBJID[eobjid]$_inst_list), (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_inst_list, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) },$NL$
$			// クラスリストS
			$TAB${ (0xD7), (EPC_RULE_GET), sizeof(_echonet_$EOBJ.EOBJID[eobjid]$_class_list), (intptr_t)_echonet_$EOBJ.EOBJID[eobjid]$_class_list, ((EPRP_SETTER *)NULL), ((EPRP_GETTER *)ecn_data_prop_get) }$NL$
		$ELSE$
			$NL$
		$END$
		};$NL$
		$NL$
	$END$$NL$

$ELSE$
	TOPPERS_EMPTY_LABEL(const EPRPINIB, _echonet_eprpinib_table);$NL$
$END$

$ 
$  ECHONET Liteオブジェクト
$ 
/*$NL$
$SPC$*  ECHONET Lite Object Functions$NL$
$SPC$*/$NL$
$NL$

$ ECHONET LiteオブジェクトID番号の最大値
const ID _echonet_tmax_eobjid = (TMIN_EOBJID + TNUM_EOBJID - 1);$NL$
$NL$

$ ECHONET Liteオブジェクト初期化ブロックの生成
$IF LENGTH(EOBJ.ID_LIST)$
	const EOBJINIB _echonet_eobjinib_table[TNUM_EOBJID] = {$NL$
	$JOINEACH eobjid EOBJ.ID_LIST ",\n"$
$		// eobjatrが（［EOBJ_LOCAL_NODE] | [EOBJ_SYNC_REMOTE_NODE] | [EOBJ_ASYNC_REMOTE_NODE] | [EOBJ_DEVICE］）でない場合（E_RSATR）
		$IF (EOBJ.EOBJATR[eobjid] & ~(EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE|EOBJ_DEVICE)) != 0$
			$ERROR EOBJ.TEXT_LINE[eobjid]$E_RSATR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eobjatr", EOBJ.EOBJATR[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
		$END$

$		// ノードプロファイルオブジェクトの場合
		$IF (EOBJ.EOBJATR[eobjid] & (EOBJ_LOCAL_NODE|EOBJ_SYNC_REMOTE_NODE|EOBJ_ASYNC_REMOTE_NODE)) != 0$
$			// (enodid == EOBJ_NULL)でない場合（E_PAR）
			$IF !EQ(EOBJ.ENODID[eobjid], EOBJ_NULL)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "enodid", EOBJ.ENODID[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (eojx1 == EOJ_X1_PROFILE)でない場合（E_PAR）
			$IF EOBJ.EOJX1[eobjid] != EOJ_X1_PROFILE$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx1", EOBJ.EOJX1[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (eojx2 == EOJ_X2_NODE_PROFILE)でない場合（E_PAR）
			$IF EOBJ.EOJX2[eobjid] != EOJ_X2_NODE_PROFILE$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx2", EOBJ.EOJX2[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (1 <= eojx3 && eojx3 <= 2)でない場合（E_PAR）
			$IF !(1 <= EOBJ.EOJX3[eobjid] && EOBJ.EOJX3[eobjid] <= 2)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx3", EOBJ.EOJX3[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$
$		// 機器オブジェクトの場合
		$ELSE$
$			// (enodid != EOBJ_NULL)でない場合（E_PAR）
			$IF EQ(EOBJ.ENODID[eobjid], EOBJ_NULL)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "enodid", EOBJ.ENODID[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (0 <= eojx1 && eojx1 <= 255)でない場合（E_PAR）
			$IF !(0 <= EOBJ.EOJX1[eobjid] && EOBJ.EOJX1[eobjid] <= 255)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx1", EOBJ.EOJX1[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (0 <= eojx2 && eojx2 <= 255)でない場合（E_PAR）
			$IF !(0 <= EOBJ.EOJX2[eobjid] && EOBJ.EOJX2[eobjid] <= 255)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx2", EOBJ.EOJX2[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$

$			// (0 <= eojx3 && eojx3 <= 255)でない場合（E_PAR）
			$IF !(0 <= EOBJ.EOJX3[eobjid] && EOBJ.EOJX3[eobjid] <= 255)$
				$ERROR EOBJ.TEXT_LINE[eobjid]$E_PAR: $FORMAT(_("illegal %1% `%2%\' of `%3%\' in %4%"), "eojx3", EOBJ.EOJX3[eobjid], eobjid, "ECN_CRE_EOBJ")$$END$
			$END$
		$END$

$		// ECHONET Liteオブジェクト初期化ブロック
		$TAB${ ($EOBJ.EOBJATR[eobjid]$), ($EOBJ.ENODID[eobjid]$), ($EOBJ.EXINF[eobjid]$), ($EOBJ.EOJX1[eobjid]$), ($EOBJ.EOJX2[eobjid]$), ($EOBJ.EOJX3[eobjid]$), _echonet_$EOBJ.EOBJID[eobjid]$_eprpinib_table, TNUM_$EOBJ.EOBJID[eobjid]$_EPRPNO }
	$END$$NL$
	};$NL$
	$NL$
$ELSE$
	TOPPERS_EMPTY_LABEL(const EOBJINIB, _echonet_eobjinib_table);$NL$
$END$
$NL$

$ ECHONET Liteノード管理ブロックの数
const int _echonet_tnum_enodid = TNUM_ENODID;$NL$
$NL$

$ 
$  ECHONET Liteノード管理ブロック
$ 
/*$NL$
$SPC$*  ECHONET Lite Node Functions$NL$
$SPC$*/$NL$
$NL$
$ ECHONET LiteノードとIPアドレスの対応情報ブロックの生成
$FOREACH enodid ENOD.LIST$
	$eobj_list = {}$
	$FOREACH eobjid EOBJ.ID_LIST$
		$IF EQ(EOBJ.ENODID[eobjid], enodid)$
			$eobj_list = APPEND(eobj_list, eobjid)$
		$END$
	$END$
	/*$NL$
	$SPC$* ノードに所属する機器オブジェクトの一覧$NL$
	$SPC$*/$NL$
	const EOBJINIB *_echonet_$EOBJ.EOBJID[enodid]$_eobjinib_table[TNUM_$EOBJ.EOBJID[enodid]$_EOBJNO] = {$NL$
	$JOINEACH eobjid eobj_list ",\n"$
		$TAB${ &_echonet_eobjinib_table[$eobjid$ - 1] }
	$END$$NL$
	};$NL$
$END$$NL$

$IF LENGTH(ENOD.LIST)$
	/*$NL$
	$SPC$* ECHONET Liteノード管理ブロック$NL$
	$SPC$*/$NL$
	EOBJCB _echonet_eobjcb_table[TNUM_ENODID] = {$NL$
	$JOINEACH enodid ENOD.LIST ",\n"$
		$TAB${ &_echonet_eobjinib_table[$enodid$ - 1], _echonet_$EOBJ.EOBJID[enodid]$_eobjinib_table, TNUM_$EOBJ.EOBJID[enodid]$_EOBJNO }
	$END$$NL$
	};$NL$
$ELSE$
	TOPPERS_EMPTY_LABEL(EOBJCB, _echonet_eobjcb_table);$NL$
$END$
$NL$

$ ECHONET LiteノードとIPアドレスの対応情報の数
const int _echonet_tnum_enodadr = TNUM_ENODADR;$NL$
$NL$

$ 
$  ECHONET LiteノードとIPアドレスの対応情報
$ 
/*$NL$
$SPC$*  ECHONET Lite Node Functions$NL$
$SPC$*/$NL$
$NL$
$ ECHONET LiteノードとIPアドレスの対応情報ブロックの生成
$IF LENGTH(EOBJ.ID_LIST)$
	ENODADRB _echonet_enodadrb_table[TNUM_ENODADR] = {$NL$
	$JOINEACH enodid ENOD.LIST ",\n"$
		$IF enodid == 1$
			$TAB$/* ENOD_MULTICAST_ID $TAB$*/ { true, ECHONET_MULTICAST_ADDR },$NL$
			$TAB$/* ENOD_LOCAL_ID     $TAB$*/ { true, },$NL$
			$TAB$/* ENOD_API_ID       $TAB$*/ { true, }
		$ELSE$
			$TAB$/* ENOD_REMOTE_ID + $enodid - 4$$TAB$*/ { true, }
		$END$
	$END$$NL$
	};$NL$
$ELSE$
	TOPPERS_EMPTY_LABEL(ENODADRB, _echonet_enodadrb_table);$NL$
$END$
$NL$

$ ECHONET Liteノードごとのアプリケーションが要求した電文のシーケンス番号
uint16_t _echonet_apitid_table[TNUM_ENODADR];$NL$
$NL$

$NL$
#include "kernel_cfg.h"$NL$
$NL$

$
$ ECHONET Lite カーネルオブジェクトID
$
const ID _echonet_ecn_svc_taskid = ECHONET_SVC_TASK;$NL$
const ID _echonet_ecn_udp_taskid = ECHONET_UDP_TASK;$NL$
const ID _echonet_ecn_api_mailboxid = ECHONET_API_MAILBOX;$NL$
const ID _echonet_ecn_svc_mailboxid = ECHONET_SVC_MAILBOX;$NL$
const ID _echonet_ecn_udp_mailboxid = ECHONET_UDP_MAILBOX;$NL$
const ID _echonet_ecn_svc_cychdrid = ECHONET_SVC_CYCHDR;$NL$
const ID _echonet_ecn_udp_cychdrid = ECHONET_UDP_CYCHDR;$NL$
$NL$
