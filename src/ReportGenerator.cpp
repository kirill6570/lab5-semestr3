#include "ReportGenerator.h"
#include "FileUtils.h"
#include "HtmlReportBuilder.h"
#include "Director.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string makeChart(bool hasScreenshot) {
    const vector<pair<string, int>> values = {
        {"Разделы сборки", 6}, {"Таблицы", 1}, {"SVG-диаграммы", 2}, {"Скриншоты", hasScreenshot ? 1 : 0}
    };
    string svg = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"760\" height=\"240\" viewBox=\"0 0 760 240\">"
        "<rect width=\"760\" height=\"240\" fill=\"white\"/><g font-family=\"sans-serif\" font-size=\"17\">";
    int y = 28;
    for (const auto& value : values) {
        svg += "<text x=\"12\" y=\"" + to_string(y + 21) + "\">" + value.first + "</text>"
            "<rect x=\"220\" y=\"" + to_string(y) + "\" width=\"" + to_string(value.second * 65) +
            "\" height=\"30\" fill=\"#375e8d\"/><text x=\"" + to_string(230 + value.second * 65) +
            "\" y=\"" + to_string(y + 21) + "\">" + to_string(value.second) + "</text>";
        y += 52;
    }
    return svg + "</g></svg>";
}
void generate(LabData data, const fs::path& output, const fs::path& assets) {
    validateScreenshot(data.screenshot);
    fs::create_directories(output / "assets");
    writeFile(output / "assets/uml.svg", readFile(assets / "uml.svg"));
    if (!data.screenshot.empty()) {
        string ext = data.screenshot.extension().string();
        fs::path target = output / ("assets/screenshot" + ext);
        if (fs::weakly_canonical(data.screenshot) != fs::weakly_canonical(target))
            fs::copy_file(data.screenshot, target, fs::copy_options::overwrite_existing);
    }
    writeFile(output / "assets/structure.svg", makeChart(!data.screenshot.empty()));
    unique_ptr<ReportBuilder> builder = make_unique<HtmlReportBuilder>();
    Director director;
    Report report = director.construct(*builder, data);
    report.save(output / "report.html");
    cout << "Создан отчёт: " << fs::absolute(output / "report.html") << '\n';
    cout << "Ссылка: " << fileUrl(output / "report.html") << '\n';
}
