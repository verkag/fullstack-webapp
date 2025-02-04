#include "server.hpp"

void Server::run() {
    asio::io_context ioc;

    asio::co_spawn(ioc, accept(), asio::detached);

    ioc.run(); 
}

asio::awaitable<void> Server::accept() {
    auto executor = co_await boost::asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v4(), 5555});
    acceptor.set_option(asio::socket_base::reuse_address());

    for (;;) {
        asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
        beast::tcp_stream stream(std::move(socket));
        asio::co_spawn(executor, process_session(std::move(stream)), asio::detached); 
    }   
}

asio::awaitable<void> Server::process_session(beast::tcp_stream stream) {
    beast::flat_buffer buffer; 

    for(;;) {
        stream.expires_after(std::chrono::seconds(5));
        beast::http::request<beast::http::string_body> req;

        co_await beast::http::async_read(stream, buffer, req, asio::use_awaitable);

        beast::http::message_generator msg = handle_request(std::move(req));

        bool keep_alive = msg.keep_alive();
        co_await beast::async_write(stream, std::move(msg), asio::use_awaitable);
        if (!keep_alive) break;
    }

    stream.socket().shutdown(asio::ip::tcp::socket::shutdown_send);
}

beast::http::message_generator Server::handle_request(beast::http::request<beast::http::string_body>&& req) {
    beast::http::response<beast::http::string_body> res{beast::http::status::ok, req.version()};
    res.body() = "123";
    res.content_length(3);
    res.keep_alive(req.keep_alive());
    return res;
}







