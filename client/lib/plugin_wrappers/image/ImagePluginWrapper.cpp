#include "ImagePluginWrapper.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using namespace nlohmann;

ImagePluginWrapper::ImagePluginWrapper(std::shared_ptr<IRequestable> connection)
    : BasicPluginWrapper(std::move(connection), "images") {
}

std::pair<Media, std::string> ImagePluginWrapper::get(const std::string &word,
                                                      size_t batch_size,
                                                      bool   restart) {
    json request_message = {
        {"query_type",  "get"       },
        {"plugin_type", plugin_type_},
        {"word",        word        },
        {"batch_size",  batch_size  },
        {"restart",     restart     }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first) {
        return {{}, "Server disconnected"};
    }
    try {
        std::cout << response.second << std::endl;
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0) {
            return {{}, response_message.at("message").get<std::string>()};
        }

        json              links_with_error = response_message.at("result");
        Media links           = links_with_error[0];
        return {links, links_with_error[1]};
    } catch (...) {
        return {{}, "Wrong response format: " + response.second};
    }
}
