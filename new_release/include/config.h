#pragma once
#include <string>

struct Config {
    double base = 2.0;
    int exponent = 5;
    std::string logFile = "../logs/growth_calc.log";
    bool enableLogging = true;
};

class ConfigLoader {
public:
    static bool readFile(const std::string& filename, Config& config);
};
