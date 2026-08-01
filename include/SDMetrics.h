#pragma once

#include <string>
#include <vector>

struct FunctionBodyMetrics
{
    unsigned statementCount{0};
    unsigned cyclomaticComplexity{1};
    unsigned maximumNestingDepth{0};
    unsigned returnCount{0};
    unsigned functionCallCount{0};
};

struct FunctionMetrics
{
    // Identification
    std::string name;
    std::string qualifiedName;
    std::string file;

    // Source location
    unsigned startLine{0};
    unsigned endLine{0};
    unsigned physicalLines{0};

    // Declaration metrics
    unsigned parameterCount{0};
    bool isDefinition{false};
    bool isConstexpr{false};
    bool isConst{false};
    bool isStatic{false};
    bool isInline{false};
    bool isVirtual{false};
    bool isPureVirtual{false};
    bool isTemplate{false};

    // Body metrics
    FunctionBodyMetrics functionBodyMetrics;
};

struct TranslationUnitMetrics
{
    std::string file;
    std::vector<FunctionMetrics> functionMetrics;
};

