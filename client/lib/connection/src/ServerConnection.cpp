#include "ServerConnection.h"

#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include <iostream>

ServerConnection::ServerConnection(unsigned short port, const std::string &host)
    : io_context_(), socket_(io_context_), is_connected_(true) {
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(host), port);
    boost::system::error_code error;
    socket_.connect(endpoint, error);
    is_connected_ = !error;
}

bool ServerConnection::is_connected() {
    return is_connected_;
}

std::pair<bool, std::string>
ServerConnection::request(const std::string &message) {
    std::string               request_message = message + "\r\n";
    boost::system::error_code error;

    boost::asio::write(socket_, boost::asio::buffer(request_message), error);
    if (error) {
        is_connected_ = false;
        return std::make_pair(false, error.message());
    }

    boost::asio::read_until(socket_, buffer_, "\r\n", error);
    if (error) {
        is_connected_ = false;
        return std::make_pair(false, error.message());
    }

    std::string  response;
    std::istream is(&buffer_);
    std::getline(is, response);
    return std::make_pair(true, response);
}
