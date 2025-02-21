#pragma once

#include <boost/asio.hpp>
#include <string>

class Config {
private: 
    friend class Server;
    Config(const unsigned short port) : port(port) {}; // Cannot create instance, only freind class allowed

public: 
    /// Port number to use. Defaults to 80 for HTTP and 443 for HTTPS. Set to 0 get an assigned port.
    unsigned short port;
    /// If io_service is not set, number of threads that the server will use when start() is called.
    /// Defaults to 1 thread.
    std::size_t thread_pool_size = 1;
    /// Timeout on request completion. Defaults to 5 seconds.
    long timeout_request = 5;
    /// Timeout on request/response content completion. Defaults to 300 seconds.
    long timeout_content = 300;
    /// Maximum size of request stream buffer. Defaults to architecture maximum.
    /// Reaching this limit will result in a message_size error code.
    std::size_t max_request_streambuf_size = (std::numeric_limits<std::size_t>::max)();
    /// IPv4 address in dotted decimal form or IPv6 address in hexadecimal notation.
    /// If empty, the address will be any address.
    std::string address = "localhost";
    /// Set to false to avoid binding the socket to an address that is already in use. Defaults to true.
    bool reuse_address = true;
    /// Make use of RFC 7413 or TCP Fast Open (TFO)
    bool fast_open = false;
};
