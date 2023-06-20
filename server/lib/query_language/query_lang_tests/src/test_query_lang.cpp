#include "interpreter.hpp"
#include <gtest/gtest.h>

TEST(QueryLangTest, ScannerTest1) {
    scanner            scan("1 + 2");
    std::vector<token> tokens = scan.scan_tokens();

    ASSERT_EQ(tokens.size(), 4);
    ASSERT_EQ(tokens[0].type, tt::NUMBER);
    ASSERT_EQ(tokens[1].type, tt::PLUS);
    ASSERT_EQ(tokens[2].type, tt::NUMBER);
}

TEST(QueryLangTest, ScannerTest2) {
    scanner            scan("\"A2\" in tags[level]");
    std::vector<token> result = scan.scan_tokens();
    ASSERT_EQ(result.size(), 7);
    ASSERT_EQ(result[0].type, tt::STRING);
    ASSERT_EQ(result[1].type, tt::IN);
    ASSERT_EQ(result[2].type, tt::IDENTIFIER);
    ASSERT_EQ(result[3].type, tt::LEFT_BRACKET);
    ASSERT_EQ(result[4].type, tt::IDENTIFIER);
    ASSERT_EQ(result[5].type, tt::RIGHT_BRACKET);
}

nlohmann::json jsonCard = {
    {"word",        "example"                                                    },
    {"special",     {"special1", "special2"}                                     },
    {"definition",  "This is an example"                                         },
    {"examples",    {"Example 1", "Example 2"}                                   },
    {"image_links", {"image1.jpg", "image2.jpg"}                                 },
    {"audio_links", {"audio1.mp3", "audio2.mp3"}                                 },
    {"tags",        {{"tag1", {"value1", "moscow"}}, {"level", {"A1"}}}          },
    {"other",
     {{"key1", "value1 слово pnfvinv 345"}, {"key2", 5}, {"key3", "value3"}}}
};

TEST(QueryLangTest, InterpreterTestFuncInTrue) {
    scanner               scan("\"A1\" in tags[level]");
    std::vector<token>    tokens = scan.scan_tokens();
    parser                par(tokens);
    std::unique_ptr<Expr> exp = par.parse();
    interpreter           inter;
    Value                 result       = inter.interpret(exp.get(), jsonCard);
    bool                  containsBool = std::holds_alternative<bool>(result);
    ASSERT_TRUE(std::get<bool>(result));
}

TEST(QueryLangTest, InterpreterTestFuncInFalse) {
    scanner               scan("\"A2\" in tags[level]");
    std::vector<token>    tokens = scan.scan_tokens();
    parser                par(tokens);
    std::unique_ptr<Expr> exp = par.parse();
    interpreter           inter;
    Value                 result       = inter.interpret(exp.get(), jsonCard);
    bool                  containsBool = std::holds_alternative<bool>(result);
    ASSERT_FALSE(std::get<bool>(result));
}

TEST(QueryLangTest, InterpreterTestFuncInAnd) {
    scanner scan("\"A1\" in tags[level] and \"value3\" in other[key3]");
    std::vector<token>    tokens = scan.scan_tokens();
    parser                par(tokens);
    std::unique_ptr<Expr> exp = par.parse();
    interpreter           inter;
    Value                 result       = inter.interpret(exp.get(), jsonCard);
    bool                  containsBool = std::holds_alternative<bool>(result);
    ASSERT_TRUE(std::get<bool>(result));
}

TEST(QueryLangTest, InterpreterTestNumber) {
    scanner               scan("1 + 3/(4-1)*(2+1)");
    std::vector<token>    tokens = scan.scan_tokens();
    parser                par(tokens);
    std::unique_ptr<Expr> exp = par.parse();
    interpreter           inter;
    Value                 result = inter.interpret(exp.get(), jsonCard);
    double containsDouble        = std::holds_alternative<double>(result);
    ASSERT_EQ(std::get<double>(result), 4);
}

TEST(QueryLangTest, InterpreterTestFuncNum) {
    scanner               scan("num(other[key2])");
    std::vector<token>    tokens = scan.scan_tokens();
    parser                par(tokens);
    std::unique_ptr<Expr> exp = par.parse();
    interpreter           inter;
    Value                 result = inter.interpret(exp.get(), jsonCard);
    double containsDouble        = std::holds_alternative<double>(result);
    ASSERT_EQ(std::get<double>(result), 5);
}
