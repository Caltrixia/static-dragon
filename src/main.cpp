#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>
#include <vector>
#include <filesystem>

#include "SDFrondendActionFactory.h"
#include "SDFrontendAction.h"
#include "SDReportGenerator.h"

//tool options
namespace
{

llvm::cl::OptionCategory StaticDragonCategory(
    "static-dragon options");

} // namespace

namespace fs = std::filesystem;

int main(int argc, const char** argv)
{
    auto optionsParserResult =
        clang::tooling::CommonOptionsParser::create(
            argc,
            argv,
            StaticDragonCategory);

    if (!optionsParserResult)
    {
        llvm::errs()
            << "static-dragon: failed to parse arguments\n"
            << llvm::toString(optionsParserResult.takeError())
            << '\n';

        return 1;
    }

    clang::tooling::CommonOptionsParser& optionsParser =
        optionsParserResult.get();

    const std::vector<std::string>& sourceFiles =
        optionsParser.getSourcePathList();

    if (sourceFiles.empty())
    {
        llvm::errs()
            << "static-dragon: no source files provided\n"
            << "Usage:\n"
            << "  static-dragon -p <build-directory> <source-files...>\n";

        return 1;
    }

    llvm::outs()
        << "Static Dragon is analyzing "
        << sourceFiles.size()
        << " translation unit(s).\n";

    clang::tooling::ClangTool tool(
        optionsParser.getCompilations(),
        sourceFiles);

    SDMetricsCollector collector;
    SDFrondendActionFactory actionFactory(collector);

    const int result = tool.run(&actionFactory);

    if (result != 0)
    {
        llvm::errs()
            << "static-dragon: analysis failed with code "
            << result
            << '\n';

        return result;
    }

    llvm::outs() << "Static Dragon analysis completed.\n";

    const auto& allMetrics =
    collector.getTranslationUnits();

    //save/print results

    fs::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    
    if(!fs::exists(reportDir)){
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return 1;        
    }


    SDReportGenerator::printToTerminal(allMetrics);

    bool reportsGenerated = true;

    const std::string txtFileName = "his-dragon-report.txt";
    const fs::path outputTxtPath = reportDir / txtFileName;
    reportsGenerated &=
    SDReportGenerator::saveTextReport(
        allMetrics,
        outputTxtPath);

    const std::string csvFileName = "his-dragon-report.csv";
    const fs::path outputCsvPath = reportDir / csvFileName;
    reportsGenerated &=
    SDReportGenerator::saveCSVReport(
        allMetrics,
        outputCsvPath);

    const std::string jsonFileName = "his-dragon-report.json";
    const fs::path outputJsonPath = reportDir / jsonFileName;
    reportsGenerated &=
    SDReportGenerator::saveJSONReport(
        allMetrics,
        outputJsonPath);

    const std::string htmlFileName = "his-dragon-report.html";
    const fs::path outputHtmlPath = reportDir / htmlFileName;
    reportsGenerated &=
    SDReportGenerator::saveHTMLReport(
        allMetrics,
        outputHtmlPath);

    if (!reportsGenerated)
    {
        llvm::errs()
            << "One or more reports could not be generated\n";
        return 1;
    }

    return 0;
}
