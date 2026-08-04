#include "SDApplication.h"

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

#include "SDConfiguration.h"
#include "SDFrondendActionFactory.h"
#include "SDReportGenerator.h"

namespace fs = std::filesystem;

namespace
{

llvm::cl::OptionCategory StaticDragonCategory(
    "static-dragon options");

constexpr int SUCCESS = 0;
constexpr int FAILURE = 1;

bool hasArgument(
    int argc,
    const char** argv,
    const std::string& expected)
{
    for (int index = 1; index < argc; ++index) {
        if (std::string(argv[index]) == expected) {
            return true;
        }
    }

    return false;
}

fs::path getConfigPath(
    int argc,
    const char** argv)
{
    for (int index = 1; index < argc; ++index) {
        if (std::string(argv[index]) != "--config") {
            continue;
        }

        if (index + 1 >= argc) {
            return {};
        }

        return argv[index + 1];
    }

    return {};
}

bool containsFormat(
    const std::vector<SDConfiguration::ReportFormat>& formats,
    SDConfiguration::ReportFormat expected)
{
    for (const SDConfiguration::ReportFormat format : formats) {
        if (format == expected) {
            return true;
        }
    }

    return false;
}

bool createReportDirectory(
    const fs::path& reportDirectory)
{
    std::error_code errorCode;

    fs::create_directories(
        reportDirectory,
        errorCode);

    if (errorCode) {
        llvm::errs()
            << "static-dragon: failed to create report directory: "
            << reportDirectory.string()
            << '\n'
            << errorCode.message()
            << '\n';

        return false;
    }

    if (!fs::is_directory(reportDirectory)) {
        llvm::errs()
            << "static-dragon: report path is not a directory: "
            << reportDirectory.string()
            << '\n';

        return false;
    }

    return true;
}

} // namespace

int SDApplication::run(
    int argc,
    const char** argv)
{
    if (hasArgument(argc, argv, "--config")) {
        return runFromConfiguration(
            getConfigPath(argc, argv));
    }

    return runFromCommandLine(argc, argv);
}

int SDApplication::runFromConfiguration(
    const fs::path& configPath)
{
    if (configPath.empty()) {
        llvm::errs()
            << "static-dragon: --config requires a file\n"
            << "Usage:\n"
            << "  static-dragon --config <config-file>\n";

        return FAILURE;
    }

    SDConfiguration configuration;

    if (!configuration.loadFromFile(configPath)) {
        return FAILURE;
    }

    std::string databaseError;

    std::unique_ptr<clang::tooling::CompilationDatabase>
        compilationDatabase =
            clang::tooling::CompilationDatabase::
                loadFromDirectory(
                    configuration.buildDirectory(),
                    databaseError);

    if (!compilationDatabase) {
        llvm::errs()
            << "static-dragon: failed to load "
            << "compilation database from: "
            << configuration.buildDirectory()
            << '\n'
            << databaseError
            << '\n';

        return FAILURE;
    }

    llvm::outs()
        << "Project: "
        << configuration.projectName()
        << '\n';

    return runAnalysis(
        *compilationDatabase,
        configuration.files(),
        configuration.outputDirectory(),
        configuration.formats());
}

int SDApplication::runFromCommandLine(
    int argc,
    const char** argv)
{
    auto optionsParserResult =
        clang::tooling::CommonOptionsParser::create(
            argc,
            argv,
            StaticDragonCategory);

    if (!optionsParserResult) {
        llvm::errs()
            << "static-dragon: failed to parse arguments\n"
            << llvm::toString(
                   optionsParserResult.takeError())
            << '\n';

        return FAILURE;
    }

    clang::tooling::CommonOptionsParser&
        optionsParser =
            optionsParserResult.get();

    const std::vector<std::string>& sourceFiles =
        optionsParser.getSourcePathList();

    if (sourceFiles.empty()) {
        llvm::errs()
            << "static-dragon: no source files provided\n"
            << "Usage:\n"
            << "  static-dragon -p "
            << "<build-directory> "
            << "<source-files...>\n";

        return FAILURE;
    }

    const fs::path defaultReportDirectory =
        fs::path(PROJECT_ROOT) / "reports";

    const std::vector<SDConfiguration::ReportFormat>
        defaultFormats{
            SDConfiguration::ReportFormat::Terminal,
            SDConfiguration::ReportFormat::Text,
            SDConfiguration::ReportFormat::CSV,
            SDConfiguration::ReportFormat::JSON,
            SDConfiguration::ReportFormat::HTML
        };

    return runAnalysis(
        optionsParser.getCompilations(),
        sourceFiles,
        defaultReportDirectory,
        defaultFormats);
}

int SDApplication::runAnalysis(
    clang::tooling::CompilationDatabase& compilationDatabase,
    const std::vector<std::string>& sourceFiles,
    const fs::path& reportDirectory,
    const std::vector<SDConfiguration::ReportFormat>&
        reportFormats)
{
    if (sourceFiles.empty()) {
        llvm::errs()
            << "static-dragon: no source files provided\n";

        return FAILURE;
    }

    llvm::outs()
        << "Static Dragon is analyzing "
        << sourceFiles.size()
        << " translation unit(s).\n";

    clang::tooling::ClangTool tool(
        compilationDatabase,
        sourceFiles);

    SDMetricsCollector collector;
    SDFrondendActionFactory actionFactory(collector);

    const int analysisResult =
        tool.run(&actionFactory);

    if (analysisResult != SUCCESS) {
        llvm::errs()
            << "static-dragon: analysis failed with code "
            << analysisResult
            << '\n';

        return analysisResult;
    }

    llvm::outs()
        << "Static Dragon analysis completed.\n";

    const auto& allMetrics =
        collector.getTranslationUnits();

    if (!generateReports(
            allMetrics,
            reportDirectory,
            reportFormats)) {
        llvm::errs()
            << "static-dragon: one or more reports "
            << "could not be generated\n";

        return FAILURE;
    }

    return SUCCESS;
}

bool SDApplication::generateReports(
    const std::vector<TranslationUnitMetrics>& allMetrics,
    const fs::path& reportDirectory,
    const std::vector<SDConfiguration::ReportFormat>& formats)
{
    if (!createReportDirectory(reportDirectory)) {
        return false;
    }

    bool reportsGenerated = true;

    if (containsFormat(
            formats,
            SDConfiguration::ReportFormat::Terminal)) {
        SDReportGenerator::printToTerminal(allMetrics);
    }

    if (containsFormat(
            formats,
            SDConfiguration::ReportFormat::Text)) {
        reportsGenerated &=
            SDReportGenerator::saveTextReport(
                allMetrics,
                reportDirectory /
                    "static-dragon-report.txt");
    }

    if (containsFormat(
            formats,
            SDConfiguration::ReportFormat::CSV)) {
        reportsGenerated &=
            SDReportGenerator::saveCSVReport(
                allMetrics,
                reportDirectory /
                    "static-dragon-report.csv");
    }

    if (containsFormat(
            formats,
            SDConfiguration::ReportFormat::JSON)) {
        reportsGenerated &=
            SDReportGenerator::saveJSONReport(
                allMetrics,
                reportDirectory /
                    "static-dragon-report.json");
    }

    if (containsFormat(
            formats,
            SDConfiguration::ReportFormat::HTML)) {
        reportsGenerated &=
            SDReportGenerator::saveHTMLReport(
                allMetrics,
                reportDirectory /
                    "static-dragon-report.html");
    }

    return reportsGenerated;
}
