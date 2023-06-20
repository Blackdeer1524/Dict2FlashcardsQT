#include "Card.h"

#include <string>
#include <fstream>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(ParseTags, JSON_to_STR) {
    json        tags     = json::parse(R"({ "pos" : "noun",
                                "languages" : ["Russian", "English"],
                                "very_deep_tag" : { "not_the_end": "value" }
                            })");

    std::string actual   = parse_tags(tags);
    std::string expected = "languages::Russian languages::English pos::noun "
                           "very_deep_tag::not_the_end::value";
    EXPECT_EQ(expected, actual);
}

TEST(ParseTags, STR_to_STR) {
    json tags = json::parse(
        R"({ "tags" : "languages::Russian languages::English pos::noun very_deep_tag::not_the_end::value" })");

    std::string actual   = parse_tags(tags.at("tags"));
    std::string expected = "languages::Russian languages::English pos::noun "
                           "very_deep_tag::not_the_end::value";
    EXPECT_EQ(expected, actual);
}

//TEST(LOAD_SAVE, SuccessfulSaveLoad) {
//    std::vector<Card> cards = {
//        {"go",
//         {"special_1", "special_2"},
//         "move", {"go there", "go here"},
//         {{"image_link_1", "image_link_2"}, {"image_link_3", "image_link_4"}},
//         {{"audio_link_1", "audio_link_2"}, {"audio_link_1", "audio_link_2"}},
//         "pos::verb",
//         "other"},
//        {"go2",
//         {"special_3", "special_4"},
//         "move", {"go there", "go here"},
//         {"image_link_3", "image_link_4"},
//         {"audio_link_3", "audio_link_4"},
//         "pos::verb",
//         "other"}
//    };
//    std::string path        = "test.json";
//
//    std::string save_result = save_cards(cards, path);
//    EXPECT_EQ("", save_result);
//    std::pair<std::vector<Card>, std::string> load_result = load_cards(path);
//    EXPECT_EQ("", load_result.second);
//
//    EXPECT_EQ(cards, load_result.first);
//}

TEST(LOAD_SAVE, WrognFormat) {
    std::string data        = R"({"word": "go"})";
    std::string path        = "test.json";

    std::ofstream file(path);
    file << data;
    file.close();
    std::pair<std::vector<Card>, std::string> load_result = load_cards(path);
    EXPECT_EQ("Wrong data format", load_result.second);
}
