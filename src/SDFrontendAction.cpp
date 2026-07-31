#include "SDFrontendAction.h"
#include "SDASTConsumer.h"

std::unique_ptr<clang::ASTConsumer> SDFrontendAction::CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef){
    return std::make_unique<SDASTConsumer>();
}
