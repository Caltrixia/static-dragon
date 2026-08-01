#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <filesystem>

#include "SDMetrics.h"
#include "SDReportGenerator.h"


namespace fs = std::filesystem;

class SDReportGenerator
{
public:
    // In trực tiếp ra terminal hoặc bất kỳ ostream nào.
    static void printToTerminal(
        const std::vector<TranslationUnitMetrics>& allMetrics);

    static void printToStream(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        std::ostream& output);

    // Lưu report.
    static bool saveTextReport(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        const fs::path& outputPath);

    static bool saveCSVReport(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        const fs::path& outputPath);

    static bool saveJSONReport(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        const fs::path& outputPath);

    static bool saveHTMLReport(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        const fs::path& outputPath);

private:
    static std::string escapeJSON(const std::string& value);
    static std::string escapeHTML(const std::string& value);
    static std::string escapeCSV(const std::string& value);
};