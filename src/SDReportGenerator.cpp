#include "SDReportGenerator.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <filesystem>

namespace
{
constexpr int FUNCTION_WIDTH = 32;
constexpr int LINE_WIDTH = 10;
constexpr int METRIC_WIDTH = 14;
}

namespace fs = std::filesystem;


void SDReportGenerator::printToTerminal(
    const std::vector<FunctionMetrics>& metrics)
{
    printToStream(metrics, std::cout);
}

void SDReportGenerator::printToStream(
    const std::vector<FunctionMetrics>& metrics,
    std::ostream& output)
{
    output << "\n================ HIS Dragon Report ================\n";
    output << "Total functions: " << metrics.size() << "\n\n";

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

    output << std::string(
        FUNCTION_WIDTH +
        LINE_WIDTH * 2 +
        METRIC_WIDTH * 4,
        '-'
    ) << '\n';

    for (const FunctionMetrics& function : metrics) {
        const FunctionBodyMetrics& body = function.functionBodyMetrics;

        output
            << std::left
            << std::setw(FUNCTION_WIDTH) << function.name
            << std::setw(LINE_WIDTH) << function.physicalLines
            << std::setw(LINE_WIDTH) << function.parameterCount
            << std::setw(METRIC_WIDTH) << body.statementCount
            << std::setw(METRIC_WIDTH) << body.cyclomaticComplexity
            << std::setw(METRIC_WIDTH) << body.maximumNestingDepth
            << std::setw(METRIC_WIDTH) << body.returnCount
            << '\n';
    }

    output << '\n';
}

bool SDReportGenerator::saveTextReport(
    const std::vector<FunctionMetrics>& metrics,
    const std::string& filename)
{
    fs ::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    if(!fs::exists(reportDir)){
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return false;        
    }

    const fs::path outputPath = reportDir / filename;
    
    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create text report: "
            << outputPath
            << '\n';

        return false;
    }

    printToStream(metrics, outputFile);

    return true;
}

bool SDReportGenerator::saveCSVReport(
    const std::vector<FunctionMetrics>& metrics,
    const std::string& filename)
{
    fs ::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    if(!fs::exists(reportDir)){
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return false;       
    }

    const fs::path outputPath = reportDir / filename;

    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create CSV report: "
            << outputPath
            << '\n';

        return false;
    }

    outputFile
        << "name,"
        << "file,"
        << "start_line,"
        << "end_line,"
        << "physical_lines,"
        << "parameter_count,"
        << "statement_count,"
        << "cyclomatic_complexity,"
        << "maximum_nesting_depth,"
        << "return_count\n";

    for (const FunctionMetrics& function : metrics) {
        const FunctionBodyMetrics& body = function.functionBodyMetrics;

        outputFile
            << escapeCSV(function.name) << ','
            << escapeCSV(function.file) << ','
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

    return true;
}

bool SDReportGenerator::saveJSONReport(
    const std::vector<FunctionMetrics>& metrics,
    const std::string& filename)
{
    fs ::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    if(!fs::exists(reportDir)){
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return false;        
    }

    const fs::path outputPath = reportDir / filename;

    std::ofstream outputFile(outputPath);

    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create JSON report: "
            << outputPath
            << '\n';

        return false;
    }

    outputFile << "{\n";
    outputFile << "  \"tool\": \"HIS Dragon\",\n";
    outputFile << "  \"functionCount\": " << metrics.size() << ",\n";
    outputFile << "  \"functions\": [\n";

    for (std::size_t index = 0; index < metrics.size(); ++index) {
        const FunctionMetrics& function = metrics[index];
        const FunctionBodyMetrics& body = function.functionBodyMetrics;

        outputFile << "    {\n";
        outputFile
            << "      \"name\": \""
            << escapeJSON(function.name)
            << "\",\n";

        outputFile
            << "      \"file\": \""
            << escapeJSON(function.file)
            << "\",\n";

        outputFile
            << "      \"startLine\": "
            << function.startLine
            << ",\n";

        outputFile
            << "      \"endLine\": "
            << function.endLine
            << ",\n";

        outputFile
            << "      \"physicalLines\": "
            << function.physicalLines
            << ",\n";

        outputFile
            << "      \"parameterCount\": "
            << function.parameterCount
            << ",\n";

        outputFile << "      \"bodyMetrics\": {\n";

        outputFile
            << "        \"statementCount\": "
            << body.statementCount
            << ",\n";

        outputFile
            << "        \"cyclomaticComplexity\": "
            << body.cyclomaticComplexity
            << ",\n";

        outputFile
            << "        \"maximumNestingDepth\": "
            << body.maximumNestingDepth
            << ",\n";

        outputFile
            << "        \"returnCount\": "
            << body.returnCount
            << '\n';

        outputFile << "      }\n";
        outputFile << "    }";

        if (index + 1 < metrics.size()) {
            outputFile << ',';
        }

        outputFile << '\n';
    }

    outputFile << "  ]\n";
    outputFile << "}\n";

    return true;
}

bool SDReportGenerator::saveHTMLReport(
    const std::vector<FunctionMetrics>& metrics,
    const std::string& filename)
{
    fs ::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    if(!fs::exists(reportDir)){
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return false;        
    }

    const fs::path outputPath = reportDir / filename;

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
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HIS Dragon Report</title>

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

        .summary {
            background: #eef3ff;
            border-left: 4px solid #3f6ad8;
            padding: 14px;
            margin-bottom: 24px;
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        th, td {
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

        .path {
            max-width: 300px;
            overflow-wrap: anywhere;
        }

        .number {
            text-align: right;
        }
    </style>
</head>

<body>
<div class="container">
    <h1>HIS Dragon Report</h1>
)";

    outputFile
        << "    <div class=\"summary\">"
        << "Total functions analyzed: <strong>"
        << metrics.size()
        << "</strong></div>\n";

    outputFile << R"(
    <table>
        <thead>
            <tr>
                <th>Function</th>
                <th>File</th>
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

    for (const FunctionMetrics& function : metrics) {
        const FunctionBodyMetrics& body = function.functionBodyMetrics;

        
        const bool hasHighComplexity =
            body.cyclomaticComplexity > 10;

        outputFile
            << "            <tr";

        if (hasHighComplexity) {
            outputFile << " class=\"warning\"";
        }

        outputFile << ">\n";

        outputFile
            << "                <td>"
            << escapeHTML(function.name)
            << "</td>\n";

        outputFile
            << "                <td class=\"path\">"
            << escapeHTML(function.file)
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << function.startLine
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << function.endLine
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << function.physicalLines
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << function.parameterCount
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << body.statementCount
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << body.cyclomaticComplexity
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << body.maximumNestingDepth
            << "</td>\n";

        outputFile
            << "                <td class=\"number\">"
            << body.returnCount
            << "</td>\n";

        outputFile << "            </tr>\n";
    }

    outputFile << R"(
        </tbody>
    </table>
</div>
</body>
</html>
)";

    return true;
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