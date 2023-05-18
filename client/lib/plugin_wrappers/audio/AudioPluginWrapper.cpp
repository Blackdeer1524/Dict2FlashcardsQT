#include "AudioPluginWrapper.h"

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using namespace nlohmann;
using audio_vector = std::vector<std::pair<std::string, std::string>>;

AudioPluginWrapper::AudioPluginWrapper(std::shared_ptr<IRequestable> connection)
    : BasicPluginWrapper(std::move(connection), "audios") {
}

std::pair<audio_vector, std::string>
AudioPluginWrapper::get(const std::string &word, size_t batch_size) {
    json request_message = {
        {"query_type",  "get"       },
        {"plugin_type", plugin_type_},
        {"word",        word        },
        {"batch_size",  batch_size  },
    };
    std::pair<bool, std::string> response(
        std::move(connection_->request(request_message.dump())));
    if (!response.first)
        return {audio_vector(), "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {audio_vector(),
                    response_message.at("error").get<std::string>()};
        return {response_message.at("result").get<audio_vector>(),
                response_message.at("error").get<std::string>()};
    } catch (...) {
        return {audio_vector(), "Wrong response format"};
    }
}