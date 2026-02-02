#pragma once
#include <string>
#include <fstream>

class Printer {
public:
    explicit Printer(bool enableLogging);
    ~Printer();

    bool open(const std::string& filePath);
    void setEnabled(bool enabled);
    void print(const std::string& message);

private:
    static std::string timestamp();
    bool enabled_;
    std::ofstream log_;
};
