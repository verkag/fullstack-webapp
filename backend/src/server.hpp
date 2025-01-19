#include <boost/asio.hpp>
#include "config.hpp"
#include "connection.hpp"

class Server {
public: 
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    explicit Server(const unsigned short port = 80);

    void run();
    Config cfg;    
private:
    void accept(); 
    void read(Connection c); 
    void find_resource(Connection c);
    void write(Connection c);

    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
