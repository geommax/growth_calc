#include "thread.h"
#include "print_utils.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>

void WorkerThread::runLinear(double base, int exponent, Printer& printer) {
    std::cout << "\n=== LINEAR GROWTH (Incremental Multiplication: B * E) ===" << std::endl;
    printer.print("Starting Linear Growth Calculation");
    printer.print("Base = " + std::to_string(base) + ", Exponent = " + std::to_string(exponent));

    double result = base;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    printer.print("Step 1: " + std::to_string(base) + " = " + std::to_string(result));

    for (int i = 2; i <= exponent; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        result = base * i;

        std::stringstream ss;
        ss << "Step " << i << ": " << base << " × " << i << " = " << result;
        printer.print(ss.str());
    }

    printer.print("Final Linear Result: " + std::to_string(result));
    std::cout << std::endl;
}

static inline std::string formatDouble(double v) {
    std::stringstream ss;
    if (std::abs(v) > 1e6 || (std::abs(v) < 0.000001 && v != 0)) {
        ss << std::scientific << std::setprecision(6) << v;
    } else {
        ss << std::fixed << std::setprecision(6) << v;
    }
    return ss.str();
}

void WorkerThread::runExponential(double base, int exponent, Printer& printer) {
    std::cout << "=== EXPONENTIAL GROWTH (Incremental Exponentiation: B^E) ===" << std::endl;
    printer.print("Starting Exponential Growth Calculation");
    printer.print("Base = " + std::to_string(base) + ", Exponent = " + std::to_string(exponent));

    double result = base;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    printer.print("Step 1: " + std::to_string(base) + "^1 = " + formatDouble(result));

    for (int i = 2; i <= exponent; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        result = std::pow(base, i);

        std::stringstream ss;
        ss << "Step " << i << ": " << base << "^" << i << " = " << formatDouble(result);
        printer.print(ss.str());
    }

    printer.print(std::string("Final Exponential Result: ") + formatDouble(result));
    std::cout << std::endl;
}
