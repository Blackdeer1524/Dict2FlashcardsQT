#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "ImagePluginWrapper.h"
#include "Media.h"
#include "mock_classes.h"

using namespace nlohmann;

TEST(ImagePWGet, Output) {
    auto               memorizer = std::make_shared<Memorizer>();
    ImagePluginWrapper wrapper(memorizer);
    wrapper.get("test_word", 5, true);

    json actual   = json::parse(memorizer->received_message);
    json expected = {
        {"query_type",  "get"      },
        {"plugin_type", "images"   },
        {"word",        "test_word"},
        {"batch_size",  5          },
        {"restart",     true       }
    };
    EXPECT_EQ(expected, actual);
}

//TEST(ImagePWGet, FullSuccess) {
//    json answer = {
//        {"status",  0                                          },
//        {"result",  json::array({"link_1", "link_2", "link_3"})},
//        {"message", ""                                         }
//    };
//    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
//    ImagePluginWrapper            wrapper(fixed_answer);
//
//    std::pair<Media, std::string> actual   = wrapper.get("test_word", 3, true);
//    std::pair<Media, std::string> expected = {
//        {std::vector<SourceWithAdditionalInfo>{
//             {"link_1", "info_1"}, {"link_2", "info_2"}, {"link_3", "info_3"}},
//         std::vector<SourceWithAdditionalInfo>{
//             {"link_4", "info_4"}, {"link_5", "info_5"}, {"link_6", "info_6"}}},
//        ""
//    };
//    EXPECT_EQ(expected, actual);
//}
//
//TEST(ImagePWGet, PartialSuccess) {
//    //    json answer = {
//    //        {"status", 0},
//    //        {"result",
//    //         json::object({"web",
//    //          json::array({{"link_1", "info_1"},
//    //                       {"link_2", "info_2"},
//    //                       {"link_3", "info_3"}})}),
//    //         {"local",
//    //          json::array({{"link_4", "info_4"},
//    //                       {"link_5", "info_5"},
//    //                       {"link_6", "info_6"}})}},
//    //        {"message", "something"}
//    //    };
//    std::string        answer       = R"({
//        "status": 0,
//        "result": {
//            "web": [
//                ["link_1", "info_1"],
//                ["link_2", "info_2"],
//                ["link_3", "info_3"]
//            ],
//            "local": [
//                ["link_4", "info_4"],
//                ["link_5", "info_5"],
//                ["link_6", "info_6"]]
//        },
//        "message": "something"
//        })";
//    auto               fixed_answer = std::make_shared<FixedAnswer>(answer);
//    ImagePluginWrapper wrapper(fixed_answer);
//
//    std::pair<Media, std::string> actual   = wrapper.get("test_word", 3, true);
//    std::pair<Media, std::string> expected = {
//        {std::vector<SourceWithAdditionalInfo>{
//             {"link_1", "info_1"}, {"link_2", "info_2"}, {"link_3", "info_3"}},
//         std::vector<SourceWithAdditionalInfo>{
//             {"link_4", "info_4"}, {"link_5", "info_5"}, {"link_6", "info_6"}}},
//        ""
//    };
//    EXPECT_EQ(expected, actual);
//}

TEST(ImagePWGet, Error) {
    json answer = {
        {"status",  1          },
        {"result",  "null"     },
        {"message", "something"}
    };
    auto fixed_answer = std::make_shared<FixedAnswer>(answer.dump());
    ImagePluginWrapper            wrapper(fixed_answer);

    std::pair<Media, std::string> actual   = wrapper.get("test_word", 3, true);
    std::pair<Media, std::string> expected = {{}, "something"};
    EXPECT_EQ(expected, actual);
}

// TEST(ImagePWGet, WrongResponseFormat) {
// }
//
// TEST(ImagePWGet, Disconnect) {
// }
