#include "BasicPluginWrapper.h"

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

using namespace nlohmann;

BasicPluginWrapper::BasicPluginWrapper(std::shared_ptr<IRequestable> connection,
                                       std::string plugin_type)
    : connection_(std::move(connection)), plugin_type_(std::move(plugin_type)) {
}

std::string BasicPluginWrapper::init(const std::string &plugin_name) {
    json request_message = {
        {"query_type",  "init"      },
        {"plugin_type", plugin_type_},
        {"plugin_name", plugin_name }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return "Server disconnected";
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return response_message.at("message").get<std::string>();
        return {};
    } catch (...) {
        return "Wrong response format: " + response.second;
    }
}

std::pair<std::string, std::string> BasicPluginWrapper::get_default_config() {
    json request_message = {
        {"query_type",  "get_default_config"},
        {"plugin_type", plugin_type_        }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {"", "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {"", response_message.at("message").get<std::string>()};
        return {response_message.at("result")[0].dump(2),
                response_message.at("result")[1].get<std::string>()};
    } catch (...) {
        return {"", "Wrong response format: " + response.second};
    }
}

std::pair<std::string, std::string> BasicPluginWrapper::get_default_scheme() {
    json request_message = {
        {"query_type",  "get_default_scheme"},
        {"plugin_type", plugin_type_        }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {"", "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {"", response_message.at("message").get<std::string>()};
        return {response_message.at("result")[0].dump(2),
                response_message.at("result")[1].get<std::string>()};
    } catch (...) {
        return {"", "Wrong response format: " + response.second};
    }
}

std::pair<std::map<std::string, std::string>, std::string>
BasicPluginWrapper::set_config(const std::string &new_config) {
    json config;
    try {
        config = json::parse(new_config);
    } catch (...) {
        return {{}, "Wrong input format"};
    }
    json request_message = {
        {"query_type",  "set_config"},
        {"plugin_type", plugin_type_},
        {"query",       config      }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {{}, "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {response_message.at("result")
                        .get<std::map<std::string, std::string>>(),
                    ""};
        return {{}, ""};
    } catch (...) {
        return {{}, "Wrong response format: " + response.second};
    }
}

std::pair<LoadResult, std::string> BasicPluginWrapper::list_plugins() {
    json request_message = {
        {"query_type",  "list_plugins"},
        {"plugin_type", plugin_type_  }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {{}, "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {{}, response_message.at("message").get<std::string>()};
        return {
            LoadResult{response_message.at("result")
                           .at("success")
                           .get<std::vector<std::string>>(),
                       response_message.at("result")
                           .at("failed")
                           .get<std::vector<std::string>>()},
            ""
        };
    } catch (...) {
        return {{}, "Wrong response format: " + response.second};
    }
}

std::pair<LoadResult, std::string> BasicPluginWrapper::load_new_plugins() {
    json request_message = {
        {"query_type",  "load_new_plugins"},
        {"plugin_type", plugin_type_      }
    };
    std::pair<bool, std::string> response(
        connection_->request(request_message.dump()));
    if (!response.first)
        return {{}, "Server disconnected"};
    try {
        json response_message = json::parse(response.second);
        if (response_message.at("status").get<int>() != 0)
            return {{}, response_message.at("message").get<std::string>()};
        return {
            LoadResult{response_message.at("result")
                           .at("success")
                           .get<std::vector<std::string>>(),
                       response_message.at("result")
                           .at("failed")
                           .get<std::vector<std::string>>()},
            ""
        };
    } catch (...) {
        return {{}, "Wrong response format: " + response.second};
    }
}
