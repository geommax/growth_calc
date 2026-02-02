#pragma once
#include <string>

class Printer;

class WorkerThread {
public:
    static void runLinear(double base, int exponent, Printer& printer);
    static void runExponential(double base, int exponent, Printer& printer);
};
