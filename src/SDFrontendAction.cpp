#include "SDFrontendAction.h"
#include "SDASTConsumer.h"

SDFrontendAction::SDFrontendAction(SDMetricsCollector& collector) : collector_(collector){}

std::unique_ptr<clang::ASTConsumer> SDFrontendAction::CreateASTConsumer(
    clang::CompilerInstance& compiler,
    llvm::StringRef inputFile) 
{
    return std::make_unique<SDASTConsumer>(
        compiler.getSourceManager(),
        collector_
    );
}