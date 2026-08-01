#include "SDASTConsumer.h"
#include "SDReportGenerator.h"
#include <iostream>

SDASTConsumer::SDASTConsumer(
    clang::SourceManager& sourceManager)
    : visitor_(sourceManager)
{
}

void SDASTConsumer::HandleTranslationUnit(clang::ASTContext& context){
    visitor_.TraverseDecl(
    context.getTranslationUnitDecl());
    const std::vector<FunctionMetrics>& metrics = visitor_.getFunctionMetrics();

    //print/save to reports
    bool success = true;

    SDReportGenerator::printToTerminal(metrics);

    success &= SDReportGenerator::saveTextReport(
        metrics,
        "his-dragon-report.txt"
    );

    success &= SDReportGenerator::saveCSVReport(
        metrics,
        "his-dragon-report.csv"
    );

    success &= SDReportGenerator::saveJSONReport(
        metrics,
        "his-dragon-report.json"
    );

    success &= SDReportGenerator::saveHTMLReport(
        metrics,
        "his-dragon-report.html"
    );

    if (!success) {
        std::cerr << "One or more reports could not be generated\n";
    }
}
