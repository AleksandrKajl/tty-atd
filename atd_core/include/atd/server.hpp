#ifndef ATD_SERVER_HPP
#define ATD_SERVER_HPP

namespace atd {

class CommandDictionary;
class SerialPort;

class AtServer {
  public:
    AtServer(SerialPort& port, const CommandDictionary& dictionary);

    void run();

  private:
    SerialPort& port_;
    const CommandDictionary& dictionary_;
};

} // namespace atd

#endif // ATD_SERVER_HPP
