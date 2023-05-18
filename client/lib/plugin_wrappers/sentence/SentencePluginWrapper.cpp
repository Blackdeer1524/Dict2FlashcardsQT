#include "SentencePluginWrapper.h"

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "IRequestable.h"

using namespace nlohmann;

SentencePluginWrapper::SentencePluginWrapper(
    std::shared_ptr<IRequestable> connection)
    : BasicPluginWrapper(std::move(connection), "sentences") {
}

std::pair<std::vector<std::string>, std::string>
SentencePluginWrapper::get(const std::string &word, size_t batch_size) {
    json request_message = {
        {"query_type",  "get"       },
        {"plugin_type", plugin_type_},
        {"word",        word        },
        {"batch_size",  batch_size  },
    };
    std::pair<bool, std::string> response(
        std::move(connection_->request(request_message.dump())));
    if (!response.first)
        return {std::vector<std::string>(), "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {std::vector<std::string>(),
                    response_message.at("error").get<std::string>()};
        return {response_message.at("result").get<std::vector<std::string>>(),
                response_message.at("error").get<std::string>()};
    } catch (...) {
        return {std::vector<std::string>(), "Wrong response format"};
    }
}
