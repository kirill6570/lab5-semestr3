cmake_minimum_required(VERSION 3.16)
project(Lab5_ReportBuilder LANGUAGES CXX)
add_library(report_core
    src/Report.cpp
    src/HtmlReportBuilder.cpp
    src/Director.cpp
    src/FileUtils.cpp
    src/ConsoleInput.cpp
    src/ReportGenerator.cpp)
target_include_directories(report_core PUBLIC include)
target_compile_features(report_core PUBLIC cxx_std_17)
if(MSVC)
  target_compile_options(report_core PRIVATE /W4 /utf-8)
else()
  target_compile_options(report_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_executable(lab5 main.cpp)
target_link_libraries(lab5 PRIVATE report_core)
include(CTest)
if(BUILD_TESTING)
  add_executable(builder_tests tests/test_builder.cpp)
  target_link_libraries(builder_tests PRIVATE report_core)
  add_test(NAME builder_contract COMMAND builder_tests)
endif()
