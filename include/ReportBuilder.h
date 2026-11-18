#pragma once

#include "LabData.h"
#include "Report.h"

class ReportBuilder {
public:
    virtual ~ReportBuilder() = default;
    virtual void reset(const LabData& data) = 0;
    virtual void buildTitle() = 0;
    virtual void buildIntroduction() = 0;
    virtual void buildUml() = 0;
    virtual void buildListing() = 0;
    virtual void buildResults() = 0;
    virtual void buildConclusion() = 0;
    virtual Report getResult() const = 0;
};
