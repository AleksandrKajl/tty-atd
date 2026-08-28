#include "atd/application.hpp"

#include <charconv>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

namespace {

void printUsage(std::ostream& output, const char* programName) {
    output << "Использование: " << programName << " --device <путь> --dictionary <путь> [--baud <скорость>]\n";
}

int reportArgumentError(const char* programName, std::string_view message) {
    std::cerr << "Ошибка: " << message << '\n';
    printUsage(std::cerr, programName);
    return 1;
}

} // namespace

int main(int argc, char* argv[]) {
    atd::ServerConfig config;

    for (int idx = 1; idx < argc; idx++) {
        const std::string_view argument = argv[idx];

        if (argument == "--help" || argument == "-h") {
            printUsage(std::cout, argv[0]);
            return 0;
        }

        if (argument == "--device") {
            if (++idx >= argc) {
                return reportArgumentError(argv[0], "не указан путь после --device");
            }
            config.devicePath = argv[idx];
            continue;
        }

        if (argument == "--dictionary") {
            if (++idx >= argc) {
                return reportArgumentError(argv[0], "не указан путь после --dictionary");
            }
            config.dictionaryPath = argv[idx];
            continue;
        }

        if (argument == "--baud") {
            if (++idx >= argc) {
                return reportArgumentError(argv[0], "не указана скорость после --baud");
            }

            const std::string_view baudRateArgument = argv[idx];
            unsigned int baudRate = 0;
            const auto [parsedEnd, error] =
                std::from_chars(baudRateArgument.data(), baudRateArgument.data() + baudRateArgument.size(), baudRate);

            if (error != std::errc{} || parsedEnd != baudRateArgument.data() + baudRateArgument.size()) {
                return reportArgumentError(argv[0], "скорость должна быть целым неотрицательным числом");
            }

            config.baudRate = baudRate;
            continue;
        }

        return reportArgumentError(argv[0], "неизвестный аргумент: " + std::string(argument));
    }

    if (config.devicePath.empty()) {
        return reportArgumentError(argv[0], "не указан обязательный аргумент --device");
    }
    if (config.dictionaryPath.empty()) {
        return reportArgumentError(argv[0], "не указан обязательный аргумент --dictionary");
    }

    try {
        atd::runServer(config);
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
