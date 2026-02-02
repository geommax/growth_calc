#include "growth_calc.h"
#include "print_utils.h"
#include "thread.h"
#include <iostream>
#include <fstream>

GrowthCalc::GrowthCalc(const Config& config) : config_(config) {}

int GrowthCalc::run() {
    Printer printer(config_.enableLogging);

    if (config_.enableLogging) {
        if (!printer.open(config_.logFile)) {
            std::cerr << "Warning: Could not open log file: " << config_.logFile << std::endl;
            std::cerr << "Continuing without file logging..." << std::endl;
            printer.setEnabled(false);
        } else {
            std::cout << "Logging to: " << config_.logFile << "\n" << std::endl;
        }
    }

    printer.print("========================================");
    printer.print("NEW CALCULATION SESSION STARTED");
    printer.print("========================================");

    WorkerThread::runLinear(config_.base, config_.exponent, printer);
    WorkerThread::runExponential(config_.base, config_.exponent, printer);

    printer.print("========================================");
    printer.print("CALCULATION SESSION COMPLETED");
    printer.print("========================================");

    std::cout << "Calculation completed!" << std::endl;
    if (config_.enableLogging) {
        std::cout << "Logs saved to: " << config_.logFile << std::endl;
    }
    return 0;
}
