#pragma once
#include "config.h"

class Printer;

class GrowthCalc {
public:
    explicit GrowthCalc(const Config& config);
    int run();

private:
    Config config_;
};
