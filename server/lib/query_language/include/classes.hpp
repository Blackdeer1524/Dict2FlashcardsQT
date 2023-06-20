#pragma once

#include "scaner.hpp"
#include <variant>

class Binary;
class Grouping;
class Unary;
class LogicalExpr;
class FuncIn;
class Literal;

// Value передается по вершинам аст дерева
using Value =
    std::variant<std::monostate, bool, double, std::string, nlohmann::json>;

// интерпретатор аст дерева обязан определить
// все функции для посещения его вершин
class ExprVisitor {
 public:
    virtual ~ExprVisitor()                 = default;
    virtual Value visit(Binary *Expr)      = 0;
    virtual Value visit(Unary *Expr)       = 0;
    virtual Value visit(Literal *Expr)     = 0;
    virtual Value visit(LogicalExpr *Expr) = 0;
    virtual Value visit(FuncIn *Expr)      = 0;
    virtual Value visit(Grouping *Expr)    = 0;
};

// базовый класс аст дерева
class Expr {
 public:
    virtual ~Expr()                            = default;
    virtual Value accept(ExprVisitor *visitor) = 0;
};

// класс для переменной, считанной из запроса
class Literal : public Expr {
 public:
    explicit Literal(std::string v);
    explicit Literal(double d);
    explicit Literal(bool b);
    explicit Literal(std::vector<std::string> json_namevec);
    Literal();

    Value                   &get_value();
    std::vector<std::string> get_json_namevec();
    Value                    accept(ExprVisitor *visitor) override;

 private:
    std::vector<std::string> json_namevec;
    Value                    val;
};

// класс бинарных операций
class Binary : public Expr {
 public:
    Binary(std::unique_ptr<Expr> left_,
           token                 tok,
           std::unique_ptr<Expr> right_);

    Expr *get_leftptr();
    Expr *get_rightptr();
    token get_opername();
    Value accept(ExprVisitor *visitor) override;

 private:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    token                 oper;
};

class FuncIn : public Expr {
 public:
    FuncIn(std::unique_ptr<Expr> left_, std::unique_ptr<Expr> right_);

    Expr *get_leftptr();
    Expr *get_rightptr();
    Value accept(ExprVisitor *visitor) override;

 private:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

// элементарный класс унарных операций
class Unary : public Expr {
 public:
    Unary(std::unique_ptr<Expr> Expression_, token tok);

    Expr *get_expr();
    token get_opername();
    Value accept(ExprVisitor *visitor) override;

 private:
    std::unique_ptr<Expr> expression;
    token                 oper;
};

class LogicalExpr : public Expr {
 public:
    LogicalExpr(std::unique_ptr<Expr> left_,
                token                 tok,
                std::unique_ptr<Expr> right_);

    Expr *get_leftptr();
    Expr *get_rightptr();
    token get_opername();
    Value accept(ExprVisitor *visitor) override;

 private:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    token                 oper;
};

// класс для выражений в скобках
class Grouping : public Expr {
 public:
    Grouping(std::unique_ptr<Expr> Expression_);

    Expr *get_expr();
    Value accept(ExprVisitor *visitor);

 private:
    std::unique_ptr<Expr> expression;
};
