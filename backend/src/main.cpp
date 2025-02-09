#include "server.hpp"
#include <iostream> 
#include <pqxx/pqxx>

int main() {
    Server s;

    pqxx::connection cx("dbname=backdb user=verkag password=123zxc");

    s.resources["^/[0-9]+/$"]["GET"] = [&cx](const beast::http::request<beast::http::string_body> req, beast::http::response<beast::http::string_body>& res) {
        pqxx::work tx(cx);
        pqxx::result r = tx.exec("SELECT * from foo;");
        tx.commit();

        for (const auto& row : r) {
            for (const auto& elem : row) {
                std::cout << elem.c_str() << std::endl;
            }
        }

        res.body() = req.target();
        res.content_length(req.target().size());
        res.keep_alive(req.keep_alive());
    };
    s.run();
}

