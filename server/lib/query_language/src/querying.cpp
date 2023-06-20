#include "querying.hpp"

#include "exception.hpp"
#include "interpreter.hpp"
#include "parser.hpp"
#include "scaner.hpp"
#include "spdlog/fmt/bundled/core.h"
#include "spdlog/spdlog.h"
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <variant>

auto prepare_filter(const std::string &query) -> std::function<
    std::variant<bool, std::string>(const nlohmann::json &json_card)> {
    SPDLOG_INFO("Preparing query: `{}`", query);
    if (query.empty()) {
        SPDLOG_INFO("Query `{}` is considered empty, returning a constant true "
                    "function",
                    query);
        return [](const nlohmann::json &) -> bool { return true; };
    }

    SPDLOG_INFO("Scanning query: `{}`", query);
    scanner            scan(query);
    std::vector<token> tokens = scan.scan_tokens();

    SPDLOG_INFO("Parsing query: `{}`", query);
    auto                  p   = parser(tokens);
    std::shared_ptr<Expr> exp = p.parse();
    if (exp == nullptr) {
        SPDLOG_WARN("Parsing query `{}` resulted in empty expression, "
                    "returning a constant false function",
                    query);
        return [](const nlohmann::json &) -> bool { return false; };
    }

    SPDLOG_INFO("Setting up an interpreter for ther query: `{}`", query);
    interpreter inter;
    return [i = std::move(inter),
            e = std::move(exp)](const nlohmann::json &json_card) mutable
           -> std::variant<bool, std::string> {
        Value val;
        try {
            val = i.interpret(e.get(), json_card);
        } catch (const ComponentException &error) {
            return error.what();
        }
        if (std::holds_alternative<bool>(val)) {
            return std::get<bool>(val);
        }
        return fmt::format("Resulting type has to be `bool`");
    };
}
