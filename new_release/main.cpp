// Modular version of growth_calc
#include "include/config.h"
#include "include/growth_calc.h"
#include <iostream>
#include <string>

static void printHelp() {
	std::cout << "Growth Pattern Calculator - Modular C++ Application\n" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "  1. With command-line arguments:" << std::endl;
	std::cout << "     ./growth_calc_modular <base> <exponent> [logfile]" << std::endl;
	std::cout << "  2. With config file:" << std::endl;
	std::cout << "     ./growth_calc_modular --config <config_file>" << std::endl;
	std::cout << "  3. Default config file:" << std::endl;
	std::cout << "     ./growth_calc_modular" << std::endl;
	std::cout << "     (Uses config.txt in current directory)\n" << std::endl;
}

int main(int argc, char* argv[]) {
	Config config;
	bool useConfigFile = false;
	std::string configFilePath = "config.txt";

	if (argc > 1) {
		std::string arg1 = argv[1];
		if (arg1 == "--help" || arg1 == "-h") {
			printHelp();
			return 0;
		} else if (arg1 == "--config") {
			if (argc < 3) {
				std::cerr << "Error: --config requires a file path" << std::endl;
				printHelp();
				return 1;
			}
			configFilePath = argv[2];
			useConfigFile = true;
		} else if (argc >= 3) {
			try {
				config.base = std::stod(argv[1]);
				config.exponent = std::stoi(argv[2]);
				if (argc >= 4) config.logFile = argv[3];
				std::cout << "Using command-line parameters:\n";
				std::cout << "  Base = " << config.base << std::endl;
				std::cout << "  Exponent = " << config.exponent << std::endl;
				std::cout << "  Log file = " << config.logFile << std::endl;
			} catch (...) {
				std::cerr << "Error: Invalid arguments. Expected: <base> <exponent> [logfile]" << std::endl;
				printHelp();
				return 1;
			}
		} else {
			std::cerr << "Error: Invalid arguments" << std::endl;
			printHelp();
			return 1;
		}
	} else {
		useConfigFile = true;
	}

	if (useConfigFile) {
		std::cout << "Reading configuration from: " << configFilePath << std::endl;
		if (!ConfigLoader::readFile(configFilePath, config)) {
			std::cerr << "Failed to read config file. Using default values." << std::endl;
			std::cerr << "Default: base=2, exponent=5" << std::endl;
			std::cout << "\nTo create a config file, run: ./growth_calc_modular --help" << std::endl;
		} else {
			std::cout << "Configuration loaded successfully:" << std::endl;
			std::cout << "  Base = " << config.base << std::endl;
			std::cout << "  Exponent = " << config.exponent << std::endl;
			std::cout << "  Log file = " << config.logFile << std::endl;
		}
	}

	if (config.exponent < 1) {
		std::cerr << "Error: Exponent must be at least 1" << std::endl;
		return 1;
	}

	if (config.exponent > 100) {
		std::cerr << "Warning: Large exponent (" << config.exponent << ") may take a long time!" << std::endl;
		std::cout << "Estimated time: " << (config.exponent * 2) << " seconds" << std::endl;
	}

	GrowthCalc app(config);
	return app.run();
}

