#include <boost/asio.hpp>

class Connection {
public: 
    Connection(const boost::asio::io_context ioc);
    boost::asio::ip::tcp::socket socket; 
    boost::asio::strand<boost::asio::any_io_executor> write_strand;
};
