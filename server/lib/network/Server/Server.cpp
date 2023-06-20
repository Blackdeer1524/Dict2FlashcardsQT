#include "Server.hpp"
#include "ResponseGenerators.hpp"
#include "Session.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/bind.hpp>
#include <boost/python.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/import.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

using boost::asio::ip::tcp;
using nlohmann::json;

PluginServer::PluginServer(std::shared_ptr<PluginsProvider> &&provider,
                           boost::asio::io_context           &context,
                           uint16_t                           port)
    : io_context_(context),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),
      plugins_provider_(std::move(provider)) {
    SPDLOG_INFO("Initializing Python interpreter");
    Py_Initialize();
    SPDLOG_INFO("Successfully initialized Python interpreter");

    SPDLOG_INFO("Starting accepting requests");
    start_accept();
}

void PluginServer::start_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec,
                                  tcp::socket               socket) {
        if (!ec) {
            auto test = std::make_unique<ResponseGenerator>(plugins_provider_);
            std::make_shared<Session>(std::move(socket), std::move(test))
                ->start();
        }
        start_accept();
    });
}
