#pragma once

#include <filesystem>
#include <string>

std::string escapeHtml(const std::string& text);
std::string readFile(const std::filesystem::path& path);
void writeFile(const std::filesystem::path& path, const std::string& text);
std::string currentDate();
void validateScreenshot(const std::filesystem::path& path);
std::filesystem::path newReportDirectory(const std::filesystem::path& root, const std::string& prefix);
std::string fileUrl(const std::filesystem::path& path);
std::string readProjectSource();
