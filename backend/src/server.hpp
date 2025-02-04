#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;

class Server {
public: 
    void run();
    // add logger logic here
private:
    asio::awaitable<void> accept(); 
    asio::awaitable<void> process_session(beast::tcp_stream stream); 
    beast::http::message_generator handle_request(beast::http::request<beast::http::string_body>&& req);
    void find_resource();
};
