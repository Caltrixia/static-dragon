#include "SDASTConsumer.h"
#include "SDRecursiveASTVisitor.h"


void SDASTConsumer::HandleTranslationUnit(clang::ASTContext& context){
    SDRecursiveASTVisitor visitor;
    visitor.TraverseDecl(
    context.getTranslationUnitDecl());
}
