$ 
$     asm_config.inc 生成用
$     パス2のアーキテクチャ依存テンプレート（RX630用）
$ 

$FILE "asm_config.inc"$


TIPM_LOCK $TAB$$TAB$ .equ $TAB$ $+TIPM_LOCK$ $NL$
IPL_LOCK $TAB$$TAB$ .equ $TAB$ $+IPL_LOCK$ $NL$
PSW_I_MASK $TAB$$TAB$ .equ $TAB$ $+PSW_I_MASK$ $NL$
PSW_IPL_MASK $TAB$$TAB$ .equ $TAB$ $+PSW_IPL_MASK$ $NL$
EXC_GET_PSW_OFFSET $TAB$$TAB$ .equ $TAB$ $+EXC_GET_PSW_OFFSET$ $NL$
