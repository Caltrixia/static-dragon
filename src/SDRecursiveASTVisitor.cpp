#include "SDRecursiveASTVisitor.h"
#include <clang/AST/Decl.h>
#include <llvm/Support/raw_ostream.h>
#include "SDFunctionBodyVisitor.h"

SDRecursiveASTVisitor::SDRecursiveASTVisitor(
    clang::SourceManager& sourceManager)
    : sourceManager_(sourceManager)
{
}

bool SDRecursiveASTVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl)
{
    // skip if func is nullptr or does not have body
    if(functionDecl == nullptr || !functionDecl->hasBody()){
        return true;
    }

    clang::Stmt* functionBody = functionDecl->getBody();

    if (functionBody == nullptr) {
        return true;
    }

    clang::SourceLocation startLocation =
    sourceManager_.getExpansionLoc(functionDecl->getBeginLoc());

    clang::SourceLocation endLocation =
    sourceManager_.getExpansionLoc(functionBody->getEndLoc());

    if (startLocation.isInvalid() || endLocation.isInvalid()) {
        return true;
    }

    // ignore system headers
    if (sourceManager_.isInSystemHeader(startLocation)) {
        return true;
    }

    FunctionMetrics metrics;
    metrics.name = functionDecl->getQualifiedNameAsString();
    metrics.file = sourceManager_.getFilename(startLocation).str();
    metrics.startLine = sourceManager_.getSpellingLineNumber(startLocation);
    metrics.endLine = sourceManager_.getSpellingLineNumber(endLocation);

    if (metrics.endLine >= metrics.startLine) {
        metrics.physicalLines = metrics.endLine - metrics.startLine + 1;
    }

    metrics.parameterCount = functionDecl->getNumParams();

    SDFunctionBodyVisitor functionBodyVisitor;
    functionBodyVisitor.TraverseStmt(functionBody);

    metrics.functionBodyMetrics = functionBodyVisitor.metrics();

    functionMetrics_.push_back(std::move(metrics));

    return true;
}

const std::vector<FunctionMetrics>& SDRecursiveASTVisitor::getFunctionMetrics() const
{
    return functionMetrics_;
}

