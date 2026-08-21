#include "atd/pattern_matcher.hpp"

namespace atd {

bool matches(std::string_view pattern, std::string_view command) {
    std::size_t patternIdx = 0;
    std::size_t commandIdx = 0;
    std::size_t lastStarIdx = std::string_view::npos;
    std::size_t starCommandIdx = 0;

    while (commandIdx < command.size()) {
        if (patternIdx < pattern.size() && (pattern[patternIdx] == '.' || pattern[patternIdx] == command[commandIdx])) {
            ++patternIdx;
            ++commandIdx;
        } else if (patternIdx < pattern.size() && pattern[patternIdx] == '*') {
            lastStarIdx = patternIdx;
            ++patternIdx;
            starCommandIdx = commandIdx;
        } else if (lastStarIdx != std::string_view::npos) {
            patternIdx = lastStarIdx + 1;
            commandIdx = ++starCommandIdx;
        } else {
            return false;
        }
    }

    while (patternIdx < pattern.size() && pattern[patternIdx] == '*') {
        ++patternIdx;
    }

    return patternIdx == pattern.size();
}

} // namespace atd
