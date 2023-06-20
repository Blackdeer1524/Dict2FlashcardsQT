#include "interpreter.hpp"

interpreter::interpreter(){};

Value interpreter::interpret(Expr *expression, nlohmann::json card_) {
    card = card_;
    return evaluate(expression);
}

Value interpreter::evaluate(Expr *expression) {
    return expression->accept(this);
}

bool interpreter::is_truthy(const Value &val) {
    if (std::holds_alternative<std::monostate>(val)) {
        return false;
    }
    if (std::holds_alternative<double>(val)) {
        return std::get<double>(val) != 0;
    }
    if (std::holds_alternative<bool>(val)) {
        return std::get<bool>(val);
    }

    return true;
}

bool interpreter::is_equal(const Value &left, const Value &right) {
    if (std::holds_alternative<bool>(left) &&
        std::holds_alternative<bool>(right)) {

        return std::get<bool>(left) == std::get<bool>(right);

    } else if (std::holds_alternative<double>(left) &&
               std::holds_alternative<double>(right)) {

        return std::get<double>(left) == std::get<double>(right);

    } else if (std::holds_alternative<nlohmann::json>(left) &&
               std::holds_alternative<nlohmann::json>(right)) {

        return std::get<nlohmann::json>(left) ==
               std::get<nlohmann::json>(right);

    } else if (std::holds_alternative<std::monostate>(left) &&
               std::holds_alternative<std::monostate>(right)) {

        return true;  // Если оба значения имеют тип std::monostate, считаем их
                      // равными
    }

    return false;  // Если типы не совпадают, считаем значения неравными
}

void interpreter::check_number_operand(const Value &operand) {
    if (!std::holds_alternative<double>(operand)) {
        throw ComponentException("Invalid operand type");
    }
}

void interpreter::check_json_operand(const Value &operand) {
    if (!std::holds_alternative<nlohmann::json>(operand)) {
        throw ComponentException("Expected JSON");
    }
}

void interpreter::check_json_is_number(const Value &operand) {
    if (!std::get<nlohmann::json>(operand).is_number()) {
        throw ComponentException("JSON is not a number");
    }
}

Value interpreter::visit(Binary *expr) {
    Value left  = evaluate(expr->get_leftptr());
    Value right = evaluate(expr->get_rightptr());

    if (std::holds_alternative<double>(left) &&
        std::holds_alternative<double>(right)) {
        double left_  = std::get<double>(left);
        double right_ = std::get<double>(right);
        switch (expr->get_opername().type) {
            case PLUS:
                return Value(left_ + right_);
            case MINUS:
                return Value(left_ - right_);

            case STAR:
                return Value(left_ * right_);

            case SLASH:
                return Value(left_ / right_);

            case LESS:
                return Value(left_ < right_);

            case LESS_EQUAL:
                return Value(left_ <= right_);

            case GREATER:
                return Value(left_ > right_);

            case GREATER_EQUAL:
                return Value(left >= right);

            case BANG_EQUAL:
                return Value(!is_equal(left_, right_));

            case EQUAL_EQUAL:
                return Value(is_equal(left_, right_));

            default:
                throw ComponentException("Invalid operand type");
        }
    } else if (std::holds_alternative<nlohmann::json>(left) &&
               std::holds_alternative<nlohmann::json>(right)) {
        nlohmann::json left_  = std::get<nlohmann::json>(left);
        nlohmann::json right_ = std::get<nlohmann::json>(right);
        if (expr->get_opername().type == PLUS) {
            return Value(mergeJson(left_, right_));
        } else {
            throw ComponentException("Invalid operand type");
        }
    } else {
        throw ComponentException("Invalid operand type");
    }
}

Value interpreter::visit(Grouping *expr) {
    return expr->get_expr()->accept(this);
}

Value interpreter::visit(FuncIn *expr) {
    Value left  = evaluate(expr->get_leftptr());
    Value right = evaluate(expr->get_rightptr());

    if (std::holds_alternative<std::string>(left) &&
        std::holds_alternative<nlohmann::json>(right)) {

        std::string    left_  = std::get<std::string>(left);
        nlohmann::json right_ = std::get<nlohmann::json>(right);
        return Value(find_word_inJson(left_, right_));
    } else {
        return Value(false);
    }
}

Value interpreter::visit(Unary *expr) {
    Value right = evaluate(expr->get_expr());

    switch (expr->get_opername().type) {
        case tt::MINUS:
            check_number_operand(right);
            return Value(-std::get<double>(right));
        case tt::NOT:
            return Value(!is_truthy(right));
        case tt::LEN:
            check_json_operand(right);
            return Value(json_length(std::get<nlohmann::json>(right)));
        case tt::SPLIT:
            check_json_operand(right);
            return Value(split_json(std::get<nlohmann::json>(right)));
        case tt::UPPER:
            check_json_operand(right);
            return Value(upper_json_string(std::get<nlohmann::json>(right)));
        case tt::LOWER:
            check_json_operand(right);
            return Value(lower_json_string(std::get<nlohmann::json>(right)));
        case tt::REDUCE:
            check_json_operand(right);
            return Value(reduce_json(std::get<nlohmann::json>(right)));
        case tt::NUM:
            check_json_operand(right);
            check_json_is_number(right);
            return Value(std::get<nlohmann::json>(right).get<double>());
        default:
            throw ComponentException("Invalid operand type");
    }
}

