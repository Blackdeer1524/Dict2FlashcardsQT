#include "BasicPluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(BasicPWSetConfig, Output) {
    auto               memorizer = std::make_shared<Memorizer>();
    BasicPluginWrapper wrapper(memorizer, "tests");
    wrapper.set_config(R"({ "language" : "english", "level" : "C2" })");

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "set_config"                              },
        {"plugin_type", "tests"                                   },
        {"query",       {{"language", "english"}, {"level", "C2"}}}
    };
    EXPECT_EQ(expected, actual);
}

TEST(BasicPWSetConfig, WrongInput) {
    auto               memorizer = std::make_shared<Memorizer>();
    BasicPluginWrapper wrapper(memorizer, "tests");

    std::pair<std::map<std::string, std::string>, std::string> actual =
        wrapper.set_config("not a json");
    std::pair<std::map<std::string, std::string>, std::string> expected = {
        {}, "Wrong input format"};

    EXPECT_EQ(expected, actual);
}

TEST(BasicPWSetConfig, Success) {
    json answer = {
        {"status", 0     },
        {"result",  "null"}
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    std::pair<std::map<std::string, std::string>, std::string> actual =
        wrapper.set_config(R"({ "language" : "english", "level" : "C2" })");
    std::pair<std::map<std::string, std::string>, std::string> expected = {{},
                                                                           ""};

    EXPECT_EQ(expected, actual);
}

TEST(BasicPWSetConfig, Error) {
    json answer = {
        {"status", 1},
        {"result",{{"field_name", "error_type"}, {"field_name2", "error_type2"}}}
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    std::pair<std::map<std::string, std::string>, std::string> actual =
        wrapper.set_config(R"({ "language" : "english"})");
    std::pair<std::map<std::string, std::string>, std::string> expected = {
        {{"field_name", "error_type"}, {"field_name2", "error_type2"}},
        ""
    };

    EXPECT_EQ(actual, expected);
}

// TEST(BasicPWSetConfig, WrongResponseFormat) {
// }
//
// TEST(BasicPWSetConfig, Disconnect) {
// }