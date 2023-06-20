#include "BasicPluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(BasicPWInitTest, Output) {
    auto               memorizer = std::make_shared<Memorizer>();
    BasicPluginWrapper wrapper(memorizer, "tests");
    wrapper.init("test_name");

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "init"     },
        {"plugin_type", "tests"    },
        {"plugin_name", "test_name"}
    };
    EXPECT_EQ(expected, actual);
}

TEST(BasicPWInit, Success) {
    std::string        answer       = R"({ "status" : 0, "message" : "null"})";
    auto               fixed_answer = std::make_shared<FixedAnswer>(answer);
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    EXPECT_TRUE(wrapper.init("exiting_plugin").empty());
}

TEST(BasicPWInit, Error) {
    std::string        answer = R"({ "status" : 1, "message" : "something" })";
    auto               fixed_answer = std::make_shared<FixedAnswer>(answer);
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    std::string        actual   = wrapper.init("nonexiten_plugin");
    std::string        expected = "something";

    EXPECT_EQ(actual, expected);
}

TEST(BasicPWInit, WrongResponseFormat) {
    std::string        answer = R"({ "status" : "ok", "message" : "something" })";
    auto               fixed_answer = std::make_shared<FixedAnswer>(answer);
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    std::string        actual   = wrapper.init("nonexiten_plugin");
    std::string        expected = "Wrong response format: " + answer;

    EXPECT_EQ(actual, expected);
}

TEST(BasicPWInit, Disconnect) {
    std::string answer       = R"({ "status" : "0", "message" : "something" })";
    auto        fixed_answer = std::make_shared<FixedAnswer>(answer, false);
    BasicPluginWrapper wrapper(fixed_answer, "tests");

    std::string        actual   = wrapper.init("nonexiten_plugin");
    std::string        expected = "Server disconnected";

    EXPECT_EQ(actual, expected);
}
