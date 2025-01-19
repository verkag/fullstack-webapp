#include "server.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/awaitable.hpp>

Server::Server(const unsigned short port) : // default args are specified in declarations but not in defenitions 
    acceptor_(ioc_), cfg(port)
{}

void Server::run() {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(cfg.address), cfg.port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::socket_base::reuse_address(cfg.reuse_address));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    //boost::asio::co_spawn(boost::asio::this_coro::executor, accept(), boost::asio::detached);

}
