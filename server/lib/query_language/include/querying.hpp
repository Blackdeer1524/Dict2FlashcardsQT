#ifndef QUERYING_H
#define QUERYING_H

#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>

auto prepare_filter(const std::string &query) -> std::function<
    std::variant<bool, std::string>(const nlohmann::json &json_card)>;

#endif  // !QUERYING_H
