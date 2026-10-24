# SVN / Trac 移行記録

2026年10月24日、TOPPERS/μKadecotのSVN trunkとTrac WikiをGitへ移行。

## 移行元と移行先

- SVN: https://dev.toppers.jp/svn/contrib/uKadecot/trunk/
- SVN UUID: `57928105-6641-4540-880c-211d16c16296`
- 取得時のSVNリポジトリ全体のリビジョン: r473
- trunkの最終変更: r265（2016-11-18）
- Wiki: https://dev.toppers.jp/trac_user/contrib/wiki/uKadecot?version=9
- GitHub: https://github.com/h7ga40/uKadecot

## 移行内容

trunkの26コミットを`git svn`で変換し、`main`の履歴として保持した。コミットメッセージ末尾の`git-svn-id`に元URL、SVNリビジョン、UUIDを記録している。SVNのプロパティだけを変更したコミットと空のtrunkを作成した最初のコミットも含む。元の日時はGitの秒精度で保持し、SVNのマイクロ秒を含む日時は対応表に保存した。

2026年10月24日、利用者の指定により、SVN作者`coas-nagasima`に対応する26コミットのAuthorとCommitterを`h7ga40 <13964697+h7ga40@users.noreply.github.com>`へ変更した。GitHubアカウントIDは`13964697`。ソースツリー、Author/Committerの日時、メッセージ、`git-svn-id`は保持した。親コミットの変更により後続のWiki移行コミットもSHAが変わる。元のSVN作者名は履歴原本XMLとリビジョン対応表の`svn_author`列に保持し、Gitの作者名は`git_author`列に記録する。

SVNの`uKadecot/`には`trunk/`と`wiki/`があり、branches・tagsは存在しなかった。`wiki/`内のSVG4点とTrac添付のPNG2点を`docs/images/`へ保存した。Wiki第9版の本文をルートの`README.md`へ変換し、見出し、コードブロック、WebAPI表、画像、商標注記、リンクを転記した。ソースコードと書き込みイメージのダウンロード案内はGitHub内の配置に合わせた。Wikiの`.mod`表記は実ファイルの`.mot`に補正し、その旨をREADMEに明記した。

ライセンス表示を含む既存のソースファイル、`Readme.txt`、配布ツール、書き込みイメージは元のまま保持した。利用条件はルートの[Readme.txt](../../Readme.txt)と各ファイルの表示を参照。

## 検証結果と保存資料

- [verification.json](verification.json): コミット・ファイル検証の結果。
- [svn-revisions.csv](svn-revisions.csv): SVNリビジョンとGitコミットSHA、作者、元日時、メッセージの対応表。
- [authors.txt](authors.txt): SVN作者とGit作者の対応。
- [rewritten-commits.csv](rewritten-commits.csv): 作者変更前後の27コミットのSHA対応（SVN履歴26件とWiki移行コミット1件）。
- [trunk-sha256.txt](trunk-sha256.txt): 最終trunkのGit格納内容のSHA-256一覧。
- [source/svn-log.xml](source/svn-log.xml): SVNの変更パスを含む履歴原本。
- [source/svn-properties.xml](source/svn-properties.xml): r473時点のtrunkのSVNプロパティ。
- [source/uKadecot.trac.txt](source/uKadecot.trac.txt): Trac Wiki記法の原文。
- [scripts/convert_wiki.py](scripts/convert_wiki.py): README変換スクリプト。
- [scripts/verify_migration.py](scripts/verify_migration.py): 履歴・最終ツリー・README内ローカルリンクの検証スクリプト。

全26コミットについてSVN作者からGit作者への対応、noreplyメールアドレス、Committer、日時、メッセージ、SVN番号を検証した。作者変更前後の27コミットではソースツリー・Author/Committer日時・メッセージの一致も照合した。最終trunkの全790ファイルについてパス集合と内容が一致した。うち525ファイルではSVN export時に発生する改行変換と`$Id$`展開を、対象ファイルのSVNプロパティに従って正規化して比較した。バイナリなどそれ以外の内容はそのまま比較した。READMEのローカルリンクと`git fsck --full`も検証を通過した。

GitはSVNの空ディレクトリと任意プロパティを直接管理しない。SVNプロパティは原本XMLを保存した。r265より前の各リビジョンについてファイル内容を個別に再exportしての照合は実施していない。専用コンパイラや実機を用いたビルド・動作試験は今回の移行検証には含まない。

## 再現手順

Git for Windowsの`git svn`、SVN CLI、Python 3が必要。新しい作業フォルダーに本記録の`authors.txt`をコピーしてから以下を実行する。これらのコマンドはローカル変換のみを行う。

```powershell
git svn clone --no-minimize-url --authors-file=authors.txt https://dev.toppers.jp/svn/contrib/uKadecot/trunk/ uKadecot
git -C uKadecot branch -m main
```

変換後、この移行記録の`docs/`とルート`README.md`をコピーする。元の取得時点を再検証する場合は、リポジトリのルートから以下を実行する。

```powershell
svn export --non-interactive -r 473 https://dev.toppers.jp/svn/contrib/uKadecot/trunk/ docs/migration/source/trunk-r473
python docs/migration/scripts/verify_migration.py
python docs/migration/scripts/convert_wiki.py
git fsck --full
```

`verify_migration.py`はローカルの`refs/remotes/git-svn`を使用する。GitHubから通常cloneした環境で同じ検証を行う場合は、対応表のr265のSHAを使い、最初に`git update-ref refs/remotes/git-svn <r265のSHA>`を実行する。

作者変更前のGit履歴は作業環境の`migration/source/before-author-rewrite.bundle`へバックアップした。今回書き換えた作業環境の`.git/svn`内部キャッシュは旧SHAを含むため、そのままSVN連携には使用しない。SVNから再取得する場合は上記の作者対応ファイルを指定して新規変換する。

## 今回の対象外

Trac Ticket・コメント・マイルストーンの移行、Wiki旧版履歴のGitコミット化、SVN/Tracの書き込み停止とサービス設定変更は行っていない。元Wikiに記載された外部サービスや動画へのリンクは原文を保持しており、現在のサービス動作は保証しない。
