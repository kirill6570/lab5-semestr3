#include "Director.h"

using namespace std;
namespace fs = std::filesystem;

Report Director::construct(ReportBuilder& builder, const LabData& data) const {
    builder.reset(data);
    builder.buildTitle();
    builder.buildIntroduction();
    builder.buildUml();
    builder.buildListing();
    builder.buildResults();
    builder.buildConclusion();
    return builder.getResult();
}
