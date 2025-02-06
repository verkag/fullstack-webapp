#include <regex>

class CustomRegex : public std::regex {
public: 
    std::string str;
    CustomRegex(std::string s);
    CustomRegex(const char* cs); // to allow construction form string literals
    bool operator<(const CustomRegex& rhs) const noexcept;
};
