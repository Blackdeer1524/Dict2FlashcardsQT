#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <string>

#include <boost/asio.hpp>

#include "IRequestable.h"

class ServerConnection : public IRequestable {
 public:
    ServerConnection(unsigned short     port,
                     const std::string &host = "127.0.0.1");
    ~ServerConnection() override;
    bool        is_connected();
    std::pair<bool, std::string> request(const std::string &message) override;

 private:
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket             socket_;
    boost::asio::streambuf  buffer_;
    bool is_connected_;
};

#endif  // SERVERCONNECTION_H
