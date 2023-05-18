#include "FormatProcessorPluginWrapper.h"

#include <memory>
#include <string>

#include "IRequestable.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

FormatProcessorPluginWrapper::FormatProcessorPluginWrapper(
    std::shared_ptr<IRequestable> connection)
    : BasicPluginWrapper(std::move(connection), "format") {
}

std::string FormatProcessorPluginWrapper::save(const std::string &cards_path,
                                               const std::string &links_path,
                                               const std::string &media_path) {
    json request_message = {
        {"query_type", "save"    },
        {"cards_path", cards_path},
        {"links_path", links_path},
        {"media_path", media_path},
    };
    std::pair<bool, std::string> response(
        std::move(connection_->request(request_message.dump())));
    if (!response.first)
        return "Server disconnected";
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return response_message.at("error").get<std::string>();
        return {};
    } catch (...) {
        return "Wrong response format";
    }
}