#pragma once

#include <clang/Tooling/Tooling.h>
#include <memory> 
#include "SDFrontendAction.h"
#include "SDMetricsCollector.h"

class SDFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    explicit SDFrontendActionFactory(SDMetricsCollector& collector);
    std::unique_ptr<clang::FrontendAction> create() override;

private:
     SDMetricsCollector& collector_;
};


