#ifndef SESSION_H
#define SESSION_H

#include "ResponseGenerators.hpp"
#include <boost/asio.hpp>
#include <concepts>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

class Session : public std::enable_shared_from_this<Session> {
 public:
    explicit Session(boost::asio::ip::tcp::socket        socket,
                     std::unique_ptr<IResponceGenerator> response_generator);

    void start();

 private:
    boost::asio::ip::tcp::socket        socket_;
    boost::asio::streambuf              request_buffer;
    std::unique_ptr<IResponceGenerator> response_generator_;

    void                                do_read();
    void                                do_write(std::string response);
};

#endif  // SESSION_H
