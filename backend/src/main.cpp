#include "test.hpp"
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context ioc{1};
    foo();
}

