#include "server.hpp"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

#include <pqxx/pqxx>

Server::~Server() {
    logger_->info("server is shutting down..");
}

Server::Server() : ioc_(), acceptor_(ioc_, {asio::ip::tcp::v4(), 5555}), signals_(ioc_), ssl_ctx_(asio::ssl::context::tlsv12) {
    ltp_ = std::make_shared<spdlog::details::thread_pool>(8192, 1);
    spdlog::set_level(spdlog::level::debug);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    console_sink->set_pattern("[%^%l%$] %v");
    console_sink->set_level(spdlog::level::info);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("logs/async_logs.txt", true);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    file_sink->set_level(spdlog::level::debug);

    spdlog::sinks_init_list sil = { console_sink, file_sink };
    logger_ = std::make_shared<spdlog::async_logger>("async_multisink_logger", sil.begin(), sil.end(), ltp_);
    logger_->set_level(spdlog::level::debug);

    setup_sectificate(ssl_ctx_);

    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait([this](const boost::system::error_code& /*ec*/, int /*signal_number*/){
        logger_->debug("signal_handler has been called");
        acceptor_.close();
    });
}

void Server::run() {
    logger_->info("server is starting up..");

    asio::co_spawn(ioc_, accept(), asio::detached); // ???

    ioc_.run(); 
}

asio::awaitable<void> Server::accept() {
    auto executor = co_await boost::asio::this_coro::executor; 
    logger_->info("server is now accepting connections.."); 

    for (;;) {
        asio::ip::tcp::socket socket = co_await acceptor_.async_accept(asio::use_awaitable);
        beast::tcp_stream stream(std::move(socket));
        asio::co_spawn(asio::make_strand(executor), process_session(std::move(stream)), asio::detached); 
    }   
}

asio::awaitable<void> Server::process_session(beast::tcp_stream stream) {
    logger_->info("accepted the message");

    beast::flat_buffer buffer; 
    stream.expires_after(std::chrono::seconds(10));
    auto [ec, res]  = co_await beast::async_detect_ssl(stream, buffer, asio::as_tuple(asio::use_awaitable));
    logger_->debug("ec is: {}, res is: {}", ec.message(), res);  
    if (res) {
        logger_->debug("ssl session handling");
        asio::ssl::stream<beast::tcp_stream> ssl_stream(std::move(stream), ssl_ctx_);
        auto bytes_transfered = co_await ssl_stream.async_handshake(asio::ssl::stream_base::server, buffer.data(), asio::use_awaitable);
        logger_->debug("handshaked successfully with {} bytes", bytes_transfered);
        buffer.consume(bytes_transfered);
        co_await run_session(ssl_stream, buffer);

        if (!ssl_stream.lowest_layer().is_open()) co_return;
        
        co_await ssl_stream.async_shutdown(asio::use_awaitable); // make some error handling 
    } 
    else {
        logger_->debug("no ssl session handling");
        co_await run_session(stream, buffer);

        if (!stream.socket().is_open()) co_return;

        stream.socket().shutdown(asio::ip::tcp::socket::shutdown_send);
    } 



}

template<typename Stream>
asio::awaitable<void> Server::run_session(Stream& stream, beast::flat_buffer& buffer) {
    while (true) {
        logger_->debug("starting run_session func");
        beast::http::request_parser<string_body> parser;
        parser.body_limit(100000);
        
        logger_->debug("socket is {}", beast::get_lowest_layer(stream).socket().is_open());
        auto [ec, bytes]  = co_await beast::http::async_read(stream, buffer, parser, asio::as_tuple(asio::use_awaitable));
        logger_->debug("ec is: {}, bytes are:", ec.message(), bytes);

        if(ec == beast::http::error::end_of_stream) co_return;

        beast::http::message_generator msg = handle_request(parser.release());

        if (!msg.keep_alive()) {
            co_await beast::async_write(stream, std::move(msg), asio::use_awaitable);
            co_return;
        }

        co_await beast::async_write(stream, std::move(msg), asio::use_awaitable);

        // this section won't be called if keep_alive is true
        //logger_->info("server handled the connection");
    }
}

beast::http::message_generator Server::handle_request(beast::http::request<beast::http::string_body>&& req) {
    beast::http::response<beast::http::string_body> returned_res;

    const std::string method = req.method_string();
    const std::string target = req.target();
    
    logger_->debug("strarting handlre_request func with {} and {}", method, target);

    bool loool = false;
    for (auto& [k,v] : resources) {
        if (std::regex_search(target, k)) {
            logger_->debug("match found");
            try {
                v[method](std::move(req), returned_res);
                loool = true;
            }
            catch (const std::exception& e) {
                logger_->error("{}", e.what());
            }
            logger_->debug("handler {} {} has been called", method, target); 
        }     
    }

    if (!loool) logger_->debug("no handlers found");
    
    return returned_res;
}







