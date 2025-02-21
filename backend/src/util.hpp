#pragma once 

#include <regex>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class CustomRegex : public std::regex {
public: 
    std::string str;
    CustomRegex(std::string s);
    CustomRegex(const char* cs); // to allow construction form string literals
    bool operator<(const CustomRegex& rhs) const noexcept;
};


void setup_sectificate(boost::asio::ssl::context& _);
