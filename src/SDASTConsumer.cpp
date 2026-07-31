#include "SDASTConsumer.h"

SDASTConsumer::SDASTConsumer(
    clang::SourceManager& sourceManager)
    : visitor_(sourceManager)
{
}

void SDASTConsumer::HandleTranslationUnit(clang::ASTContext& context){
    visitor_.TraverseDecl(
    context.getTranslationUnitDecl());
}
