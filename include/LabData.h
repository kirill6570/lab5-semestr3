#pragma once

#include <filesystem>
#include <string>

// Исходные данные одного отчёта.
struct LabData {
    std::string discipline, author, group, title, goal, task, conclusion, date;
    int number = 5;
    std::string source;
    std::filesystem::path screenshot;
};
