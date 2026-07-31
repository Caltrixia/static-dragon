#include <clang/Tooling/Tooling.h>
#include "SDASTConsumer.h"
#include "SDFrontendAction.h"
#include "SDRecursiveASTVisitor.h"

int main()
{
    const char* code = R"(
        void foo() {}

        int add(int a, int b)
        {
            return a + b;
        }
    )";

    clang::tooling::runToolOnCode(
        std::make_unique<SDFrontendAction>(),
        code);
}
