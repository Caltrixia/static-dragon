#pragma once
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>


class SDRecursiveASTVisitor : public clang::RecursiveASTVisitor<SDRecursiveASTVisitor>
{
public:
    explicit SDRecursiveASTVisitor(clang::SourceManager& sourceManager);
    bool VisitFunctionDecl(clang::FunctionDecl* func);
private:
    clang::SourceManager& sourceManager_;
};
