#include "SDRecursiveASTVisitor.h"

bool SDRecursiveASTVisitor::VisitFunctionDecl(clang::FunctionDecl* func){
    if (func->hasBody())
    {
        llvm::outs()
            << "Function: "
            << func->getNameAsString()
            << "\n";
    }

    return true;
}

