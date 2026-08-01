#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "SDMetrics.h"

class SDReportGenerator
{
public:
    // In trực tiếp ra terminal hoặc bất kỳ ostream nào.
    static void printToTerminal(
        const std::vector<FunctionMetrics>& metrics);

    static void printToStream(
        const std::vector<FunctionMetrics>& metrics,
        std::ostream& output);

    // Lưu report.
    static bool saveTextReport(
        const std::vector<FunctionMetrics>& metrics,
        const std::string& outputPath);

    static bool saveCSVReport(
        const std::vector<FunctionMetrics>& metrics,
        const std::string& outputPath);

    static bool saveJSONReport(
        const std::vector<FunctionMetrics>& metrics,
        const std::string& outputPath);

    static bool saveHTMLReport(
        const std::vector<FunctionMetrics>& metrics,
        const std::string& outputPath);

private:
    static std::string escapeJSON(const std::string& value);
    static std::string escapeHTML(const std::string& value);
    static std::string escapeCSV(const std::string& value);
};