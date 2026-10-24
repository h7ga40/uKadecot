"""Check every SVN commit's metadata and every file in the final Git tree."""
from pathlib import Path
import csv
import hashlib
import json
import re
import subprocess
import xml.etree.ElementTree as ET
from datetime import datetime

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT / 'output/uKadecot' if (ROOT / 'output').exists() else ROOT.parents[1]
DOCS = REPO / 'docs/migration'
DOCS.mkdir(parents=True, exist_ok=True)

def git(*args):
    return subprocess.check_output(['git', '-C', str(REPO), *args])

entries = ET.parse(ROOT / 'source/svn-log.xml').getroot().findall('logentry')
commits = git('rev-list', '--reverse', 'refs/remotes/git-svn').decode().splitlines()
assert len(commits) == len(entries), (len(commits), len(entries))
by_revision = {}
for sha in commits:
    author, email, date, body = git('show', '-s', '--format=%an%x00%ae%x00%at%x00%B', sha).decode('utf-8').split('\x00', 3)
    match = re.search(r'\ngit-svn-id: (\S+)@(\d+) ([\w-]+)\s*$', body)
    assert match, sha
    revision = match[2]
    assert revision not in by_revision
    by_revision[revision] = (sha, author, email, date, body[:match.start()].rstrip())
rows = []
for entry in reversed(entries):
    revision = entry.get('revision')
    sha, author, email, date, message = by_revision[revision]
    assert author == entry.findtext('author'), revision
    assert int(date) == int(datetime.fromisoformat(entry.findtext('date')).timestamp()), revision
    assert message == entry.findtext('msg', '').rstrip(), revision
    rows.append([revision, sha, author, email, entry.findtext('date'), message])
with (DOCS / 'svn-revisions.csv').open('w', encoding='utf-8', newline='') as stream:
    writer = csv.writer(stream)
    writer.writerow(['svn_revision', 'git_commit', 'svn_author', 'git_email', 'svn_date', 'message'])
    writer.writerows(rows)

props = {}
prefix = 'https://dev.toppers.jp/svn/contrib/uKadecot/trunk/'
for target in ET.parse(ROOT / 'source/svn-properties.xml').getroot().findall('target'):
    props[target.get('path').removeprefix(prefix)] = {p.get('name'): p.text for p in target.findall('property')}

def canonical(data, properties):
    if 'svn:eol-style' in properties:
        data = data.replace(b'\r\n', b'\n').replace(b'\r', b'\n')
    if 'svn:keywords' in properties:
        for keyword in properties['svn:keywords'].split():
            name = keyword.encode()
            data = re.sub(rb'\$' + name + rb'(?::[^\r\n$]*)?\$', b'$' + name + b'$', data)
    return data

tree = git('ls-tree', '-rz', '--full-tree', 'refs/remotes/git-svn').split(b'\0')
files = {}
for record in filter(None, tree):
    meta, name = record.split(b'\t', 1)
    mode, kind, sha = meta.split()
    assert kind == b'blob' and mode in (b'100644', b'100755'), record
    files[name.decode('utf-8')] = sha.decode()
export = ROOT / 'source/trunk-r473'
export_files = {p.relative_to(export).as_posix() for p in export.rglob('*') if p.is_file()}
assert set(files) == export_files, (set(files) - export_files, export_files - set(files))
hashes = []
normalized = 0
for name, sha in sorted(files.items()):
    original = (export / name).read_bytes()
    imported = git('cat-file', 'blob', sha)
    properties = props.get(name, {})
    assert canonical(original, properties) == canonical(imported, properties), name
    normalized += original != imported
    hashes.append(f'{hashlib.sha256(imported).hexdigest()}  {name}')
(DOCS / 'trunk-sha256.txt').write_text('\n'.join(hashes) + '\n', encoding='utf-8')
readme = (REPO / 'README.md').read_text(encoding='utf-8')
for target in re.findall(r'\]\(([^)]+)\)', readme):
    if not target.startswith(('http:', 'https:', '#')):
        assert (REPO / target).exists(), target
report = {
    'svn_repository_revision': 473,
    'last_trunk_revision': 265,
    'verified_commits': len(rows),
    'verified_files': len(files),
    'files_requiring_svn_eol_or_keyword_normalization': normalized,
    'trunk_head': commits[-1],
    'metadata_checks': 'authors, dates (whole seconds), messages, unique SVN revisions',
    'tree_check': 'all paths and all file contents; only declared SVN EOL/keyword transformations normalized',
    'readme_local_links': 'passed',
}
(DOCS / 'verification.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report, ensure_ascii=False, indent=2))
