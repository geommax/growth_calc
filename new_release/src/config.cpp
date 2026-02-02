#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>

static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool ConfigLoader::readFile(const std::string& filename, Config& config) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        if (key == "base") {
            config.base = std::stod(value);
        } else if (key == "exponent" || key == "expo") {
            config.exponent = std::stoi(value);
        } else if (key == "logfile") {
            config.logFile = value;
        } else if (key == "enable_logging") {
            config.enableLogging = (value == "true" || value == "1");
        }
    }

    file.close();
    return true;
}
