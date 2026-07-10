from pathlib import Path
root = Path('EngineTests')
updated = []
for path in root.rglob('*.cpp'):
    text = path.read_text(encoding='utf-8')
    if '\u2014' in text:
        path.write_text(text.replace('\u2014', '-'), encoding='utf-8')
        updated.append(str(path))
print('Updated files:', len(updated))
for p in updated:
    print(p)
