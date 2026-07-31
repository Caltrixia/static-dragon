#pragma once
#include <clang/Basic/SourceManager.h>
#include <clang/AST/ASTConsumer.h>
#include "SDRecursiveASTVisitor.h"



class SDASTConsumer : public clang::ASTConsumer{
public:
    explicit SDASTConsumer(clang::SourceManager& sourceManager);
    void HandleTranslationUnit(clang::ASTContext& context) override;
private:
    SDRecursiveASTVisitor visitor_;
};
