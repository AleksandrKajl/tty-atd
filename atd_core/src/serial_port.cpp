#include "atd/serial_port.hpp"
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <system_error>
#include <termios.h>
#include <unistd.h>

namespace {

speed_t toSpeed(unsigned int baudRate) {

    switch (baudRate) {
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    default:
        throw std::invalid_argument("Неподдерживаемая скорость: " + std::to_string(baudRate));
    }
}
} // namespace

namespace atd {

SerialPort::SerialPort(const std::string& device, unsigned int baudRate) {
    // Вычисляем скорость до открытия устройств, чтобы не было утечки
    const speed_t speed = toSpeed(baudRate);

    int fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_CLOEXEC);

    if (fd == -1) {
        throw std::system_error(errno, std::generic_category(), "Не удалось открыть последовательный порт: " + device);
    }

    termios settings{};

    if (tcgetattr(fd, &settings) == -1) {
        const int err = errno;
        close(fd);
        throw std::system_error(err, std::generic_category(), "Не удалось получить настройки последовательного порта");
    }

    cfmakeraw(&settings);

    // 8N1
    settings.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
    settings.c_cflag |= CS8;

    // Включить приём и игнорировать линии управления модемом
    settings.c_cflag |= CREAD | CLOCAL;

    // Отключить аппаратный flow control
    settings.c_cflag &= ~CRTSCTS;

    // Отключить программный Flow control
    settings.c_iflag &= ~(IXON | IXOFF | IXANY);

    if (cfsetispeed(&settings, speed) == -1 || cfsetospeed(&settings, speed) == -1
        || tcsetattr(fd, TCSANOW, &settings) == -1) {
        const int err = errno;
        close(fd);
        throw std::system_error(err, std::generic_category(), "Не удалось сконфигурировать последовательный порт");
    }

    descriptor_ = fd;
}

SerialPort::~SerialPort() {
    if (descriptor_ != -1) {
        close(descriptor_);
    }
}

std::size_t SerialPort::read(char* buffer, std::size_t size) {
    ssize_t bytesRead = 0;

    while (!bytesRead) {
        bytesRead = ::read(descriptor_, buffer, size);

        if (bytesRead == -1) {
            if (errno == EINTR) {
                bytesRead = 0;
                continue;
            }

            throw std::system_error(errno, std::generic_category(),
                                    "Не удалось прочитать данные из последовательного порта");
        } else if (bytesRead == 0) {
            break;
        }
    }

    return static_cast<std::size_t>(bytesRead);
}

void SerialPort::writeAll(std::string_view data) {
    std::size_t sent = 0;

    while (sent < data.size()) {
        const ssize_t bytesWritten = write(descriptor_, data.data() + sent, data.size() - sent);

        if (bytesWritten == -1) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "Не удалось записать данные в последовательный порт");

        } else if (bytesWritten == 0) {
            throw std::runtime_error("write вернул 0 до отправки всех данных");
        }

        sent += static_cast<std::size_t>(bytesWritten);
    }
}

} // namespace atd
