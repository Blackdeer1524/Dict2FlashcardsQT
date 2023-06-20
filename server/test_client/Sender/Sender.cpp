#include "Sender.hpp"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string.h>
#include <string>

Sender::Sender(const std::string &hostname, uint16_t port) : socket_(ios_) {
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(hostname), port);
    socket_.connect(endpoint);
}

auto Sender::request(const std::string &str_request) -> nlohmann::json {
    bufffer_ = {};

    strncpy(bufffer_.data(), str_request.c_str(), str_request.size());

    boost::system::error_code error;

    size_t                    len = socket_.write_some(
        boost::asio::buffer(bufffer_, strlen(bufffer_.data())), error);
    assert(len == str_request.size());
    size_t len2 = socket_.read_some(boost::asio::buffer(bufffer_), error);
    auto   parsed_response =
        nlohmann::json::parse(bufffer_.data(), bufffer_.data() + len2);

    return parsed_response;
}
