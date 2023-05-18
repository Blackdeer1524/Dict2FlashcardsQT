#include <memory>
#include <string>
#include <vector>

#include "Card.h"
#include "WordPluginWrapper.h"
#include "mock_classes.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(WordPWGet, Output) {
    auto              memorizer = std::make_shared<Memorizer>();
    WordPluginWrapper wrapper(memorizer);
    wrapper.get("test_word", "pos::noun", 3, true);

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "get"      },
        {"plugin_type", "word"     },
        {"word",        "test_word"},
        {"filter",      "pos::noun"},
        {"batch_size",  3          },
        {"restart",     true       }
    };
    EXPECT_EQ(expected, actual);
}

TEST(WordPWGet, PartialSuccess) {
    std::string answer =
        R"({"status":0,
            "result":[{
                "word":"go",
                "special":["something special"],
                "definition":"move",
                "examples":["go somewhere"],
                "image_links":[],
                "audio_links":[],
                "tags":{"tag":"tag"}
            }],
            "error":"something"})";
    auto              fixed_answer = std::make_shared<FixedAnswer>(answer);
    WordPluginWrapper wrapper(fixed_answer);

    std::pair<std::vector<Card>, std::string> actual =
        wrapper.get("go", "pos::noun", 1, true);
    std::pair<std::vector<Card>, std::string> expected = {
        {Card{"go",
              {"something special"},
              "move",
              {"go somewhere"},
              std::vector<std::string>(),
              std::vector<std::string>(),
              "tag::tag"}},
        "something"};
    EXPECT_EQ(expected, actual);
}

TEST(WordPWGet, Error) {
    std::string answer =
        R"({"status":1,
            "result":"null",
            "error":"something"})";
    auto              fixed_answer = std::make_shared<FixedAnswer>(answer);
    WordPluginWrapper wrapper(fixed_answer);

    std::pair<std::vector<Card>, std::string> actual =
        wrapper.get("go", "pos::noun", 1, true);
    std::pair<std::vector<Card>, std::string> expected = {{}, "something"};
    EXPECT_EQ(expected, actual);
}

// TEST(WordPWGet, FullSuccess) {
// }
//
// TEST(WordPWGet, WrongResponseFormat) {
// }
//
// TEST(WordPWGet, Disconnect) {
// }

// TEST(WordPWOutputTest, GetDictScheme) {
//     auto              memorizer = std::make_shared<Memorizer>();
//     WordPluginWrapper wrapper(memorizer);
//     EXPECT_THROW(wrapper.get_dict_scheme(), std::runtime_error);
//
//     json actual   = json::parse(memorizer->received_message);
//     json expected = json::parse(
//         R"({ "query_type" : "get_dict_scheme" , "plugin_type" : "word" })");
//     EXPECT_EQ(expected, actual);
// }
// TEST(WordPWInputTest, GetDefaultConfigSuccess) {
//     std::string answer =
//         R"({ "status" : 0, "result" : { "field_1" : "value_1", "field_2" :
//         "value_2" }})";
//     auto              fixed_answer = std::make_shared<FixedAnswer>(answer);
//     WordPluginWrapper wrapper(fixed_answer);
//
//     json              actual = json::parse(wrapper.get_dict_scheme());
//     json              expected =
//         json::parse(R"({ "field_1" : "value_1", "field_2" : "value_2" })");
//     EXPECT_EQ(expected, actual);
// }
//
// TEST(WordPWInputTest, GetDefaultConfigFailure) {
//     std::string       answer       = R"({ "status" : 1, "result" : "null"
//     })"; auto              fixed_answer =
//     std::make_shared<FixedAnswer>(answer); WordPluginWrapper
//     wrapper(fixed_answer);
//
//     EXPECT_THROW(wrapper.get_dict_scheme(), std::runtime_error);
// }