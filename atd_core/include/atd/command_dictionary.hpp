#ifndef ATD_COMMAND_DICTIONARY_HPP
#define ATD_COMMAND_DICTIONARY_HPP

#include <string>
#include <string_view>
#include <vector>

namespace atd {

class CommandDictionary {
public:
    explicit CommandDictionary(const std::string& path);

    std::string_view findResponse(std::string_view normalizedCommand) const;

private:
    struct Rule {
        std::string pattern;
        std::string response;
    };

    std::vector<Rule> rules_;
};

} // namespace atd

#endif // ATD_COMMAND_DICTIONARY_HPP
