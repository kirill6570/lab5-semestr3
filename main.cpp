#include "ConsoleInput.h"
#include "FileUtils.h"
#include "ReportGenerator.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

const vector<string> disciplines = {
    "Программирование", "Информатика", "Технологии и методы программирования"
};
int main(int argc, char* argv[]) {
    try {
        cout << "=== Генератор HTML-отчётов | ЛР №5 | Вариант 10 ===\n";
        const bool demo = argc >= 2 && string(argv[1]) == "--demo";
        if (argc > 3 || (argc >= 2 && !demo)) {
            cerr << "Запуск: lab5 [--demo [каталог результатов]]\n";
            return 1;
        }
        fs::path output = argc == 3 ? fs::path(argv[2]) : fs::path("generated");
        LabData data;
        data.author = "Короткевич Кирилл Олегович";
        data.group = "АБ-522";
        data.date = currentDate();
        data.source = readProjectSource();
        if (demo) {
            const vector<vector<string>> examples = {
                {"Генерация HTML-отчётов. Паттерн Строитель",
                 "Изучить наследование и полиморфизм при пошаговой сборке документа.",
                 "Разработать классы продукта, абстрактного строителя, HTML-строителя и Директора. Сформировать отчёт с таблицей и изображениями.",
                 "В примере показана сборка HTML-документа: Директор задаёт последовательность, а конкретный строитель создаёт разделы."},
                {"Представление и хранение информации",
                 "Рассмотреть текстовое, табличное и графическое представление информации.",
                 "Сформировать учебный документ по информатике. Представить структуру документа таблицей, добавить SVG-диаграммы и поясняющий рисунок.",
                 "В примере информация представлена текстом, таблицей и графикой. Документ и связанные ресурсы сохранены отдельными файлами."},
                {"Разделение ответственности и повторное использование компонентов",
                 "Рассмотреть организацию программы из компонентов с отдельными обязанностями.",
                 "Описать взаимодействие данных, строителя, Директора и продукта. Продемонстрировать повторное использование алгоритма сборки для разных дисциплин.",
                 "В примере общий алгоритм использован с другим набором данных. Разделение ответственности позволяет изменять содержание без изменения порядка сборки."}
            };
            const fs::path batch = newReportDirectory(output, "demo");
            if (fs::exists("assets/demo-preview.png")) data.screenshot = "assets/demo-preview.png";
            for (size_t i = 0; i < disciplines.size(); ++i) {
                data.discipline = disciplines[i];
                data.title = examples[i][0];
                data.goal = examples[i][1];
                data.task = examples[i][2];
                data.conclusion = examples[i][3];
                generate(data, batch / ("discipline-" + to_string(i + 1)), "assets");
            }
        } else {
            for (size_t i = 0; i < disciplines.size(); ++i) cout << i + 1 << ". " << disciplines[i] << '\n';
            const int discipline = askNumber("Дисциплина (1–3): ", 1, 3);
            data.discipline = disciplines[discipline - 1];
            data.number = askNumber("Номер лабораторной работы (1–99): ", 1, 99);
            data.title = askRequired("Тема работы: ");
            data.goal = askRequired("Цель работы: ");
            data.task = askRequired("Задание: ");
            data.conclusion = askRequired("Вывод: ");
            data.screenshot = askScreenshot();
            generate(data, newReportDirectory(output, "discipline-" + to_string(discipline) + "-report"), "assets");
        }
        return 0;
    } catch (const exception& error) {
        cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
