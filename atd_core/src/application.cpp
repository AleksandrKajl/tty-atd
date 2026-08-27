#include "atd/application.hpp"
#include "atd/command_dictionary.hpp"
#include "atd/serial_port.hpp"
#include "atd/server.hpp"

namespace atd {

void runServer(const ServerConfig& config) {
    CommandDictionary dictionary(config.dictionaryPath);
    SerialPort port(config.devicePath, config.baudRate);

    AtServer server(port, dictionary);
    server.run();
}
} // namespace atd
