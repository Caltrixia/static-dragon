#include "SDFrondendActionFactory.h"

SDFrondendActionFactory::SDFrondendActionFactory(SDMetricsCollector& collector) 
                                                                : collector_(collector)
{
}

std::unique_ptr<clang::FrontendAction> SDFrondendActionFactory::create()
{
    return std::make_unique<SDFrontendAction>(collector_);
}