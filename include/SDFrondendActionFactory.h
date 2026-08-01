#pragma once

#include <clang/Tooling/Tooling.h>
#include <memory> 
#include "SDFrontendAction.h"
#include "SDMetricsCollector.h"

class SDFrondendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    explicit SDFrondendActionFactory(SDMetricsCollector& collector);
    std::unique_ptr<clang::FrontendAction> create() override;

private:
     SDMetricsCollector& collector_;
};


