#include "atd/command_dictionary.hpp"

#include <fstream>
#include <stdexcept>

namespace {

std::runtime_error makeLineError(const std::string& path, std::size_t lineNumber, const char* reason) {
    return std::runtime_error("Ошибка в словаре " + path + ", строка " + std::to_string(lineNumber) + ": " + reason);
}

std::string decodeResponse(std::string_view encodedResponse, const std::string& path, std::size_t lineNumber) {
    std::string response;
    response.reserve(encodedResponse.size());

    for (std::size_t index = 0; index < encodedResponse.size(); ++index) {
        const char character = encodedResponse[index];
        if (character != '\\') {
            response += character;
            continue;
        }

        if (++index == encodedResponse.size()) {
            throw makeLineError(path, lineNumber, "незавершённая escape-последовательность");
        }

        switch (encodedResponse[index]) {
        case 'r':
            response += '\r';
            break;
        case 'n':
            response += '\n';
            break;
        case '\\':
            response += '\\';
            break;
        default:
            throw makeLineError(path, lineNumber, "неподдерживаемая escape-последовательность");
        }
    }

    return response;
}

} // namespace

namespace atd {

CommandDictionary::CommandDictionary(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error("Не удалось открыть словарь: " + path);
    }

    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        const std::size_t separator = line.find('=');
        if (separator == std::string::npos) {
            throw makeLineError(path, lineNumber, "отсутствует разделитель =");
        }

        const std::string_view pattern(line.data(), separator);
        const std::string_view encodedResponse(line.data() + separator + 1, line.size() - separator - 1);
        if (pattern.empty() || encodedResponse.empty()) {
            throw makeLineError(path, lineNumber, "шаблон и ответ должны быть непустыми");
        }

        rules_.push_back({std::string(pattern), decodeResponse(encodedResponse, path, lineNumber)});
    }

    if (file.bad()) {
        throw std::runtime_error("Не удалось прочитать словарь: " + path);
    }
}

} // namespace atd
