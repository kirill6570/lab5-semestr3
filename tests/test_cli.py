"""Интеграционные проверки запуска и физических файлов. Python 3, без пакетов."""
import subprocess
import tempfile
from pathlib import Path
from html.parser import HTMLParser

ROOT = Path(__file__).resolve().parents[1]
BIN = ROOT / 'build/lab5'

class Images(HTMLParser):
    def __init__(self):
        super().__init__(); self.paths = []
    def handle_starttag(self, tag, attrs):
        if tag == 'img': self.paths.append(dict(attrs)['src'])

with tempfile.TemporaryDirectory() as temporary:
    out = Path(temporary) / 'reports'
    p = subprocess.run([str(BIN), '--demo', str(out)], cwd=ROOT, capture_output=True, text=True)
    assert p.returncode == 0, p.stderr
    names = ['Программирование', 'Информатика', 'Технологии и методы программирования']
    for index, name in enumerate(names, 1):
        report = out / f'discipline-{index}/report.html'
        text = report.read_text()
        assert name in text and '<table>' in text and '<code>' in text
        parser = Images(); parser.feed(text)
        assert all((report.parent / path).is_file() for path in parser.paths)
    bad = subprocess.run([str(BIN), '--unknown'], cwd=ROOT, capture_output=True)
    assert bad.returncode == 1
    missing = subprocess.run([str(BIN), '--demo'], cwd=temporary, capture_output=True)
    assert missing.returncode == 1
    # Неверный выбор повторно запрашивается, не принимается молча.
    answer = 'oops\n4\n1\n0\n5\nТема <b>\nЦель\nЗадание\nВывод\n\n'
    interactive = subprocess.run([str(BIN)], cwd=ROOT, input=answer, capture_output=True, text=True)
    assert interactive.returncode == 0, interactive.stderr
    assert interactive.stdout.count('Введите целое число') == 3
    custom = (ROOT / 'generated/custom/report.html').read_text()
    assert 'Тема &lt;b&gt;' in custom
print('CLI: три дисциплины, все файлы ресурсов, некорректный ввод и ошибки файлов — OK')
