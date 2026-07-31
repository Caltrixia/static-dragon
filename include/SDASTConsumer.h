#pragma once

#include <clang/AST/ASTConsumer.h>


class SDASTConsumer : public clang::ASTConsumer{
public:
    void HandleTranslationUnit(clang::ASTContext& context) override;
};
