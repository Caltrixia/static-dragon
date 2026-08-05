#include "SDFrontendActionFactory.h"

SDFrontendActionFactory::SDFrontendActionFactory(SDMetricsCollector& collector) 
                                                                : collector_(collector)
{
}

std::unique_ptr<clang::FrontendAction> SDFrontendActionFactory::create()
{
    return std::make_unique<SDFrontendAction>(collector_);
}