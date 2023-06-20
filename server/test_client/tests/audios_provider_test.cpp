#include <boost/asio/io_service.hpp>
#include <deque>
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include <nlohmann/json.hpp>
#include <string>

#include "Media.hpp"
#include "Sender.hpp"

class AudiosProvider : public ::testing::Test {
 protected:
    static constexpr auto HOST = "127.0.0.1";
    static constexpr auto PORT = 8888;

    auto                  SetUp() -> void override {
        boost::asio::io_service      ios;
        boost::asio::ip::tcp::socket socket(ios);
        std::construct_at<Sender>(&sender_, HOST, PORT);
    }

    Sender sender_;

 public:
    AudiosProvider() : sender_(HOST, PORT) {
    }
};

// Макро потому что возможно буду тестить код без него
#define Init(sender)                                                           \
    do {                                                                       \
        const auto *request =                                                  \
            R"({"query_type": "init",)"                                        \
            R"("plugin_name": "audios", "plugin_type": "audios" })"            \
            "\r\n";                                                            \
        auto expected = R"*({"status": 0, "message": ""})*"_json;              \
        auto actual   = (sender).request(request);                             \
        ASSERT_EQ(expected, actual);                                           \
    } while (0)

TEST_F(AudiosProvider, Init) {
    Init(sender_);
}

TEST_F(AudiosProvider, ValidateConfig) {
    Init(sender_);

    const auto *request  = R"(
        {
            "query_type": "validate_config",
            "plugin_type": "audios",
            "config": {"audio region": "uk", "timeout": 1}
        })"
                           "\r\n";

    auto        expected = R"*({"status": 0, "result": {}})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(AudiosProvider, blankGet) {
    Init(sender_);

    const auto *request = R"*(
        {
            "query_type": "get",
            "plugin_type": "audios",
            "word": "sunshade",
            "filter": "",
            "batch_size": 0,
            "restart": false
        })*"
                          "\r\n";

    auto        expected =
        R"*({"result": [{"local": [], "web": []}, ""], "status": 0})*"_json;
    auto actual = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(AudiosProvider, Get) {
    Init(sender_);

    const auto *request = R"*(
        {
            "query_type": "get",
            "plugin_type": "audios",
            "word": "sunshade",
            "filter": "",
            "batch_size": 5,
            "restart": false
        })*"
                          "\r\n";

    auto        actual  = sender_.request(request);
    ASSERT_EQ(actual["status"], 0);
    ASSERT_FALSE(actual["result"][1].empty());
    Media links = actual["result"][0];
    ASSERT_LE(links.web.size(), 5);
}

TEST_F(AudiosProvider, ListPlugins) {
    Init(sender_);

    const auto *request = R"(
        {
            "query_type": "list_plugins",
            "plugin_type": "audios"
        })"
                          "\r\n";

    auto        expected =
        R"*({"result": {"success": ["audios"], "failed": []}, "status": 0})*"_json;
    auto actual = sender_.request(request);
    ASSERT_EQ(expected, actual);
}
