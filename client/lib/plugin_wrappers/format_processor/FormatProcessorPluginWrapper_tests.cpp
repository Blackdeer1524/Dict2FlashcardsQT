#include "FormatProcessorPluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(FormatPeocessorPWSave, Output) {
    auto                         memorizer = std::make_shared<Memorizer>();
    FormatProcessorPluginWrapper wrapper(memorizer);
    wrapper.save("path_1", "path_2", "path_3");

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type", "save"  },
        {"cards_path", "path_1"},
        {"links_path", "path_2"},
        {"media_path", "path_3"},
    };
    EXPECT_EQ(expected, actual);
}

TEST(FormatPeocessorPWSave, Success) {
    std::string answer       = R"({ "status" : 0, "error" : "null"})";
    auto        fixed_answer = std::make_shared<FixedAnswer>(answer);
    FormatProcessorPluginWrapper wrapper(fixed_answer);

    EXPECT_TRUE(wrapper.save("path_1", "path_2", "path_3").empty());
}

TEST(FormatPeocessorPWSave, Error) {
    std::string answer       = R"({ "status" : 1, "error" : "something" })";
    auto        fixed_answer = std::make_shared<FixedAnswer>(answer);
    FormatProcessorPluginWrapper wrapper(fixed_answer);

    std::string actual   = wrapper.save("path_1", "path_2", "path_3");
    std::string expected = "something";

    EXPECT_EQ(actual, expected);
}

// TEST(FormatPeocessorPWSave, WrongResponseFormat) {
// }
//
// TEST(FormatPeocessorPWSave, Disconnect) {
// }