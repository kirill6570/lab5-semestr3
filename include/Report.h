#pragma once

#include <filesystem>
#include <string>

// Продукт: хранит собранный HTML и сохраняет его в файл.
class Report {
    std::string html;
public:
    void append(const std::string& part);
    const std::string& getHtml() const;
    void save(const std::filesystem::path& path) const;
};
