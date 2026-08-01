#include "SDReportGenerator.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

namespace
{

constexpr int FILE_WIDTH = 42;
constexpr int FUNCTION_WIDTH = 32;
constexpr int LINE_WIDTH = 10;
constexpr int METRIC_WIDTH = 14;

std::size_t countFunctions(
    const std::vector<TranslationUnitMetrics>& allMetrics)
{
    std::size_t totalFunctions = 0;

    for (const TranslationUnitMetrics& unit : allMetrics) {
        totalFunctions += unit.functionMetrics.size();
    }

    return totalFunctions;
}

bool checkOutputStream(
    const std::ofstream& outputFile,
    const fs::path& outputPath,
    const std::string& reportType)
{
    if (outputFile) {
        return true;
    }

    std::cerr
        << "Failed while writing "
        << reportType
        << " report: "
        << outputPath
        << '\n';

    return false;
}

} // namespace

void SDReportGenerator::printToTerminal(
    const std::vector<TranslationUnitMetrics>& allMetrics)
{
    printToStream(allMetrics, std::cout);
}

void SDReportGenerator::printToStream(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    std::ostream& output)
{
    const std::size_t totalFunctions =
        countFunctions(allMetrics);

    output
        << "\n================ Static Dragon Report ================\n"
        << "Total translation units: "
        << allMetrics.size()
        << '\n'
        << "Total functions: "
        << totalFunctions
        << "\n\n";

    for (const TranslationUnitMetrics& unit : allMetrics) {
        output
            << "File: "
            << unit.file
            << '\n'
            << "Functions: "
            << unit.functionMetrics.size()
            << "\n\n";

        output
            << std::left
            << std::setw(FUNCTION_WIDTH) << "Function"
            << std::setw(LINE_WIDTH) << "Lines"
            << std::setw(LINE_WIDTH) << "Params"
            << std::setw(METRIC_WIDTH) << "Statements"
            << std::setw(METRIC_WIDTH) << "Complexity"
            << std::setw(METRIC_WIDTH) << "Max nesting"
            << std::setw(METRIC_WIDTH) << "Returns"
            << '\n';

        output
            << std::string(
                FUNCTION_WIDTH +
                    LINE_WIDTH * 2 +
                    METRIC_WIDTH * 4,
                '-')
            << '\n';

        for (const FunctionMetrics& function :
             unit.functionMetrics) {
            const FunctionBodyMetrics& body =
                function.functionBodyMetrics;

            output
                << std::left
                << std::setw(FUNCTION_WIDTH)
                << function.name
                << std::setw(LINE_WIDTH)
                << function.physicalLines
                << std::setw(LINE_WIDTH)
                << function.parameterCount
                << std::setw(METRIC_WIDTH)
                << body.statementCount
                << std::setw(METRIC_WIDTH)
                << body.cyclomaticComplexity
                << std::setw(METRIC_WIDTH)
                << body.maximumNestingDepth
                << std::setw(METRIC_WIDTH)
                << body.returnCount
                << '\n';
        }

        output << '\n';
    }
}

bool SDReportGenerator::saveTextReport(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    const fs::path& outputPath)
{
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create text report: "
            << outputPath
            << '\n';

        return false;
    }

    printToStream(allMetrics, outputFile);

    return checkOutputStream(
        outputFile,
        outputPath,
        "text");
}

bool SDReportGenerator::saveCSVReport(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    const fs::path& outputPath)
{
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create CSV report: "
            << outputPath
            << '\n';

        return false;
    }

    outputFile
        << "file,"
        << "name,"
        << "start_line,"
        << "end_line,"
        << "physical_lines,"
        << "parameter_count,"
        << "statement_count,"
        << "cyclomatic_complexity,"
        << "maximum_nesting_depth,"
        << "return_count\n";

    for (const TranslationUnitMetrics& unit : allMetrics) {
        for (const FunctionMetrics& function :
             unit.functionMetrics) {
            const FunctionBodyMetrics& body =
                function.functionBodyMetrics;

            outputFile
                << escapeCSV(unit.file) << ','
                << escapeCSV(function.name) << ','
                << function.startLine << ','
                << function.endLine << ','
                << function.physicalLines << ','
                << function.parameterCount << ','
                << body.statementCount << ','
                << body.cyclomaticComplexity << ','
                << body.maximumNestingDepth << ','
                << body.returnCount
                << '\n';
        }
    }

    return checkOutputStream(
        outputFile,
        outputPath,
        "CSV");
}

