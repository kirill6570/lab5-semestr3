#pragma once

#include "ReportBuilder.h"

// Директор задаёт последовательность шагов через интерфейс строителя.
class Director {
public:
    Report construct(ReportBuilder& builder, const LabData& data) const;
};
