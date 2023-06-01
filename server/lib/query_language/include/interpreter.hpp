#pragma once
#include "parser.hpp"

class interpreter : ExprVisitor {
 public:
    interpreter() = default;
    Value interpret(Expr *expression, nlohmann::json card_);

 private:
    nlohmann::json card;
    Value          result;
    auto           evaluate(Expr *expression) -> Value;
    auto           is_truthy(const Value &val) -> bool;
    auto           is_equal(const Value &left, const Value &right) -> bool;
    Value          visit(Binary *expr) override;
    Value          visit(Grouping *expr) override;
    Value          visit(Unary *expr) override;
    Value          visit(Literal *expr) override;
    Value          visit(FuncIn *expr) override;
    Value          visit(LogicalExpr *ex) override;
    auto find_word_inJson(std::string word, nlohmann::json jsonValue) -> bool;
    void check_json_operand(const Value &operand);
    void check_json_is_number(const Value &operand);
    void check_number_operand(const Value &operand);
    auto find_json_value(const nlohmann::json &card, std::vector<std::string>)
        -> nlohmann::json;
    auto json_length(const nlohmann::json &jsonValue) -> double;
    auto split_string(const std::string &str) -> std::vector<std::string>;
    auto split_json(const nlohmann::json &jsonValue) -> nlohmann::json;
    auto upper_json_string(const nlohmann::json &data) -> nlohmann::json;
    auto lower_json_string(const nlohmann::json &data) -> nlohmann::json;
    auto reduce_json(const nlohmann::json &jsonElem) -> nlohmann::json;
    auto get_self_keys(const nlohmann::json &json_Value) -> nlohmann::json;
    auto handle_any_key(const nlohmann::json           &json_Value,
                        const std::vector<std::string> &levels_vec,
                        size_t current_index) -> nlohmann::json;
    auto mergeJson(const nlohmann::json &jsonLeft,
                   const nlohmann::json &jsonRight) -> nlohmann::json;
};
