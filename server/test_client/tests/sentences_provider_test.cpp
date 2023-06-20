#include <boost/asio/io_service.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include <nlohmann/json.hpp>

#include "Sender.hpp"

class SentencesProvider : public ::testing::Test {
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
    SentencesProvider() : sender_(HOST, PORT) {
    }
};

// Макро потому что возможно буду тестить код без него
#define Init(sender)                                                           \
    do {                                                                       \
        const auto *request =                                                  \
            R"({"query_type": "init",)"                                        \
            R"("plugin_name": "sentences", "plugin_type": "sentences" })"      \
            "\r\n";                                                            \
        auto expected = R"*({"status": 0, "message": ""})*"_json;              \
        auto actual   = sender.request(request);                               \
        ASSERT_EQ(expected, actual);                                           \
    } while (0)

TEST_F(SentencesProvider, Init) {
    SetUp();
    Init(sender_);
}

TEST_F(SentencesProvider, ValidateConfig) {
    SetUp();
    Init(sender_);

    const auto *request  = R"(
        {
            "query_type": "validate_config",
            "plugin_type": "sentences",
            "config": {"timeout": 1}
        })"
                           "\r\n";

    auto        expected = R"*({"status": 0, "result": {}})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(SentencesProvider, blankGet) {
    SetUp();
    Init(sender_);

    const auto *request  = R"*(
        {
            "query_type": "get",
            "plugin_type": "sentences",
            "word": "go",
            "batch_size": 0,
            "restart": false
        })*"
                           "\r\n";

    auto        expected = R"*({"result": [[], ""], "status": 0})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(SentencesProvider, ListPlugins) {
    SetUp();
    Init(sender_);

    const auto *request = R"(
        {
            "query_type": "list_plugins",
            "plugin_type": "sentences"
        })"
                          "\r\n";

    auto        expected =
        R"*({"result": {"success": ["sentences"], "failed": []}, "status": 0})*"_json;
    auto actual = sender_.request(request);
    ASSERT_EQ(expected, actual);
}
