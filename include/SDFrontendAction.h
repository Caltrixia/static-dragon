#pragma once
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/FrontendActions.h>

class SDFrontendAction : public clang::ASTFrontendAction
{
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef) override;
};
