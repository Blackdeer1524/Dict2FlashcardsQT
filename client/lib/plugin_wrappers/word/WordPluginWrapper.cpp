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
                       bool               restart) {
    json request_message = {
        {"query_type",  "get"         },
        {"plugin_type", plugin_type_  },
        {"word",        word          },
        {"filter",      query_language},
        {"batch_size",  batch_size    },
        {"restart",     restart       }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {{}, "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0) {
            return {std::vector<Card>(),
                    response_message.at("message").get<std::string>()};
        }
        json              cards_with_error = response_message["result"];
        std::vector<Card> cards            = cards_with_error[0];

        return {cards, cards_with_error[1]};
    } catch (...) {
        return {{}, "Wrong response format: " + response.second};
    }
}

std::pair<std::string, std::string> WordPluginWrapper::get_dict_scheme() {
    json request_message = {
        {"query_type",  "get_dict_scheme"},
        {"plugin_type", plugin_type_     }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first) {
        return {"", "Server disconnected"};
    }
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0) {
            return {"", response_message.at("message").get<std::string>()};
        }
        return {response_message.at("result")[0].dump(2),
                response_message.at("result")[1].get<std::string>()};
    } catch (...) {
        return {"", "Wrong response format: " + response.second};
    }
}
