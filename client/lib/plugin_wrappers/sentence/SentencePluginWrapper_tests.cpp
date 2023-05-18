#include "SentencePluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(SentensePWGet, Output) {
    auto                  memorizer = std::make_shared<Memorizer>();
    SentencePluginWrapper wrapper(memorizer);
    wrapper.get("test_word", 5);

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "get"      },
        {"plugin_type", "sentences"},
        {"word",        "test_word"},
        {"batch_size",  5          },
    };
    EXPECT_EQ(expected, actual);
}

TEST(SentensePWGet, PartialSuccess) {
    json answer = {
        {"status", 0                                                      },
        {"result", json::array({"sentence_1", "sentence_2", "sentence_3"})},
        {"error",  "something"                                            }
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    SentencePluginWrapper                            wrapper(fixed_answer);

    std::pair<std::vector<std::string>, std::string> actual =
        wrapper.get("test_word", 3);
    std::pair<std::vector<std::string>, std::string> expected = {
        std::vector<std::string>{"sentence_1", "sentence_2", "sentence_3"},
        "something"
    };
    EXPECT_EQ(expected, actual);
}

// TEST(SentencePWGet, FullSuccess) {
// }
//
//  TEST(ImagePWGet, WrongResponseFormat) {
//  }
//
//  TEST(ImagePWGet, Disconnect) {
//  }