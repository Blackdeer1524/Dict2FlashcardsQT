#include "BasicPluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(BasicPWGetDefaultConfig, Output) {
    auto               memorizer = std::make_shared<Memorizer>();
    BasicPluginWrapper wrapper(memorizer, "tests");
    wrapper.get_default_config();

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "get_default_config"},
        {"plugin_type", "tests"             }
    };
    EXPECT_EQ(expected, actual);
}

TEST(BasicPWGetDefaultConfig, PartialSuccess) {
    json answer = {
        {"status", 0                                         },
        {"result", {{"language", "english"}, {"level", "C2"}}},
        {"error",  "something"                               }
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    BasicPluginWrapper           wrapper(fixed_answer, "tests");

    std::pair<json, std::string> actual = {
        json::parse(wrapper.get_default_config().first),
        wrapper.get_default_config().second};
    std::pair<json, std::string> expected = {
        {{"language", "english"}, {"level", "C2"}},
        "something"
    };

    EXPECT_EQ(expected, actual);
}

TEST(BasicPWGetDefaultConfig, Error) {
    json answer = {
        {"status",   1          },
        {"response", "null"     },
        {"error",    "something"}
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    BasicPluginWrapper                  wrapper(fixed_answer, "tests");

    std::pair<std::string, std::string> actual   = wrapper.get_default_config();
    std::pair<std::string, std::string> expected = {"", "something"};

    EXPECT_EQ(actual, expected);
}

// TEST(BasicPWGetDefaultConfig, WrongResponseFormat) {
// }
//
// TEST(BasicPWGetDefaultConfig, Disconnect) {
// }