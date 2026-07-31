#include "SDFrontendAction.h"
#include "SDASTConsumer.h"

std::unique_ptr<clang::ASTConsumer> SDFrontendAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef fileName){
    return std::make_unique<SDASTConsumer>(compiler.getSourceManager());
}
