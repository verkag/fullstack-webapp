#include "server.hpp"

int main()
{
    Server s;
    s.resources["^/[0-9]+/$"]["GET"] = [](const beast::http::request<beast::http::string_body> req, beast::http::response<beast::http::string_body>& res) {
        res.body() = req.target();
        res.content_length(req.target().size());
        res.keep_alive(req.keep_alive());
    };
    s.run();
}

