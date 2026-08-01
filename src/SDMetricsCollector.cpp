#include "SDMetricsCollector.h"

#include <vector>

void SDMetricsCollector::addTranslationUnit(TranslationUnitMetrics metrics)
{
    translationUnits_.push_back(std::move(metrics));
}

const std::vector<TranslationUnitMetrics>& SDMetricsCollector::getTranslationUnits() const
{
    return translationUnits_;
}