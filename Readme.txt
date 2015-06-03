
	TOPPERS/uKadecot（Release 1.0.0）
		TOPPERS ECHONET Lite Communication Middleware
		Sample Application for WebApp Contoller

【はじめに】

TOPPERS/uKadecotは，ECHONET Liteの機器向け通信ミドルウェアTOPPERS/ECNL
の，公開前のコントローラ対応中の版を使用し，Kadecot APIを部分実装した家
電サーバのサンプルプログラムです．
TOPPERS/ECNLについては以下のWebページをお読みください．

	https://www.toppers.jp/ecnl.html

この版では，TOPPERS/ASP＋TINETではなく，TOPPERS/SSP＋uIPを使用していま
す．

Kadecotについては以下のWebページをお読みください．

	http://kadecot.net/

「Kadecot」は株式会社ソニーコンピュータサイエンス研究所の商標です．

GR-SAKURAへの書き込みイメージのファイルは以下のフォルダにあります．

	csp\flash\ukadecot_20150603105310.mot

【利用条件】

TOPPERS/uKadecotの利用条件は，各ファイルの先頭に表示されているTOPPERSラ
イセンスです．TOPPERSライセンスに関するFAQが，以下のページにあります．

	http://www.toppers.jp/faq/faq_ct12.html

このパッケージには，サンプルプログラムに必要なTOPPERS/SSPの他，以下のソ
フトウェアが含まれており，使用する場合はそれぞれの利用条件に従ってくだ
さい．

・uIP：IPスタック
	https://github.com/adamdunkels/uip
・Petit FAT File System Module：Webサーバ用ファイルシステム
	http://elm-chan.org/fsw/ff/00index_p.html
・http-parser：HTTPパーサー
	https://github.com/joyent/http-parser
・mod_websocket：WebSocket（base64,sha1を使用）
	https://github.com/nori0428/mod_websocket
・JSONSL：JSONパーサー
	https://github.com/mnunberg/jsonsl
・jQuery
	http://jquery.com
・jQuery Mobile
	http://jquerymobile.com
・jQueryMobile - DateBox
	http://dev.jtsage.com/jQM-DateBox/
・Saltarelle C# to JavaScript Compiler
	http://www.saltarelle-compiler.com
・linq.js - LINQ for JavaScript
	http://linqjs.codeplex.com/
・Unity用Kadecot WAMPライブラリ
	http://kadecot.net/blog/2842/

また，実行ファイル作成に使用するツールには，以下のソフトウェアを含んで
います．

・DynamicJson
	http://dynamicjson.codeplex.com/
・FatFs - Generic FAT File System Module
	http://elm-chan.org/fsw/ff/00index_e.html
・zlib
	http://zlib.net/
・JSMin：The JavaScript Minifier
	http://www.crockford.com/javascript/jsmin.html
・SRecord
	http://srecord.sourceforge.net/

【コンパイル方法】

GR-SAKURAの実行ファイルのコンパイルに，Renesas CubeSuite+を使用します．
ツールのコンパイルには，Visual Studio 2012以降を使用します．
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

・Kadecot API用名称テーブル作成ツールのコンパイル，実行
	tools\KadecotNames\KadecotNames.sln
→KadecotNames.bin

・Kadecot API用名称テーブルのコピー
	tools\KadecotNames\copy_kadecot_names.bat
→kadecot_names.bin

・３つのファイル（ukadecot.mod,httpd-fs.bin,kadecot_names.bin）を結合
	csp\flash\ukadecot_cat.bat
→ukadecot_YYMMDDhhmmss.mod

最後の「ukadecot_YYMMDDhhmmss.mod」が，GR-SAKURAへの書き込みイメージに
なります．
各手順は変更したところだけ実行し，最後の手順を行えば，書き込みイメージ
を作成することが出来ます．

コントローラWebアプリケーションでは，NuGetパッケージを使用していますが，
含まれていません．Visual Studio 2012では，ビルド時にダウンロードされま
せんので，以下の操作を行ってください．
Visual Studioで「EcnlControllerUI.sln」を開き，「ソリューション エクス
プローラー」の「ソリューション 'EcnlControllerUI'」を右クリックして，
「NuGet パッケージの復元の有効化」を選択し，表示されたダイアログでOKを
押します．
設定が完了すると，ビルド時にNuGetパッケージがダウンロードされるようにな
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
