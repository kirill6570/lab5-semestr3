#pragma once

#include "ReportBuilder.h"

// Конкретный строитель: создаёт HTML-разделы.
class HtmlReportBuilder : public ReportBuilder {
    Report report;
    LabData data;
    void section(const std::string& title, const std::string& body);
public:
    void reset(const LabData& value) override;
    void buildTitle() override;
    void buildIntroduction() override;
    void buildUml() override;
    void buildListing() override;
    void buildResults() override;
    void buildConclusion() override;
    Report getResult() const override;
};
