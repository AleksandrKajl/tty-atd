#ifndef ATD_APPLICATION_HPP
#define ATD_APPLICATION_HPP

#include <string>

namespace atd {

struct ServerConfig {
    std::string devicePath;
    std::string dictionaryPath;
    unsigned int baudRate = 115200;
};

void runServer(const ServerConfig& config);

} // namespace atd

#endif // ATD_APPLICATION_HPP
