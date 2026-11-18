#include "HtmlReportBuilder.h"
#include "FileUtils.h"
#include <vector>

using namespace std;
namespace fs = std::filesystem;

void HtmlReportBuilder::section(const string& title, const string& body) {
    report.append("<section><h2>" + title + "</h2>" + body + "</section>\n");
}

void HtmlReportBuilder::reset(const LabData& value) {
    data = value;
    report = Report(); // Повторная сборка не накапливает старые разделы.
}

void HtmlReportBuilder::buildTitle() {
    report.append("<!DOCTYPE html>\n<html lang=\"ru\"><head><meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<title>ЛР №" + to_string(data.number) + " — " + escapeHtml(data.discipline) + "</title>"
        "<style>.identity{position:sticky;top:0;background:#22374d;color:white;padding:8px 16px;font:12px/1.4 sans-serif;z-index:2;text-align:center}@media print{.identity{display:none}}body{margin:0;background:#e9edf2;color:#171717;font:14pt/1.5 'Times New Roman',serif}"
        "main{box-sizing:border-box;max-width:210mm;margin:24px auto;padding:20mm 15mm 20mm 30mm;background:white}"
        "p{text-align:justify;text-indent:1.25cm;margin:0 0 12px}h1{font-size:20pt}h2{font-size:16pt}"
        "header{text-align:center;border-bottom:1px solid #bbb;padding-bottom:20px}header p{text-align:center;text-indent:0}"
        "table{width:100%;border-collapse:collapse;font-size:12pt}th,td{border:1px solid #777;padding:8px;text-align:justify}"
        "pre{white-space:pre-wrap;overflow-wrap:anywhere;background:#f4f5f7;padding:12px;font:9pt/1.3 monospace}"
        "figure{margin:20px 0}figure img{max-width:100%;height:auto}figcaption{text-align:center;font-size:12pt}"
        "section{margin-top:24px}@media print{body{background:white}main{margin:0;max-width:none;padding:0}"
        "header{break-after:page}h2{break-after:avoid}figure,tr{break-inside:avoid}@page{size:A4;margin:20mm 15mm 20mm 30mm}}"
        "@media(max-width:700px){main{margin:0;padding:20px}h1{font-size:18pt}}"
        "</style></head><body><aside class=\"identity\">" + escapeHtml(data.author) + " | " + escapeHtml(data.group) + " | " + escapeHtml(data.date) + "</aside><main><header>"
        "<p>Новосибирский государственный технический университет</p>"
        "<h1>Лабораторная работа №" + to_string(data.number) + "</h1><p>" + escapeHtml(data.discipline) + "</p>"
        "<h2>" + escapeHtml(data.title) + "</h2><p>" + escapeHtml(data.author) + " | " + escapeHtml(data.group) + "</p>"
        "<p>Сформировано: " + escapeHtml(data.date) + "</p></header>\n");
}

void HtmlReportBuilder::buildIntroduction() {
    section("1 Цель и задание", "<p><strong>Цель:</strong> " + escapeHtml(data.goal) + "</p>"
        "<p><strong>Задание:</strong> " + escapeHtml(data.task) + "</p>");
}

void HtmlReportBuilder::buildUml() {
    section("2 Диаграмма классов UML", "<figure><img src=\"assets/uml.svg\" alt=\"UML классов генератора отчётов\">"
        "<figcaption>Рисунок 1 — Структура паттерна Строитель</figcaption></figure>");
}

void HtmlReportBuilder::buildListing() {
    section("3 Листинг программы", "<details><summary>Исходный код программы</summary><pre><code>" + escapeHtml(data.source) + "</code></pre></details>");
}

void HtmlReportBuilder::buildResults() {
    string body = "<p>Документ собран из шести последовательных шагов. Таблица отражает структуру полученного отчёта.</p>"
        "<table><caption>Таблица 1 — Элементы отчёта</caption><thead><tr><th>Элемент</th><th>Содержание</th></tr></thead><tbody>";
    const vector<vector<string>> rows = {
        {"Титульный лист", "Дисциплина, тема, автор, группа и дата"},
        {"Цель и задание", "Исходные данные выбранной лабораторной работы"},
        {"UML", "Связи классов Builder, Director и Report"},
        {"Листинг", "Исходный код из файлов main.cpp, include и src"},
        {"Результаты", "Таблица, диаграмма и рисунок при его выборе"},
        {"Вывод", "Итог, заданный пользователем"}
    };
    for (const auto& row : rows)
        body += "<tr><td>" + escapeHtml(row[0]) + "</td><td>" + escapeHtml(row[1]) + "</td></tr>";
    body += "</tbody></table><figure><img src=\"assets/structure.svg\" alt=\"Состав отчёта\">"
        "<figcaption>Рисунок 2 — Количество элементов в структуре отчёта</figcaption></figure>";
    if (!data.screenshot.empty())
        body += "<figure><img src=\"assets/screenshot" + data.screenshot.extension().string() +
            "\" alt=\"Снимок результата работы\"><figcaption>Рисунок 3 — Снимок результата работы</figcaption></figure>";
    section("4 Результаты работы", body);
}

void HtmlReportBuilder::buildConclusion() {
    section("5 Вывод", "<p>" + escapeHtml(data.conclusion) + "</p>");
    report.append("</main></body></html>\n");
}

Report HtmlReportBuilder::getResult() const { return report; }
