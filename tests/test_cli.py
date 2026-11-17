"""Интеграционные проверки запуска и физических файлов. Python 3, без пакетов."""
import shutil
import signal
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
    shutil.copy2(ROOT / 'main.cpp', Path(temporary) / 'main.cpp')
    shutil.copytree(ROOT / 'assets', Path(temporary) / 'assets')
    # Неверный выбор повторно запрашивается, не принимается молча.
    answer = 'oops\n4\n1\n0\n5\nТема <b>\nЦель\nЗадание\nВывод\n\n'
    interactive = subprocess.run([str(BIN)], cwd=temporary, input=answer, capture_output=True, text=True)
    assert interactive.returncode == 0, interactive.stderr
    assert interactive.stdout.count('Введите целое число') == 3
    custom = (Path(temporary) / 'generated/custom/report.html').read_text()
    assert 'Тема &lt;b&gt;' in custom
    # Ошибочный путь не сбрасывает уже введённые данные.
    answer = ('1\n5\n\nТема сохранена\nЦель\nЗадание\nВывод\n'
              'xdg-open assets/uml.svg\nmissing.png\nassets/demo-preview.png\n')
    retry = subprocess.run([str(BIN)], cwd=temporary, input=answer, capture_output=True, text=True)
    assert retry.returncode == 0, retry.stderr
    assert retry.stdout.count('Путь к скриншоту') == 3
    assert retry.stdout.count('Тема работы:') == 2
    assert 'Тема сохранена' in (Path(temporary) / 'generated/custom/report.html').read_text()
    assert (Path(temporary) / 'generated/custom/assets/screenshot.png').is_file()
    # После ошибки можно отказаться от рисунка пустой строкой.
    skip = subprocess.run([str(BIN)], cwd=temporary,
        input='1\n5\nТема\nЦель\nЗадание\nВывод\nmissing.jpg\n\n', capture_output=True, text=True)
    assert skip.returncode == 0, skip.stderr
    # Ctrl+C сохраняет стандартное поведение завершения процесса.
    with subprocess.Popen([str(BIN)], cwd=temporary, stdin=subprocess.PIPE,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True) as running:
        assert 'Генератор' in running.stdout.readline()
        running.send_signal(signal.SIGINT)
        running.communicate(timeout=5)
        assert running.returncode == -signal.SIGINT
print('CLI: три дисциплины, все файлы ресурсов, некорректный ввод и ошибки файлов — OK')
