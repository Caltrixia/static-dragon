#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>
#include <vector>

#include "SDFrontendAction.h"

//tool options
namespace
{

llvm::cl::OptionCategory StaticDragonCategory(
    "static-dragon options");

} // namespace

int main(int argc, const char** argv)
{
    auto optionsParserResult =
        clang::tooling::CommonOptionsParser::create(
            argc,
            argv,
            StaticDragonCategory);

    if (!optionsParserResult)
    {
        llvm::errs()
            << "static-dragon: failed to parse arguments\n"
            << llvm::toString(optionsParserResult.takeError())
            << '\n';

        return 1;
    }

    clang::tooling::CommonOptionsParser& optionsParser =
        optionsParserResult.get();

    const std::vector<std::string>& sourceFiles =
        optionsParser.getSourcePathList();

    if (sourceFiles.empty())
    {
        llvm::errs()
            << "static-dragon: no source files provided\n"
            << "Usage:\n"
            << "  static-dragon -p <build-directory> <source-files...>\n";

        return 1;
    }

    llvm::outs()
        << "Static Dragon is analyzing "
        << sourceFiles.size()
        << " translation unit(s).\n";

    clang::tooling::ClangTool tool(
        optionsParser.getCompilations(),
        sourceFiles);

    std::unique_ptr<clang::tooling::FrontendActionFactory> actionFactory =
        clang::tooling::newFrontendActionFactory<SDFrontendAction>();

    const int result = tool.run(actionFactory.get());

    if (result != 0)
    {
        llvm::errs()
            << "static-dragon: analysis failed with code "
            << result
            << '\n';

        return result;
    }

    llvm::outs() << "Static Dragon analysis completed.\n";

    return 0;
}