bool SDReportGenerator::saveJSONReport(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    const fs::path& outputPath)
{
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create JSON report: "
            << outputPath
            << '\n';

        return false;
    }

    outputFile << "{\n";
    outputFile << "  \"tool\": \"Static Dragon\",\n";
    outputFile
        << "  \"translationUnitCount\": "
        << allMetrics.size()
        << ",\n";
    outputFile
        << "  \"functionCount\": "
        << countFunctions(allMetrics)
        << ",\n";
    outputFile << "  \"translationUnits\": [\n";

    for (std::size_t unitIndex = 0;
         unitIndex < allMetrics.size();
         ++unitIndex) {
        const TranslationUnitMetrics& unit =
            allMetrics[unitIndex];

        outputFile << "    {\n";
        outputFile
            << "      \"file\": \""
            << escapeJSON(unit.file)
            << "\",\n";
        outputFile
            << "      \"functionCount\": "
            << unit.functionMetrics.size()
            << ",\n";
        outputFile << "      \"functions\": [\n";

        for (std::size_t functionIndex = 0;
             functionIndex < unit.functionMetrics.size();
             ++functionIndex) {
            const FunctionMetrics& function =
                unit.functionMetrics[functionIndex];

            const FunctionBodyMetrics& body =
                function.functionBodyMetrics;

            outputFile << "        {\n";

            outputFile
                << "          \"name\": \""
                << escapeJSON(function.name)
                << "\",\n";

            outputFile
                << "          \"startLine\": "
                << function.startLine
                << ",\n";

            outputFile
                << "          \"endLine\": "
                << function.endLine
                << ",\n";

            outputFile
                << "          \"physicalLines\": "
                << function.physicalLines
                << ",\n";

            outputFile
                << "          \"parameterCount\": "
                << function.parameterCount
                << ",\n";

            outputFile
                << "          \"bodyMetrics\": {\n";

            outputFile
                << "            \"statementCount\": "
                << body.statementCount
                << ",\n";

            outputFile
                << "            \"cyclomaticComplexity\": "
                << body.cyclomaticComplexity
                << ",\n";

            outputFile
                << "            \"maximumNestingDepth\": "
                << body.maximumNestingDepth
                << ",\n";

            outputFile
                << "            \"returnCount\": "
                << body.returnCount
                << '\n';

            outputFile << "          }\n";
            outputFile << "        }";

            if (functionIndex + 1 <
                unit.functionMetrics.size()) {
                outputFile << ',';
            }

            outputFile << '\n';
        }

        outputFile << "      ]\n";
        outputFile << "    }";

        if (unitIndex + 1 < allMetrics.size()) {
            outputFile << ',';
        }

        outputFile << '\n';
    }

    outputFile << "  ]\n";
    outputFile << "}\n";

    return checkOutputStream(
        outputFile,
        outputPath,
        "JSON");
}

bool SDReportGenerator::saveHTMLReport(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    const fs::path& outputPath)
{
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create HTML report: "
            << outputPath
            << '\n';

        return false;
    }

    outputFile << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, initial-scale=1.0">

    <title>Static Dragon Report</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f4f6f8;
            color: #202124;
            margin: 0;
            padding: 32px;
        }

        .container {
            max-width: 1400px;
            margin: auto;
            background: white;
            border-radius: 10px;
            padding: 28px;
            box-shadow: 0 4px 18px rgba(0, 0, 0, 0.08);
        }

        h1 {
            margin-top: 0;
        }

        h2 {
            margin-top: 36px;
            overflow-wrap: anywhere;
        }

        .summary {
            background: #eef3ff;
            border-left: 4px solid #3f6ad8;
            padding: 14px;
            margin-bottom: 24px;
        }

        .unit-summary {
            color: #5f6368;
            margin-bottom: 12px;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 32px;
        }

        th,
        td {
            border-bottom: 1px solid #dddddd;
            text-align: left;
            padding: 10px;
        }

        th {
            background: #202124;
            color: white;
            position: sticky;
            top: 0;
        }

        tr:hover {
            background: #f1f5ff;
        }

        .warning {
            background: #fff1f0;
        }

        .number {
            text-align: right;
        }
    </style>
