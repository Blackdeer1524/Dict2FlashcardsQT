#ifndef TEST_SENDER_H
#define TEST_SENDER_H

#include <array>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <string>

class Sender {
 public:
    explicit Sender(const std::string &hostname, uint16_t port);

    auto request(const std::string &str_request) -> nlohmann::json;

 private:
    std::array<char, 4096>       bufffer_;
    boost::asio::io_service      ios_;
    boost::asio::ip::tcp::socket socket_;
};

#endif
