#include "SDRecursiveASTVisitor.h"
#include <clang/AST/Decl.h>
#include <llvm/Support/raw_ostream.h>

SDRecursiveASTVisitor::SDRecursiveASTVisitor(
    clang::SourceManager& sourceManager)
    : sourceManager_(sourceManager)
{
}

bool SDRecursiveASTVisitor::VisitFunctionDecl(clang::FunctionDecl* func)
{
    // skip if func is nullptr
    if(func == nullptr){
        return true;
    }

    //only analyze if func has body
    if (!func->hasBody())
    {   
        return true;
    }
    
    const clang::SourceLocation beginLoc = func->getBeginLoc();
    const clang::SourceLocation endLoc = func->getEndLoc();

    //skip functions in headers
    if(sourceManager_.isInSystemHeader(beginLoc))
    {
        return true;
    }

    //get lines
    const unsigned startLine = sourceManager_.getSpellingLineNumber(beginLoc);
    const unsigned endLine = sourceManager_.getSpellingLineNumber(endLoc);

    const unsigned lineCount = (endLine >= startLine) ? endLine - startLine + 1 : 0;
    
    const unsigned paramCount = func->getNumParams();

    const llvm::StringRef fileName = sourceManager_.getFilename(beginLoc);

    llvm::outs() << "Function: "
                 << func->getNameAsString()
                 << '\n';
    
    llvm::outs() << " File: " 
                 << fileName
                 << '\n';

    llvm::outs()
        << "  Has body: yes\n";                 

    llvm::outs()
        << "  Start line: "
        << startLine
        << '\n';

    llvm::outs()
        << "  End line: "
        << endLine
        << '\n';

    llvm::outs()
        << "  Lines: "
        << lineCount
        << "\n\n";

    return true;
}

