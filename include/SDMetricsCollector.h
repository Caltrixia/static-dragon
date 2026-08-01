#pragma once

#include "SDMetrics.h"

class SDMetricsCollector 
{
public:
    void addTranslationUnit(TranslationUnitMetrics metrics);

    const std::vector<TranslationUnitMetrics>& getTranslationUnits() const;
private:
    std::vector<TranslationUnitMetrics> translationUnits_;
};