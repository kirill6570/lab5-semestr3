#include "ConsoleInput.h"
#include "FileUtils.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

string ask(const string& prompt) {
    cout << prompt;
    string answer;
    if (!getline(cin, answer)) throw runtime_error("Ввод завершён до заполнения данных.");
    return answer;
}

string askRequired(const string& prompt) {
    while (true) {
        string answer = ask(prompt);
        if (answer.find_first_not_of(" \t\r") != string::npos) return answer;
        cout << "Поле не должно быть пустым. Повторите ввод.\n";
    }
}

fs::path askScreenshot() {
    while (true) {
        fs::path path = ask("Путь к скриншоту PNG/JPG (Enter — без рисунка): ");
        try {
            validateScreenshot(path);
            return path;
        } catch (const runtime_error& error) {
            cout << "Ошибка: " << error.what()
                 << "\nПопробуйте ещё раз или нажмите Enter без рисунка. Выход — Ctrl+C.\n";
        }
    }
}

int askNumber(const string& prompt, int low, int high) {
    while (true) {
        string text = ask(prompt);
        istringstream input(text);
        int value; char extra;
        if ((input >> value) && !(input >> extra) && value >= low && value <= high) return value;
        cout << "Введите целое число от " << low << " до " << high << ".\n";
    }
}
