#pragma once

#include "LabData.h"
#include <filesystem>
#include <string>

std::string makeChart(bool hasScreenshot);
void generate(LabData data, const std::filesystem::path& output, const std::filesystem::path& assets);
