#ifndef ATD_PATTERN_MATCHER_HPP
#define ATD_PATTERN_MATCHER_HPP

#include <string_view>

namespace atd {

bool matches(std::string_view patern, std::string_view command);

}

#endif // ATD_PATTERN_MATCHER_HPP
