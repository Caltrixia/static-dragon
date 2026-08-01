#pragma once
#include <clang/Basic/SourceManager.h>
#include <clang/AST/ASTConsumer.h>
#include "SDRecursiveASTVisitor.h"
#include "SDMetricsCollector.h"



class SDASTConsumer : public clang::ASTConsumer{
public:
    explicit SDASTConsumer(clang::SourceManager& sourceManager, SDMetricsCollector& collector);
    void HandleTranslationUnit(clang::ASTContext& context) override;
private:
    SDMetricsCollector& collector_;
    SDRecursiveASTVisitor visitor_;
};
