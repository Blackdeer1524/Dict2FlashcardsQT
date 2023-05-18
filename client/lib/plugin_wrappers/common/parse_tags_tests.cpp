#include "Card.h"

#include <string>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(ParseTags, TEST_1) {
    json        tags     = json::parse(R"({ "pos" : "noun",
                                "languages" : ["Russian", "English"],
                                "very_deep_tag" : { "not_the_end": "value" }
                            })");

    std::string actual   = parse_tags(tags);
    std::string expected = "languages::Russian languages::English pos::noun "
                           "very_deep_tag::not_the_end::value";
    EXPECT_EQ(expected, actual);
}