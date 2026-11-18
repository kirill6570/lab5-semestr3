#include "FileUtils.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string escapeHtml(const string& text) {
    string result;
    for (char c : text) {
        switch (c) {
        case '&': result += "&amp;"; break;
        case '<': result += "&lt;"; break;
        case '>': result += "&gt;"; break;
        case '"': result += "&quot;"; break;
        case '\'': result += "&#39;"; break;
        default: result += c;
        }
    }
    return result;
}

string readFile(const fs::path& path) {
    ifstream file(path, ios::binary);
    if (!file) throw runtime_error("Не удалось открыть файл: " + path.string());
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    if (file.bad()) throw runtime_error("Ошибка чтения: " + path.string());
    return text;
}

void writeFile(const fs::path& path, const string& text) {
    ofstream file(path, ios::binary);
    if (!file) throw runtime_error("Не удалось создать файл: " + path.string());
    file << text;
    file.close();
    if (!file) throw runtime_error("Ошибка записи: " + path.string());
}

string currentDate() {
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm value = *localtime(&now);
    ostringstream out;
    out << put_time(&value, "%d.%m.%Y %H:%M:%S");
    return out.str();
}
void validateScreenshot(const fs::path& path) {
    if (path.empty()) return;
    const string ext = path.extension().string();
    if (ext != ".png" && ext != ".jpg" && ext != ".jpeg")
        throw runtime_error("Для скриншота нужен файл PNG или JPG. Введите путь, а не команду.");
    error_code error;
    if (!fs::is_regular_file(path, error))
        throw runtime_error("Файл не найден или недоступен: " + path.string());
    ifstream image(path, ios::binary);
    if (!image) throw runtime_error("Не удалось открыть изображение: " + path.string());
}

// Каждый запуск получает свободную папку; старые отчёты сохраняются.
fs::path newReportDirectory(const fs::path& root, const string& prefix) {
    fs::create_directories(root);
    for (int number = 1; ; ++number) {
        ostringstream name;
        name << prefix << '-' << setfill('0') << setw(3) << number;
        fs::path candidate = root / name.str();
        if (fs::create_directory(candidate)) return candidate;
    }
}

string fileUrl(const fs::path& path) {
    const string bytes = fs::absolute(path).generic_string();
    const char* hex = "0123456789ABCDEF";
    string url = "file://";
    for (unsigned char c : bytes) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '/' || c == '-' || c == '_' || c == '.' || c == '~')
            url += static_cast<char>(c);
        else { url += '%'; url += hex[c >> 4]; url += hex[c & 15]; }
    }
    return url;
}

// Листинг формируется из физических исходников многофайлового проекта.
string readProjectSource() {
    string result = "// ===== main.cpp =====\n" + readFile("main.cpp");
    for (const fs::path& directory : {fs::path("include"), fs::path("src")}) {
        vector<fs::path> files;
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && (entry.path().extension() == ".h" || entry.path().extension() == ".cpp"))
                files.push_back(entry.path());
        }
        sort(files.begin(), files.end());
        for (const auto& path : files)
            result += "\n// ===== " + path.generic_string() + " =====\n" + readFile(path);
    }
    return result;
}
