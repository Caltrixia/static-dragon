#pragma once
#include <clang/AST/RecursiveASTVisitor.h>

class SDRecursiveASTVisitor : public clang::RecursiveASTVisitor<SDRecursiveASTVisitor>
{
public:
    bool VisitFunctionDecl(clang::FunctionDecl* func);
};
