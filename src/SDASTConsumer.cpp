#include "SDASTConsumer.h"
#include "SDReportGenerator.h"
#include <iostream>

SDASTConsumer::SDASTConsumer(
    clang::SourceManager& sourceManager, SDMetricsCollector& collector)
    : visitor_(sourceManager), collector_(collector)
{
}

void SDASTConsumer::HandleTranslationUnit(clang::ASTContext& context){
    visitor_.TraverseDecl(
    context.getTranslationUnitDecl());

    const clang::SourceManager& sourceManager =
    context.getSourceManager();

    const clang::FileID mainFileID =
        sourceManager.getMainFileID();

    const auto fileEntryRef =
        sourceManager.getFileEntryRefForID(mainFileID);

    if (!fileEntryRef) {
        std::cerr
            << "Failed to determine main source file\n";
        return;
    }

    TranslationUnitMetrics translationUnitMetrics;

    translationUnitMetrics.file =
        fileEntryRef->getName().str();

    translationUnitMetrics.functionMetrics =
        visitor_.getFunctionMetrics();

    collector_.addTranslationUnit(
        std::move(translationUnitMetrics)
    );
}