</head>

<body>
<div class="container">
    <h1>Static Dragon Report</h1>
)";

    outputFile
        << "    <div class=\"summary\">\n"
        << "        Translation units analyzed: <strong>"
        << allMetrics.size()
        << "</strong><br>\n"
        << "        Total functions analyzed: <strong>"
        << countFunctions(allMetrics)
        << "</strong>\n"
        << "    </div>\n";

    for (const TranslationUnitMetrics& unit : allMetrics) {
        outputFile
            << "    <section>\n"
            << "        <h2>"
            << escapeHTML(unit.file)
            << "</h2>\n"
            << "        <div class=\"unit-summary\">"
            << unit.functionMetrics.size()
            << " function(s)</div>\n";

        outputFile << R"(
        <table>
            <thead>
                <tr>
                    <th>Function</th>
                    <th>Start</th>
                    <th>End</th>
                    <th>Lines</th>
                    <th>Parameters</th>
                    <th>Statements</th>
                    <th>Complexity</th>
                    <th>Max nesting</th>
                    <th>Returns</th>
                </tr>
            </thead>

            <tbody>
)";

        for (const FunctionMetrics& function :
             unit.functionMetrics) {
            const FunctionBodyMetrics& body =
                function.functionBodyMetrics;

            const bool hasHighComplexity =
                body.cyclomaticComplexity > 10;

            outputFile << "                <tr";

            if (hasHighComplexity) {
                outputFile << " class=\"warning\"";
            }

            outputFile << ">\n";

            outputFile
                << "                    <td>"
                << escapeHTML(function.name)
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << function.startLine
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << function.endLine
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << function.physicalLines
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << function.parameterCount
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << body.statementCount
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << body.cyclomaticComplexity
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << body.maximumNestingDepth
                << "</td>\n";

            outputFile
                << "                    <td class=\"number\">"
                << body.returnCount
                << "</td>\n";

            outputFile << "                </tr>\n";
        }

        outputFile << R"(
            </tbody>
        </table>
    </section>
)";
    }

    outputFile << R"(
</div>
</body>
</html>
)";

    return checkOutputStream(
        outputFile,
        outputPath,
        "HTML");
}

std::string SDReportGenerator::escapeJSON(
    const std::string& value)
{
    std::ostringstream escaped;

    for (const char character : value) {
        switch (character) {
        case '"':
            escaped << "\\\"";
            break;

        case '\\':
            escaped << "\\\\";
            break;

        case '\n':
            escaped << "\\n";
            break;

        case '\r':
            escaped << "\\r";
            break;

        case '\t':
            escaped << "\\t";
            break;

        case '\b':
            escaped << "\\b";
            break;

        case '\f':
            escaped << "\\f";
            break;

        default:
            escaped << character;
            break;
        }
    }

    return escaped.str();
}

std::string SDReportGenerator::escapeHTML(
    const std::string& value)
{
    std::ostringstream escaped;

    for (const char character : value) {
        switch (character) {
        case '&':
            escaped << "&amp;";
            break;

        case '<':
            escaped << "&lt;";
            break;

        case '>':
            escaped << "&gt;";
            break;

        case '"':
            escaped << "&quot;";
            break;

        case '\'':
            escaped << "&#39;";
            break;

        default:
            escaped << character;
            break;
        }
    }

    return escaped.str();
}

std::string SDReportGenerator::escapeCSV(
    const std::string& value)
{
    bool requiresQuotes = false;

    for (const char character : value) {
        if (character == ',' ||
            character == '"' ||
            character == '\n' ||
            character == '\r') {
            requiresQuotes = true;
            break;
        }
    }

    if (!requiresQuotes) {
        return value;
    }

    std::string escaped;
    escaped.reserve(value.size() + 2);

    escaped.push_back('"');

    for (const char character : value) {
        if (character == '"') {
            escaped += "\"\"";
        } else {
            escaped.push_back(character);
        }
    }

    escaped.push_back('"');

    return escaped;
}