Value interpreter::visit(LogicalExpr *ex) {
    Value left = evaluate(ex->get_leftptr());
    if (ex->get_opername().type == OR) {
        if (is_truthy(left)) {
            return left;
        }
    } else {
        if (!is_truthy(left)) {
            return left;
        }
    }
    return evaluate(ex->get_rightptr());
}

Value interpreter::visit(Literal *expr) {
    if (!expr->get_json_namevec().empty()) {
        nlohmann::json json_val =
            find_json_value(card, expr->get_json_namevec());
        if (!json_val.empty()) {
            expr->get_value() = Value(json_val);
        } else {
            expr->get_value() = Value();
        }
    }
    return expr->get_value();
}

nlohmann::json interpreter::upper_json_string(const nlohmann::json &data) {

    if (data.is_string()) {
        std::string str = data.get<std::string>();
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    if (data.is_array()) {
        nlohmann::json result = nlohmann::json::array();
        for (const auto &item : data) {
            result.push_back(upper_json_string(item));
        }
        return result;
    }

    return data;
}

nlohmann::json interpreter::lower_json_string(const nlohmann::json &data) {

    if (data.is_string()) {
        std::string str = data.get<std::string>();
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    if (data.is_array()) {
        nlohmann::json result = nlohmann::json::array();
        for (const auto &item : data) {
            result.push_back(lower_json_string(item));
        }
        return result;
    }

    return data;
}

std::vector<std::string> interpreter::split_string(const std::string &str) {
    std::vector<std::string> words;
    std::string              word;
    std::istringstream       iss(str);
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

// Рекурсивная функция для разделения JSON элемента
nlohmann::json interpreter::split_json(const nlohmann::json &jsonValue) {
    if (jsonValue.is_null()) {
        throw ComponentException("Element is null");
    }

    if (jsonValue.is_string()) {
        std::string str = jsonValue.get<std::string>();
        return split_string(str);
    }

    if (jsonValue.is_array()) {
        nlohmann::json result = nlohmann::json::array();
        for (const auto &item : jsonValue) {
            if (item.is_string()) {
                std::string str = item.get<std::string>();
                result.push_back(split_string(str));
            }
        }
        return result;
    }

    throw ComponentException("Invalid operation");
}

double interpreter::json_length(const nlohmann::json &jsonValue) {
    if (jsonValue.is_null()) {
        throw ComponentException("Element is null");
    }

    if (jsonValue.is_array() || jsonValue.is_object()) {
        return jsonValue.size();
    }

    return 1;  // неитерируемый объект, например число или строка
}

bool interpreter::find_word_inJson(std::string word, nlohmann::json jsonValue) {
    if (jsonValue.is_string()) {
        return jsonValue.get<std::string>() == word;
    }

    if (!jsonValue.is_array()) {
        return false;
    }

    bool found = std::ranges::any_of(jsonValue, [&](const auto &element) {
        return find_word_inJson(word, element);
    });

    if (found) {
        return true;
    }

    return false;
}

nlohmann::json
interpreter::find_json_value(const nlohmann::json    &card,
                             std::vector<std::string> levels_vec) {
    nlohmann::json current_json = card;

    for (size_t i = 0; i < levels_vec.size(); ++i) {
        std::string key = levels_vec[i];

        if (current_json.is_object()) {
            if (current_json.contains(key)) {
                current_json = current_json[key];
            } else if (key == "$ANY") {
                return handle_any_key(current_json, levels_vec, i);
            } else if (key == "$SELF") {
                return get_self_keys(current_json);
            } else {
                throw ComponentException("Invalid operand type");
            }
        } else {
            throw ComponentException("Invalid operand type");
        }
    }

    return current_json;
}

nlohmann::json
interpreter::handle_any_key(const nlohmann::json           &json_Value,
                            const std::vector<std::string> &levels_vec,
                            size_t                          current_index) {
    nlohmann::json any_values;

    for (auto it = json_Value.begin(); it != json_Value.end(); ++it) {
        const auto &result = find_json_value(
            it.value(),
            std::vector<std::string>(levels_vec.begin() + current_index + 1,
                                     levels_vec.end()));

        if (!result.is_null()) {
            any_values.push_back(result);
        }
    }

    if (any_values.size() == 1) {  // возвращаем элемент, а не массив
        return any_values[0];
    } else {
        return any_values;
    }
}

nlohmann::json interpreter::get_self_keys(const nlohmann::json &json_Value) {
    nlohmann::json self_keys;

    for (auto it = json_Value.begin(); it != json_Value.end(); ++it) {
        self_keys.push_back(it.key());
    }

    return self_keys;
}

nlohmann::json interpreter::reduce_json(const nlohmann::json &jsonElem) {
    if (jsonElem.is_null()) {
        throw ComponentException("Element is null");
    }

    nlohmann::json result;

    for (const auto &item : jsonElem) {
        if (item.is_array()) {
            for (const auto &nestedItem : item) {
                result.push_back(nestedItem);
            }
        } else {
            result.push_back(item);
        }
    }

    return result;
}

nlohmann::json interpreter::mergeJson(const nlohmann::json &jsonLeft,
                                      const nlohmann::json &jsonRight) {
    nlohmann::json mergedJson = nlohmann::json::object();

    for (nlohmann::json::const_iterator it = jsonLeft.begin();
         it != jsonLeft.end();
         ++it) {
        mergedJson[it.key()] = it.value();
    }
    for (nlohmann::json::const_iterator it = jsonRight.begin();
         it != jsonRight.end();
         ++it) {
        mergedJson[it.key()] = it.value();
    }
    return mergedJson;
}
