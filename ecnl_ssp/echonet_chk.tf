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
$ 必須プロパティのチェック
$ =====================================================================

$FUNCTION compare$
	$RESULT = ARGV[1] - ARGV[2]$
$END$

$ プロファイルオブジェクトスーパークラス
$ProfileObjectSuperClass.EPC = { 0x88, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x9D, 0x9E, 0x9F }$
$ProfileObjectSuperClass.MUST = { EPC_NONE, EPC_RULE_GET, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_RULE_GET, EPC_RULE_GET, EPC_RULE_GET }$

$ ノードプロファイルクラス
$NodeProfileClass.EPC = { 0x80, 0x82, 0x83, 0x89, 0xBF, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7 }$
$NodeProfileClass.MUST = { EPC_RULE_GET, EPC_RULE_GET, EPC_RULE_GET, EPC_NONE, EPC_NONE, EPC_RULE_GET, EPC_RULE_GET, EPC_RULE_ANNO, EPC_RULE_GET, EPC_RULE_GET }$

$ 機器オブジェクトスーパークラス
$DeviceSuperClass.EPC = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x93, 0x97, 0x98, 0x99, 0x9A, 0x9D, 0x9E, 0x9F }$
$DeviceSuperClass.MUST = { EPC_RULE_GET, EPC_RULE_SET|EPC_RULE_GET, EPC_RULE_GET, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_RULE_GET, EPC_NONE, EPC_RULE_GET, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_NONE, EPC_RULE_GET, EPC_RULE_GET, EPC_RULE_GET }$

$ResultOfGetPropertyInfo = {}$

$FUNCTION GetPropertyInfo$
	$eoj.x1 = ARGV[1]$
	$eoj.x2 = ARGV[2]$
	$attr = ARGV[3]$
	$list = {}$
	$FOREACH i { 0 , 1 , ... , 15 }$
		$map[i] = 0x00$
	$END$

	$IF (attr != EPC_ANNOUNCE) && (attr != EPC_RULE_SET) && (attr != EPC_RULE_GET)$
		$ERROR$$FORMAT("%1% is illegal value, must be EPC_RULE_ANNO or EPC_RULE_SET or EPC_RULE_GET.", attr)$$END$
	$END$

	$IF eoj.x1 == EOJ_X1_SENSOR$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_AIR_CONDITIONER$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_AMENITY$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_HOUSEWORK$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_WELLNESS$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_CONTROLLER$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_AUDIO_VISUAL$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$ELIF eoj.x1 == EOJ_X1_PROFILE$
		$i = 0$
		$WHILE i < LENGTH(ProfileObjectSuperClass.EPC)$
			$IF (AT(ProfileObjectSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(ProfileObjectSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
		$IF eoj.x2 == EOJ_X2_NODE_PROFILE$
			$i = 0$
			$WHILE i < LENGTH(NodeProfileClass.EPC)$
				$IF (AT(NodeProfileClass.MUST, i) & attr) != 0$
					$list = APPEND(list, AT(NodeProfileClass.EPC, i))$
				$END$
				$i = i + 1$
			$END$
		$END$
	$ELIF eoj.x1 == EOJ_X1_USER_DEFINED$
		$i = 0$
		$WHILE i < LENGTH(DeviceSuperClass.EPC)$
			$IF (AT(DeviceSuperClass.MUST, i) & attr) != 0$
				$list = APPEND(list, AT(DeviceSuperClass.EPC, i))$
			$END$
			$i = i + 1$
		$END$
	$END$

	$i = 0$
	$WHILE i < LENGTH(list)$
		$pos = AT(list, i) & 0xF$
		$bit = (AT(list, i) >> 4) - 8$
		$map[pos] = map[pos] | (1 << bit)$
		$i = i + 1$
	$END$

	$ResultOfGetPropertyInfo = {}$
	$i = 0$
	$WHILE i < LENGTH(list)$
		$ResultOfGetPropertyInfo = APPEND(ResultOfGetPropertyInfo, AT(list, i))$
		$i = i + 1$
	$END$

	$RESULT = ResultOfGetPropertyInfo$
$END$
