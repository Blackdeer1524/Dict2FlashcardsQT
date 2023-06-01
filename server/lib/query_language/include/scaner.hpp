#pragma once

#include "exception.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

enum token_type {
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    QMARK,
    COLON,

    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,
    JSON,
    BOOL,
    EMPTY,
    DOUBLE,

    // logic
    NOT,
    AND,
    OR,
    ANY,
    ALL,
    FALSE,
    TRUE,

    // func
    IN,
    LEN,
    SPLIT,
    LOWER,
    UPPER,
    REDUCE,
    EOTF,
    NUM,
    NUL
};

using tt = token_type;

struct token {
    token();
    token(token_type type);
    token(token_type type, std::string lexeme);
    token(token_type type, std::string lexeme, std::string literal);
    token_type  type;
    std::string lexeme;
    std::string literal;
};

class scanner {
 public:
    scanner(std::string s);
    std::vector<token> scan_tokens();

 private:
    std::string                       source;
    std::vector<token>                tokens;
    std::map<std::string, token_type> keywords;
    int                               start   = 0;
    int                               current = 0;

    void                              init_keywords();
    bool                              has_next(size_t i = 0);
    static bool                       is_digit(char ch);
    void                              add_token(token_type type);
    void add_token(token_type type, const std::string &literal);
    char advance();
    bool match(const std::string &expected);
    char peek();
    char peek_next();
    void number();
    void read_json_level();
    void read_json_keyword();
    void string();
    void scan_token();
};
