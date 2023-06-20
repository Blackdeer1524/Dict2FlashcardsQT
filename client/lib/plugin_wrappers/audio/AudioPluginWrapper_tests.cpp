#include "AudioPluginWrapper.h"
#include "mock_classes.h"

#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

TEST(AudioPWGet, Output) {
    auto               memorizer = std::make_shared<Memorizer>();
    AudioPluginWrapper wrapper(memorizer);
    wrapper.get("test_word", 5, true);

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "get"      },
        {"plugin_type", "audios"   },
        {"word",        "test_word"},
        {"batch_size",  5          },
        {"restart",     true       }
    };
    EXPECT_EQ(expected, actual);
}

// TEST(AudioPWGet, PartialSuccess) {
//     json answer = {
//         {"status",  0                       },
//         {"result",
//          json::array({{"link_1", "info_1"},
//                       {"link_2", "info_2"},
//                       {"link_3", "info_3"}})},
//         {"message", "something"             }
//     };
//     //    std::string answer = R"({ "status":0,
//     //
//     "result":[{"link_1":"info_1"},{"link_2":"info_2"},{"link_3":"info_3"}],
//     //    "message":"null"})";
//     auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
//     AudioPluginWrapper                   wrapper(fixed_answer);
//
//     std::pair<audio_vector, std::string> actual =
//         wrapper.get("test_word", 3, true);
//     std::pair<audio_vector, std::string> expected = {
//         audio_vector{
//                      {"link_1", "info_1"}, {"link_2", "info_2"}, {"link_3",
//                      "info_3"}},
//         "something"
//     };
//     EXPECT_EQ(expected, actual);
// }

TEST(AudioPWGet, Error) {
    json answer = {
        {"status",  1          },
        {"result",  "null"     },
        {"message", "something"}
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    AudioPluginWrapper            wrapper(fixed_answer);

    std::pair<Media, std::string> actual   = wrapper.get("test_word", 3, true);
    std::pair<Media, std::string> expected = {{}, "something"};
    EXPECT_EQ(expected, actual);
}

// TEST(AudioPWGet, WrongResponseFormat) {
// }
//
// TEST(AudioPWGet, Disconnect) {
// }
