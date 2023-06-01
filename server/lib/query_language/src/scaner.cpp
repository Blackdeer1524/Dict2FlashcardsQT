#include "scaner.hpp"

#include <utility>

token::token() : type(token_type::NUL) {
}

token::token(token_type type) : type(type) {
}

token::token(token_type type, std::string lexeme)
    : type(type), lexeme(std::move(lexeme)) {
}

token::token(token_type type, std::string lexeme, std::string literal)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)) {
}

void scanner::init_keywords() {
    keywords["in"]     = tt::IN;
    keywords["len"]    = tt::LEN;
    keywords["split"]  = tt::SPLIT;
    keywords["lower"]  = tt::LOWER;
    keywords["upper"]  = tt::UPPER;
    keywords["all"]    = tt::ALL;
    keywords["any"]    = tt::ANY;
    keywords["and"]    = tt::AND;
    keywords["or"]     = tt::OR;
    keywords["not"]    = tt::NOT;
    keywords["reduce"] = tt::REDUCE;
    keywords["num"]    = tt::NUM;
}

auto scanner::has_next(size_t i) -> bool {
    return current + i < source.size();
}

auto scanner::is_digit(char ch) -> bool {
    return std::isdigit(ch);
}

void scanner::add_token(token_type type) {
    add_token(type, "");
}

void scanner::add_token(token_type type, const std::string &literal) {
    std::string const text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal);
}

auto scanner::advance() -> char {
    current++;
    return source[current - 1];
}

auto scanner::match(const std::string &expected) -> bool {
    if (!has_next(expected.size())) {
        return false;
    }
    if (source.substr(current, expected.size()) != expected) {
        return false;
    }
    current += expected.size();
    return true;
}

auto scanner::peek() -> char {
    if (!has_next()) {
        return '\0';
    }
    return source[current];
}

auto scanner::peek_next() -> char {
    if (current + 1 >= source.size()) {
        return '\0';
    }
    return source[current + 1];
}

void scanner::number() {

    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
        advance();
        while (is_digit(peek())) {
            advance();
        }
    }

    add_token(tt::NUMBER, source.substr(start, current - start));
}

void scanner::read_json_keyword() {
    advance();  // считали $
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string const text = source.substr(start, current - start);
    if (text == "$ANY" || text == "$SELF") {
        add_token(tt::IDENTIFIER);
    }
    //
}

void scanner::read_json_level() {
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string const text = source.substr(start, current - start);
    auto              type = keywords.find(text);
    type == keywords.end() ? add_token(tt::IDENTIFIER)
                                        : add_token(type->second);
}

void scanner::string() {

    while (peek() != '"' && has_next()) {
        advance();
    }

    // нет вторых кавычек
    if (!has_next()) {

        throw ComponentException("Missing closing quotation mark for string.");
    }

    advance();  // пропуск для кавычки
    // добавляем слово без кавычек
    std::string const value = source.substr(start + 1, current - start - 2);
    add_token(tt::STRING, value);
}

void scanner::scan_token() {
    const char token = advance();
    switch (token) {
        case '(':
            add_token(tt::LEFT_PAREN);
            break;
        case ')':
            add_token(tt::RIGHT_PAREN);
            break;
        case '[':
            add_token(tt::LEFT_BRACKET);
            break;
        case ']':
            add_token(tt::RIGHT_BRACKET);
            break;
        case ',':
            add_token(tt::COMMA);
            break;
        case '.':
            add_token(tt::DOT);
            break;
        case '-':
            add_token(tt::MINUS);
            break;
        case '+':
            add_token(tt::PLUS);
            break;
        case ';':
            add_token(tt::SEMICOLON);
            break;
        case '*':
            add_token(tt::STAR);
            break;
        case '/':
            add_token(SLASH);
            break;
        case '?':
            add_token(tt::QMARK);
            break;
        case ':':
            add_token(tt::COLON);
            break;
        case '!':
            add_token(match(std::string(1, '=')) ? tt::BANG_EQUAL : tt::BANG);
            break;
        case '=':
            add_token(match(std::string("=")) ? tt::EQUAL_EQUAL : tt::EQUAL);
            break;
        case '<':
            add_token(match(std::string("=")) ? tt::LESS_EQUAL : tt::LESS);
            break;
        case '>':
            add_token(match(std::string("=")) ? tt::GREATER_EQUAL
                                              : tt::GREATER);
            break;
        case ' ':
        case '\r':
        case '\n':
        case '\t':
            break;
        case '"':
            string();
            break;

        default:
            if (is_digit(token)) {
                number();
            } else if (token == '$') {
                read_json_keyword();
            } else if (isalpha(token)) {
                read_json_level();
            } else {
                throw ComponentException("Unexpected character encountered.");
            }
            break;
    }
}

scanner::scanner(std::string source_) : source(std::move(source_)) {
    init_keywords();
}

auto scanner::scan_tokens() -> std::vector<token> {
    while (has_next()) {
        start = current;
        scan_token();
    }
    tokens.emplace_back(EOTF, "", "");
    return tokens;
}
