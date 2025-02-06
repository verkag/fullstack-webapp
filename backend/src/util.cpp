#include "util.hpp"

CustomRegex::CustomRegex(std::string s) : std::regex(s), str(std::move(s)) {}

CustomRegex::CustomRegex(const char* cs) : std::regex(cs), str(cs) {}

bool CustomRegex::operator<(const CustomRegex& rhs) const noexcept {
    return str < rhs.str;
}
