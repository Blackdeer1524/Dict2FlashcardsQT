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
    wrapper.save("path_1");

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type", "save"  },
        {"cards_path", "path_1"}
    };
    EXPECT_EQ(expected, actual);
}

TEST(FormatPeocessorPWSave, Success) {
    std::string answer       = R"({ "status" : 0, "message" : "null"})";
    auto        fixed_answer = std::make_shared<FixedAnswer>(answer);
    FormatProcessorPluginWrapper wrapper(fixed_answer);

    EXPECT_TRUE(wrapper.save("path_1").empty());
}

TEST(FormatPeocessorPWSave, Error) {
    std::string answer       = R"({ "status" : 1, "message" : "something" })";
    auto        fixed_answer = std::make_shared<FixedAnswer>(answer);
    FormatProcessorPluginWrapper wrapper(fixed_answer);

    std::string                  actual   = wrapper.save("path_1");
    std::string                  expected = "something";

    EXPECT_EQ(actual, expected);
}

// TEST(FormatPeocessorPWSave, WrongResponseFormat) {
// }
//
// TEST(FormatPeocessorPWSave, Disconnect) {
// }
