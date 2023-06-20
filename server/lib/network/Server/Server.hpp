#ifndef SERVER_H
#define SERVER_H

#include "PluginsProvider.hpp"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class PluginServer {
 public:
    explicit PluginServer(std::shared_ptr<PluginsProvider> &&provider,
                          boost::asio::io_context           &context,
                          uint16_t                           port);

    ~PluginServer()                                        = default;
    PluginServer(const PluginServer &)                     = delete;
    PluginServer(PluginServer &&)                          = delete;
    auto operator=(const PluginServer &) -> PluginServer & = delete;
    auto operator=(PluginServer &&) -> PluginServer      & = delete;

 private:
    boost::asio::io_context         &io_context_;
    boost::asio::ip::tcp::acceptor   acceptor_;
    std::shared_ptr<PluginsProvider> plugins_provider_;

    void                             start_accept();

    void handle_accept(const boost::system::error_code &error);
};

#endif  // SERVER_H!
