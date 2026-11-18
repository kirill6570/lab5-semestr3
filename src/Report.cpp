#include "Report.h"
#include "FileUtils.h"

using namespace std;
namespace fs = std::filesystem;

void Report::append(const string& part) { html += part; }
const string& Report::getHtml() const { return html; }
void Report::save(const fs::path& path) const { writeFile(path, html); }
