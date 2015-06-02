$ 
$     asm_config.inc 生成用
$     パス2のターゲット依存テンプレート（GR-SAKURA用）
$ 
$     Copyright (C) 2010 by Witz Corporation, JAPAN

$ 

$ 
$  プロセッサ依存のテンプレートファイルのインクルード
$ 
$INCLUDE"rx630_ccrx/prc_asm_config.tf"$

$FILE "asm_config.inc"$

$NL$

LOG_INH_ENTER_ENABLE $TAB$ .equ $TAB$ $+LOG_INH_ENTER_ENABLE$ $NL$
LOG_INH_LEAVE_ENABLE $TAB$ .equ $TAB$ $+LOG_INH_LEAVE_ENABLE$ $NL$
LOG_EXC_ENTER_ENABLE $TAB$ .equ $TAB$ $+LOG_EXC_ENTER_ENABLE$ $NL$
LOG_EXC_LEAVE_ENABLE $TAB$ .equ $TAB$ $+LOG_EXC_LEAVE_ENABLE$ $NL$
SIL_DLY_TIM1 $TAB$ .equ $TAB$ $+SIL_DLY_TIM1$ $NL$
SIL_DLY_TIM2 $TAB$ .equ $TAB$ $+SIL_DLY_TIM2$ $NL$
