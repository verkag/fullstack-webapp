#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/ssl.hpp>
#include <functional>
#include <map>
#include "util.hpp"


#include <spdlog/spdlog.h>
#include "spdlog/async.h"

namespace asio = boost::asio;
namespace beast = boost::beast;

//using executor_type = asio::strand<asio::io_context::executor_type>;
//using stream_type = typename beast::tcp_stream::rebind_executor<executor_type>::other;

using string_body = beast::http::string_body;

class Server {
public: 
    void run();
    Server();
    ~Server();

    std::map<CustomRegex, std::map<std::string, std::function<void(const beast::http::request<string_body>, beast::http::response<string_body>&)>>> resources;
private:
    asio::awaitable<void> accept(); 
    asio::awaitable<void> process_session(beast::tcp_stream stream); 
    template<typename Stream>
    asio::awaitable<void> run_session(Stream& stream, beast::flat_buffer& buffer);
    beast::http::message_generator handle_request(beast::http::request<beast::http::string_body>&& req);

    std::shared_ptr<spdlog::async_logger> logger_; 
    std::shared_ptr<spdlog::details::thread_pool> ltp_;

    asio::io_context ioc_;
    asio::ip::tcp::acceptor acceptor_; // probably need acceptor_type 
    asio::signal_set signals_;

    boost::asio::ssl::context ssl_ctx_;
};
