#pragma once
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/FrontendActions.h>
#include "SDMetricsCollector.h"

class SDFrontendAction : public clang::ASTFrontendAction
{
public:
    explicit SDFrontendAction(SDMetricsCollector& collector);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inputFile) override;

private:
    SDMetricsCollector& collector_;
};
