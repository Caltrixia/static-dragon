#include "SDFunctionBodyVisitor.h"


bool SDFunctionBodyVisitor::VisitIfStmt(clang::IfStmt*)
{
    ++metrics_.cyclomaticComplexity;

    return true;
}

bool SDFunctionBodyVisitor::VisitForStmt(clang::ForStmt*)
{
    ++metrics_.cyclomaticComplexity;

    return true;
}

bool SDFunctionBodyVisitor::VisitWhileStmt(clang::WhileStmt*)
{
    ++metrics_.cyclomaticComplexity;

    return true;
}

bool SDFunctionBodyVisitor::VisitReturnStmt(
    clang::ReturnStmt*)
{
    ++metrics_.returnCount;

    return true;
}

bool SDFunctionBodyVisitor::VisitDoStmt(clang::DoStmt* stmt)
{
    ++metrics_.cyclomaticComplexity;
    return true;
}

bool SDFunctionBodyVisitor::VisitSwitchStmt(clang::SwitchStmt* stmt)
{
    ++metrics_.cyclomaticComplexity;
    return true;
}

bool SDFunctionBodyVisitor::VisitCaseStmt(clang::CaseStmt* stmt)
{
    ++metrics_.cyclomaticComplexity;
    return true;
}

bool SDFunctionBodyVisitor::VisitDefaultStmt(clang::DefaultStmt* stmt)
{
    return true;
}

bool SDFunctionBodyVisitor::VisitCallExpr(
    clang::CallExpr*)
{
    ++metrics_.functionCallCount;

    return true;
}

bool SDFunctionBodyVisitor::VisitCXXCatchStmt(
    clang::CXXCatchStmt*)
{
    ++metrics_.cyclomaticComplexity;
    return true;
}


void SDFunctionBodyVisitor::enterScope()
{
    ++currentNestingDepth_;

    metrics_.maximumNestingDepth =
        std::max(metrics_.maximumNestingDepth,
                 currentNestingDepth_);
}

void SDFunctionBodyVisitor::leaveScope()
{
    --currentNestingDepth_;
}

bool SDFunctionBodyVisitor::TraverseIfStmt(
    clang::IfStmt* statement)
{
    if (statement == nullptr)
    {
        return true;
    }

    enterScope();

    const bool success =
        clang::RecursiveASTVisitor<
            SDFunctionBodyVisitor>::TraverseIfStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseForStmt(
    clang::ForStmt* statement)
{
    if (statement == nullptr)
    {
        return true;
    }

    enterScope();

    const bool success =
        clang::RecursiveASTVisitor<
            SDFunctionBodyVisitor>::TraverseForStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseWhileStmt(
    clang::WhileStmt* statement)
{
    if (statement == nullptr)
    {
        return true;
    }

    enterScope();

    const bool success =
        clang::RecursiveASTVisitor<
            SDFunctionBodyVisitor>::TraverseWhileStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseDoStmt(
    clang::DoStmt* statement)
{
    if (statement == nullptr)
    {
        return true;
    }

    enterScope();

    const bool success =
        clang::RecursiveASTVisitor<
            SDFunctionBodyVisitor>::TraverseDoStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseSwitchStmt(
    clang::SwitchStmt* statement)
{
    if (statement == nullptr)
    {
        return true;
    }

    enterScope();

    const bool success =
        clang::RecursiveASTVisitor<
            SDFunctionBodyVisitor>::TraverseSwitchStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseCXXTryStmt(
    clang::CXXTryStmt* statement)
{
    if (!statement)
    {
        return true;
    }

    enterScope();

    const bool success =
        Base::TraverseCXXTryStmt(statement);

    leaveScope();

    return success;
}

bool SDFunctionBodyVisitor::TraverseCXXCatchStmt(
    clang::CXXCatchStmt* statement)
{
    if (!statement)
    {
        return true;
    }

    enterScope();

    const bool success =
        Base::TraverseCXXCatchStmt(statement);

    leaveScope();

    return success;
}


const FunctionBodyMetrics& SDFunctionBodyVisitor::metrics() const noexcept
{
    return metrics_;
}
