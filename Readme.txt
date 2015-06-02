
	TOPPERS/ECNL（Release 1.1.a）
		TOPPERS ECHONET Lite Communication Middleware

【はじめに】

TOPPERS/ECNLは，ECHONET Liteの機器向け通信ミドルウェアです．
コントローラ対応中の公開前の版を使用し，Kadecot APIを実装した家電サーバの
サンプルプログラムです．
TOPPERS/ECNLについては以下のWebページをお読みください．

	https://www.toppers.jp/ecnl.html

この版では，TOPPERS/ASP＋TINETではなく，TOPPERS/SSP＋uIPを使用しています．

【利用条件】

TOPPERS/ECNLの利用条件は，各ファイルの先頭に表示されているTOPPERSライセ
ンスです．TOPPERSライセンスに関するFAQが，以下のページにあります．

	http://www.toppers.jp/faq/faq_ct12.html

このパッケージには，サンプルプログラムに必要なTOPPERS/SSPの他，以下のソ
フトウェアが含まれており，使用する場合はそれぞれの利用条件に従ってくだ
さい．

・uIP：IPスタック
	https://github.com/adamdunkels/uip
・Petit FAT File System Module：Webサーバ用ファイルシステム
	http://elm-chan.org/fsw/ff/00index_p.html
・mod_websocket：WebSocket（base64,sha1を使用）
	https://github.com/nori0428/mod_websocket
・JSONSL：JSONパーサー
	https://github.com/mnunberg/jsonsl
・jQuery
	http://jquery.com
・jQuery Mobile
	http://jquerymobile.com
・Saltarelle C# to JavaScript Compiler
	http://www.saltarelle-compiler.com
・Unity用Kadecot WAMPライブラリ
	http://kadecot.net/blog/2842/

「Kadecot」は株式会社ソニーコンピュータサイエンス研究所の商標です．

【コンパイル方法】

GR-SAKURAの実行ファイルのコンパイルに，Renesas CubeSuite+を使用します．
アプリケーションと，Kadecotで使用する名称テーブル，Webアプリケーション
のファイルシステムの，３つのファイルを結合して，書き込みイメージを作成
します．

・TOPPERS/SSPライブラリのコンパイル
	csp\libssp\libssp.mtpj
→libssp.a

・アプリケーションのコンパイル
	csp\ukadecot\ukadecot.mtpj
→ukadecot.mod

・コントローラWebアプリケーションのコンパイル
	tools\EcnlControllerUI\EcnlControllerUI.sln
→ctrlui.js

・Webアプリケーションのコピー/minify
	tools\EcnlControllerUI\EcnlCtrlUI\copy_httpd-fs.bat
→httpd-fs\*

・Webアプリケーションのファイルシステム化
	uip\apps\webserver\makefsdata.exe
→httpd-fs.bin

・３つのファイル（ukadecot.mod, httpd-fs.bin, kadecot_names.bin）を結合
	csp\flash\ukadecot_cat.bat
→ukadecot_YYMMDDhhmmss.mod

最後の「ukadecot_YYMMDDhhmmss.mod」が，GR-SAKURAへの書き込みイメージにな
ります．

【質問・バグレポート・意見等の送付先】

TOPPERS/ECNLをより良いものにするためのご意見等を歓迎します．TOPPERS/ECNL
に関する質問やバグレポート，ご意見等は，TOPPERSプロジェクトの会員は
TOPPERS開発者メーリングリスト（dev@toppers.jp）宛またはTOPPERS/ECNLのバ
グトラッキングシステム（Trac）に，その他の方はTOPPERSユーザーズメーリン
グリスト（users@toppers.jp）宛にお願いします．

TOPPERSユーザーズメーリングリストへの登録方法については，以下のページに
説明があります．

	http://www.toppers.jp/community.html

以上
