#pragma once

#include "classes.hpp"

class parser {
 public:
    parser(std::vector<token> &t);
    std::unique_ptr<Expr> parse();

 private:
    std::vector<token>       tokens;
    std::vector<std::string> read_json_elem();
    int                      current = 0;
    bool                     is_at_end();
    bool                     check(token_type type);
    bool                     match(std::vector<token_type> types);
    token                    peek();
    token                    previous();
    token                    advance();
    std::unique_ptr<Expr>    primary();
    std::unique_ptr<Expr>    func_in();
    std::unique_ptr<Expr>    unar();
    std::unique_ptr<Expr>    multiplication();
    std::unique_ptr<Expr>    addition();
    std::unique_ptr<Expr>    comparison();
    std::unique_ptr<Expr>    equality();
    std::unique_ptr<Expr>    and_expr();
    std::unique_ptr<Expr>    or_expr();
    std::unique_ptr<Expr>    expression();
};
