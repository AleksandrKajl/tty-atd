#include "atd/server.hpp"

#include "atd/command_dictionary.hpp"
#include "atd/serial_port.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

constexpr std::size_t readBufferSize = 256;
constexpr std::size_t maxCommandSize = 1024;

constexpr std::string_view lineEnding = "\r\n";
constexpr std::string_view errorResponse = "ERROR";

} // namespace

namespace atd {

AtServer::AtServer(SerialPort& port, const CommandDictionary& dictionary) : port_(port), dictionary_(dictionary) {
}

void AtServer::run() {
    std::array<char, readBufferSize> readBuffer;
    std::string command;
    command.reserve(maxCommandSize);

    bool isDiscarding = false;

    const auto sendResponse = [this](std::string_view response) {
        port_.writeAll(lineEnding);
        port_.writeAll(response);
        port_.writeAll(lineEnding);
    };

    while (true) {
        const std::size_t bytesRead = port_.read(readBuffer.data(), readBuffer.size());

        if (bytesRead == 0) {
            throw std::runtime_error("Соединение с последовательным портом закрыто");
        }

        for (std::size_t idx = 0; idx < bytesRead; idx++) {
            const char byte = readBuffer[idx];

            if (byte == ' ') {
                continue;
            }

            if (byte == '\r' || byte == '\n') {
                if (isDiscarding) {
                    sendResponse(errorResponse);
                    isDiscarding = false;
                } else if (!command.empty()) {
                    const std::string_view response = dictionary_.findResponse(command);
                    sendResponse(response.empty() ? errorResponse : response);
                    command.clear();
                }
                continue;
            }

            if (isDiscarding) {
                continue;
            }

            if (command.size() == maxCommandSize) {
                command.clear();
                isDiscarding = true;
                continue;
            }

            command.push_back(byte);
        }
    }
}

} // namespace atd
