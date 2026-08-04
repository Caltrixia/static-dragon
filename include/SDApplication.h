#pragma once

#include <clang/Tooling/CompilationDatabase.h>
#include <filesystem>
#include <string>
#include <vector>

#include "SDConfiguration.h"
#include "SDMetrics.h"

class SDApplication {
public:
    // Entry point for the application
    int run(int argc, const char** argv);

private:
    // Run using a configuration file
    int runFromConfiguration(const std::filesystem::path& configPath);

    // Run using command-line arguments
    int runFromCommandLine(int argc, const char** argv);

    // Perform analysis on source files
    int runAnalysis(
        clang::tooling::CompilationDatabase& compilationDatabase,
        const std::vector<std::string>& sourceFiles,
        const std::filesystem::path& reportDirectory,
        const std::vector<SDConfiguration::ReportFormat>& reportFormats
    );

    // Generate reports from collected metrics
    bool generateReports(
        const std::vector<TranslationUnitMetrics>& allMetrics,
        const std::filesystem::path& reportDirectory,
        const std::vector<SDConfiguration::ReportFormat>& formats
    );
};
