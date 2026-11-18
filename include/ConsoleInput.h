#pragma once

#include <filesystem>
#include <string>

std::string ask(const std::string& prompt);
std::string askRequired(const std::string& prompt);
std::filesystem::path askScreenshot();
int askNumber(const std::string& prompt, int low, int high);
