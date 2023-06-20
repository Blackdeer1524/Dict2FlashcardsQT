#include "parser.hpp"

parser::parser(std::vector<token> &t) : tokens(t){};

bool parser::is_at_end() {
    return tokens[current].type == token_type::EOTF;
}

token parser::peek() {
    return tokens[current];
}

token parser::previous() {
    return tokens[current - 1];
}

token parser::advance() {
    if (!is_at_end())
        current++;
    return previous();
}

bool parser::check(token_type type) {
    return !is_at_end() && peek().type == type;
}

bool parser::match(std::vector<token_type> types) {
    return std::ranges::any_of(types, [this](const token_type &type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    });
}

std::unique_ptr<Expr> parser::primary() {
    token_type type = peek().type;
    advance();

    switch (type) {
        case tt::FALSE:
            return std::make_unique<Literal>(false);
        case tt::TRUE:
            return std::make_unique<Literal>(true);
        case tt::NUMBER:
            return std::make_unique<Literal>(std::stod(previous().literal));
        case tt::STRING:
            return std::make_unique<Literal>(previous().literal);
        case tt::LEFT_PAREN: {
            std::unique_ptr<Expr> Expr = expression();
            if (match({tt::RIGHT_PAREN})) {
                return std::make_unique<Grouping>(std::move(Expr));
            }
            throw ComponentException(
                "Missing closing parenthesis for grouping.");
        }
        case tt::IDENTIFIER: {
            std::vector<std::string> json_fields = read_json_elem();
            if (!json_fields.empty()) {
                return std::make_unique<Literal>(json_fields);
            }
            throw ComponentException("Invalid JSON element.");
        }
        default:
            throw ComponentException("Unexpected token encountered.");
    }
}

std::vector<std::string> parser::read_json_elem() {
    std::vector<std::string> json_fields;
    json_fields.push_back(previous().lexeme);
    while (match({tt::LEFT_BRACKET})) {
        if (match({tt::IDENTIFIER})) {
            json_fields.push_back(previous().lexeme);
        } else {
            return std::vector<std::string>();
        }

        if (!match({tt::RIGHT_BRACKET})) {
            throw ComponentException("Missing closing parenthesis for JSON.");
        }
    }
    return json_fields;
}

std::unique_ptr<Expr> parser::func_in() {
    std::unique_ptr<Expr> left = primary();
    if (match({tt::IN})) {
        /*if(!match({tt::LEFT_PAREN})){
            throw ComponentException("Missing open parenthesis for function
        call.");
        }*/

        std::unique_ptr<Expr> right = primary();

        /*if (!match({tt::RIGHT_PAREN})) {
            throw ComponentException("Missing closing parenthesis for function
        call.");
        }*/

        return std::make_unique<FuncIn>(std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::unar() {
    if (match({tt::NOT,
               tt::MINUS,
               tt::LEN,
               tt::SPLIT,
               tt::UPPER,
               tt::LOWER,
               tt::REDUCE,
               tt::NUM})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = unar();
        return std::make_unique<Unary>(std::move(right), oper);
    }
    return func_in();
}

std::unique_ptr<Expr> parser::multiplication() {
    std::unique_ptr<Expr> left = unar();
    while (match({tt::SLASH, tt::STAR})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = unar();
        left =
            std::make_unique<Binary>(std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::addition() {
    std::unique_ptr<Expr> left = multiplication();
    while (match({tt::MINUS, tt::PLUS})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = multiplication();
        left =
            std::make_unique<Binary>(std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::comparison() {
    std::unique_ptr<Expr> left = addition();
    while (match({tt::LESS, tt::LESS_EQUAL, tt::GREATER, tt::GREATER_EQUAL})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = addition();
        left =
            std::make_unique<Binary>(std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::equality() {
    std::unique_ptr<Expr> left = comparison();
    while (match({tt::BANG_EQUAL, tt::EQUAL_EQUAL})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = comparison();
        left =
            std::make_unique<Binary>(std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::and_expr() {
    std::unique_ptr<Expr> left = equality();

    while (match({tt::AND})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = equality();
        left                        = std::make_unique<LogicalExpr>(
            std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::or_expr() {
    std::unique_ptr<Expr> left = and_expr();

    while (match({tt::OR})) {
        token                 oper  = previous();
        std::unique_ptr<Expr> right = and_expr();
        left                        = std::make_unique<LogicalExpr>(
            std::move(left), oper, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> parser::expression() {
    return or_expr();
}

std::unique_ptr<Expr> parser::parse() {
    return expression();
}
