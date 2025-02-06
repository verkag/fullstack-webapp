#include <functional>
#include <map>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "util.hpp"


namespace asio = boost::asio;
namespace beast = boost::beast;

using string_body = beast::http::string_body;

class Server {
public: 
    void run();
    // add logger logic here
    std::map<CustomRegex, std::map<std::string, std::function<void(const beast::http::request<string_body>, beast::http::response<string_body>&)>>> resources;
private:
    asio::awaitable<void> accept(); 
    asio::awaitable<void> process_session(beast::tcp_stream stream); 
    beast::http::message_generator handle_request(beast::http::request<beast::http::string_body>&& req);
    void find_resource();
};
