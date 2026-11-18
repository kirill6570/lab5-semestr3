#include "HtmlReportBuilder.h"
#include "Director.h"
#include "ReportGenerator.h"
#include <iostream>
using namespace std;
#include <cassert>

int main() {
    LabData data;
    data.discipline = "Программирование";
    data.title = "<script>&\"'";
    data.source = "if (a < b && b > 0) {}";
    HtmlReportBuilder builder;
    Director director;
    Report first = director.construct(builder, data);
    const string html = first.getHtml();
    assert(html.find("&lt;script&gt;&amp;&quot;&#39;") != string::npos);
    assert(html.find("if (a &lt; b &amp;&amp; b &gt; 0) {}") != string::npos);
    assert(html.find("assets/screenshot") == string::npos);
    assert(html.find("<!DOCTYPE html>") == 0);
    assert(html.find("</html>") != string::npos);
    data.title = "Новый документ";
    data.screenshot = "example.png";
    Report second = director.construct(builder, data);
    assert(second.getHtml().find("&lt;script&gt;") == string::npos);
    assert(second.getHtml().find("assets/screenshot.png") != string::npos);
    assert(first.getHtml() == html); // Полученный продукт независим от строителя.
    assert(makeChart(false).find(">0</text>") != string::npos);
    assert(makeChart(true).find("Скриншоты") != string::npos);
    cout << "Builder: экранирование, повторная сборка, независимость продукта — OK\n";
}
