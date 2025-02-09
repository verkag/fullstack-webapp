#include "server.hpp"
#include <iostream>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

#include <pqxx/pqxx>

Server::~Server() {
    logger_->info("server is shutting down..");
}

Server::Server() : ioc_(), acceptor_(ioc_, {asio::ip::tcp::v4(), 5555}), signals_(ioc_) {
    ltp_ = std::make_shared<spdlog::details::thread_pool>(8192, 1);
    spdlog::set_level(spdlog::level::debug);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    console_sink->set_pattern("[%^%l%$] %v");
    console_sink->set_level(spdlog::level::info);

   // auto console_err_sink = std::make_shared<spdlog::sinks::stderr_color_sink_st>();
   // console_err_sink->set_pattern("[%^%l%$] %v");
   // console_err_sink->set_level(spdlog::level::err);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/async_logs.txt", true);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    file_sink->set_level(spdlog::level::debug);

    spdlog::sinks_init_list sil = { console_sink, file_sink };
    logger_ = std::make_shared<spdlog::async_logger>("async_multisink_logger", sil.begin(), sil.end(), ltp_);
    logger_->set_level(spdlog::level::debug);


    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait([this](const boost::system::error_code& /*ec*/, int /*signal_number*/){
        logger_->debug("signal_handler has been called");
        acceptor_.close();
    });
}

void Server::run() {

    logger_->info("server is starting up..");
    asio::co_spawn(ioc_, accept(), asio::detached);
    logger_->debug("co_spawn was passed");

    ioc_.run(); 
}

asio::awaitable<void> Server::accept() {
    auto executor = co_await boost::asio::this_coro::executor; // typically this executor would be transformed to asio::strand when switching to multithreaded model
    logger_->info("server is now accepting connections.."); 

    for (;;) {
        asio::ip::tcp::socket socket = co_await acceptor_.async_accept(asio::use_awaitable);
        beast::tcp_stream stream(std::move(socket));
        asio::co_spawn(executor, process_session(std::move(stream)), asio::detached); 
    }   
}

asio::awaitable<void> Server::process_session(beast::tcp_stream stream) {
    logger_->info("accepted the message");
    beast::flat_buffer buffer; 

    for(;;) {
        stream.expires_after(std::chrono::seconds(10));
        beast::http::request<beast::http::string_body> req;

        co_await beast::http::async_read(stream, buffer, req, asio::use_awaitable);

        beast::http::message_generator msg = handle_request(std::move(req));

        bool keep_alive = msg.keep_alive();
        co_await beast::async_write(stream, std::move(msg), asio::use_awaitable);
        if (!keep_alive) break;
    }

    // this section won't be called if keep_alive is true
    logger_->info("server handled the connection");
    stream.socket().shutdown(asio::ip::tcp::socket::shutdown_send);
}

beast::http::message_generator Server::handle_request(beast::http::request<beast::http::string_body>&& req) {
    const std::string method = req.method_string();
    const std::string target = req.target();
    logger_->debug("inside handle_connection funciton, searching for {} {}", target, method);
    beast::http::response<beast::http::string_body> returned_res;
    for (auto& [k,v] : resources) {
        if (std::regex_search(target, k)) {
            logger_->debug("match found");
            try {
                v[method](std::move(req), returned_res);
            } catch (const std::exception& e) {
                logger_->error("{}", e.what());
            }
            logger_->debug("handler {} {} has been called", target, method); 
        }     
    }

    
    return returned_res;
}







