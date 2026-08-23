#ifndef ATD_SERIAL_PORT_HPP
#define ATD_SERIAL_PORT_HPP

#include <cstddef>
#include <string>
#include <string_view>

namespace atd {

class SerialPort {
  public:
    SerialPort(const std::string& device, unsigned int baudRate);
    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    std::size_t read(char* buffer, std::size_t size);
    void writeAll(std::string_view data);

  private:
    int descriptor_ = -1;
};

} // namespace atd

#endif // ATD_SERIAL_PORT_HPP
