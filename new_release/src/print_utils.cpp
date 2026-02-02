#include "print_utils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

Printer::Printer(bool enableLogging) : enabled_(enableLogging) {}

Printer::~Printer() {
    if (log_.is_open()) {
        log_.close();
    }
}

bool Printer::open(const std::string& filePath) {
    if (!enabled_) return false;
    log_.open(filePath, std::ios::app);
    return log_.is_open();
}

void Printer::setEnabled(bool enabled) {
    enabled_ = enabled;
}

void Printer::print(const std::string& message) {
    std::string line = "[" + timestamp() + "] " + message;
    std::cout << line << std::endl;
    if (enabled_ && log_.is_open()) {
        log_ << line << std::endl;
        log_.flush();
    }
}

std::string Printer::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
