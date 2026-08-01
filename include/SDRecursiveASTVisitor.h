#pragma once
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include "SDMetrics.h"


class SDRecursiveASTVisitor : public clang::RecursiveASTVisitor<SDRecursiveASTVisitor>
{
public:
    explicit SDRecursiveASTVisitor(clang::SourceManager& sourceManager);
    bool VisitFunctionDecl(clang::FunctionDecl* funcDecl);
    const std::vector<FunctionMetrics>& getFunctionMetrics() const;
private:
    clang::SourceManager& sourceManager_;
    std::vector<FunctionMetrics> functionMetrics_;
};
