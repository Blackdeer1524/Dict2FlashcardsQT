#include <boost/asio/io_service.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include <nlohmann/json.hpp>

#include "Media.hpp"
#include "Sender.hpp"

class ImagesProvider : public ::testing::Test {
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
    ImagesProvider() : sender_(HOST, PORT) {
    }
};

// Макро потому что возможно буду тестить код без него
#define Init(sender)                                                           \
    do {                                                                       \
        const auto *request =                                                  \
            R"({"query_type": "init",)"                                        \
            R"("plugin_name": "images", "plugin_type": "images" })"            \
            "\r\n";                                                            \
        auto expected = R"*({"status": 0, "message": ""})*"_json;              \
        auto actual   = (sender).request(request);                             \
        ASSERT_EQ(expected, actual);                                           \
    } while (0)

TEST_F(ImagesProvider, Init) {
    Init(sender_);
}

TEST_F(ImagesProvider, ValidateConfig) {
    Init(sender_);

    const auto *request  = R"(
        {
            "query_type": "validate_config",
            "plugin_type": "images",
            "config": {"timeout": 1}
        })"
                           "\r\n";

    auto        expected = R"*({"status": 0, "result": {}})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(ImagesProvider, blankGet) {
    Init(sender_);

    const auto *request = R"*(
        {
            "query_type": "get",
            "plugin_type": "images",
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

TEST_F(ImagesProvider, Get) {
    Init(sender_);

    const auto *request = R"*(
        {
            "query_type": "get",
            "plugin_type": "images",
            "word": "sunshade",
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

TEST_F(ImagesProvider, ListPlugins) {
    Init(sender_);

    const auto *request = R"(
        {
            "query_type": "list_plugins",
            "plugin_type": "images"
        })"
                          "\r\n";

    auto        expected =
        R"*({"result": {"success": ["images"], "failed": []}, "status": 0})*"_json;
    auto actual = sender_.request(request);
    ASSERT_EQ(expected, actual);
}
