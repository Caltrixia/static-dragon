#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include "SDMetrics.h"

class SDFunctionBodyVisitor : public clang::RecursiveASTVisitor<SDFunctionBodyVisitor>
{
public:
    using Base =
        clang::RecursiveASTVisitor<SDFunctionBodyVisitor>;
    bool VisitIfStmt(clang::IfStmt* stmt);
    bool VisitForStmt(clang::ForStmt* stmt);
    bool VisitWhileStmt(clang::WhileStmt* stmt);
    bool VisitDoStmt(clang::DoStmt* stmt);
    bool VisitSwitchStmt(clang::SwitchStmt* stmt);
    bool VisitCaseStmt(clang::CaseStmt* stmt);
    bool VisitDefaultStmt(clang::DefaultStmt* stmt);
    bool VisitReturnStmt(clang::ReturnStmt* stmt);
    bool VisitCallExpr(clang::CallExpr* expr);
    bool VisitCXXCatchStmt(clang::CXXCatchStmt*);

    bool TraverseIfStmt(clang::IfStmt* statement);
    bool TraverseForStmt(clang::ForStmt* statement);
    bool TraverseWhileStmt(clang::WhileStmt* statement);
    bool TraverseDoStmt(clang::DoStmt* statement);
    bool TraverseSwitchStmt(clang::SwitchStmt* statement);
    bool TraverseCXXTryStmt(clang::CXXTryStmt* statement);
    bool TraverseCXXCatchStmt(clang::CXXCatchStmt* statement);

    //get all function body metrics
    const FunctionBodyMetrics& metrics() const noexcept;

private:
    void enterScope();
    void leaveScope();
private:
    FunctionBodyMetrics metrics_;
    unsigned currentNestingDepth_{0};
};