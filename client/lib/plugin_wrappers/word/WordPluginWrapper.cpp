#include "WordPluginWrapper.h"

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "Card.h"
#include "IRequestable.h"

using namespace nlohmann;

WordPluginWrapper::WordPluginWrapper(std::shared_ptr<IRequestable> connection)
    : BasicPluginWrapper(std::move(connection), "word") {
}

std::pair<std::vector<Card>, std::string>
WordPluginWrapper::get(const std::string &word,
                       const std::string &query_language,
                       size_t             batch_size,
                       bool               reload) {
    json request_message = {
        {"query_type",  "get"         },
        {"plugin_type", plugin_type_  },
        {"word",        word          },
        {"filter",      query_language},
        {"batch_size",  batch_size    },
        {"restart",     reload        }
    };
    std::pair<bool, std::string> response(
        std::move(connection_->request(request_message.dump())));
    if (!response.first)
        return {std::vector<Card>(), "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {std::vector<Card>(),
                    response_message.at("error").get<std::string>()};
        return {response_message.at("result").get<std::vector<Card>>(),
                response_message.at("error").get<std::string>()};
    } catch (...) {
        return {std::vector<Card>(), "Wrong response format"};
    }
}

std::pair<std::string, std::string> WordPluginWrapper::get_dict_scheme() {
    json request_message = {
        {"query_type",  "get_dict_scheme"},
        {"plugin_type", plugin_type_     }
    };
    std::pair<bool, std::string> response(
        std::move(connection_->request(request_message.dump())));
    if (!response.first)
        return {"", "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {"", response_message.at("error").get<std::string>()};
        return {response_message.at("result").dump(2),
                response_message.at("error").get<std::string>()};
    } catch (...) {
        return {"", "Wrong response format"};
    }
}