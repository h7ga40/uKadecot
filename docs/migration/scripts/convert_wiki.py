"""Convert the archived uKadecot Trac Wiki to repository Markdown."""
from pathlib import Path
import re
import shutil

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT / 'output/uKadecot' if (ROOT / 'output').exists() else ROOT.parents[1]
source = (ROOT / 'source/uKadecot.trac.txt').read_text(encoding='utf-8-sig')
images = REPO / 'docs/images'
images.mkdir(parents=True, exist_ok=True)
for image in (ROOT / 'source/wiki-r473').glob('*.svg'):
    shutil.copyfile(image, images / image.name)
for name in ('ECNLCtrl1.png', 'ECNLCtrl2.png'):
    image = ROOT / 'source' / name
    assert image.read_bytes().startswith(b'\x89PNG\r\n\x1a\n'), name
    shutil.copyfile(image, images / name)

def inline(text):
    text = re.sub(r'\[\[Image\((?:source:/uKadecot/wiki/)?([^,)]+)[^)]*\)\]\]',
                  lambda m: f'![{m[1]}](docs/images/{m[1]})', text)
    text = re.sub(r'\[\[br\]\]', '<br>\n', text, flags=re.I)
    text = re.sub(r'\{\{\{(.*?)\}\}\}', r'`\1`', text)
    text = re.sub(r'\[(https?://\S+) ([^\]]+)\]', r'[\2](\1)', text)
    text = text.replace('^[#kadecotlicense ※]^', '[※](#kadecotlicense)')
    text = text.replace('[=#kadecotlicense]', '<a id="kadecotlicense"></a>')
    return text

lines = source.splitlines()
out = []
i = 0
while i < len(lines):
    line = lines[i]
    if line == '{{{':
        out.append('```text')
        i += 1
        while lines[i] != '}}}':
            out.append(lines[i])
            i += 1
        out.append('```')
    elif line.startswith('||'):
        if out and out[-1]:
            out.append('')
        rows = []
        while i < len(lines) and lines[i].startswith('||'):
            cells = [inline(c.strip()) for c in lines[i][2:-2].split('||')]
            rows.append([c for c in cells if c])
            i += 1
        # Trac empty cells encode colspans; use a consistent three-column table.
        out.extend(['| 項目 | 名前 | 内容 |', '| --- | --- | --- |'])
        for cells in rows:
            if len(cells) == 1:
                cells = cells + ['', ''] if cells[0].endswith('：') else ['', '', cells[0]]
            elif len(cells) == 2:
                if cells[0] in ('Arduino関数：', 'WAMP Procedure名：'):
                    cells.insert(1, '')
                else:
                    cells.insert(0, '')
            cells = [c.replace('|', '\\|') for c in cells]
            out.append('| ' + ' | '.join(cells) + ' |')
        continue
    elif match := re.fullmatch(r'(=+) (.*?) \1', line):
        if out and out[-1]:
            out.append('')
        out.append('#' * (len(match[1]) + (0 if not out else 1)) + ' ' + match[2])
        out.append('')
    elif line.startswith('* '):
        out.append('- ' + inline(line[2:]))
    else:
        out.append(inline(line))
    i += 1

text = '\n'.join(out)
old = 'ソースコードは[こちら](http://dev.toppers.jp/trac/contrib/browser/uKadecot)にあります。trunkの行の「Size」列にある↓をクリックすると、zipファイルでダウンロードできます。<br>\n書き込みイメージは[こちら](http://dev.toppers.jp/trac/contrib/browser/uKadecot/trunk/csp/flash/)にある「ukadecot_YYMMDDhhmmss.mod」をダウンロードしてください。'
new = 'ソースコードは[GitHubリポジトリ](https://github.com/h7ga40/uKadecot)から取得できます。「Code」→「Download ZIP」でダウンロードしてください。\n\n書き込みイメージは[csp/flash/](csp/flash/)の `ukadecot_*.mot` をダウンロードしてください。元Wikiの拡張子表記は `.mod` ですが、SVNに格納されている実ファイルは `.mot` です。'
old = old.replace('<br>\n', '<br>\n\n')
assert old in text
text = text.replace(old, new)
text += '\n\n---\n\n出典：[Trac Wiki uKadecot](https://dev.toppers.jp/trac_user/contrib/wiki/uKadecot?version=9)（第9版）。本文をMarkdownに変換し、画像を同梱しました。ダウンロード案内は移行後の配置に合わせて更新しています。\n\nSVN履歴・リビジョン対応表については[移行記録](docs/migration/README.md)を参照してください。\n'
assert not re.search(r'\[\[|\{\{\{|\|\|', text)
(REPO / 'README.md').write_text(text, encoding='utf-8', newline='\n')
print('Converted Wiki to README.md; copied', len(list(images.iterdir())), 'images')
