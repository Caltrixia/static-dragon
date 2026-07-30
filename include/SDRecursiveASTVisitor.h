#pragma once


class MyVisitor : public RecursiveASTVisitor<MyVisitor>
{
public:
    bool VisitFunctionDecl(FunctionDecl *FD)
    {
        llvm::outs() <<  FD->getNameAsString() << "\n"
    }
};