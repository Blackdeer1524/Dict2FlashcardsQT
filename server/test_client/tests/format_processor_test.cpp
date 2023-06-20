#include <boost/asio/io_service.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include <nlohmann/json.hpp>

#include "Sender.hpp"

class FormatProcessor : public ::testing::Test {
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
    FormatProcessor() : sender_(HOST, PORT) {
    }
};

// Макро потому что возможно буду тестить код без него
#define Init(sender)                                                           \
    do {                                                                       \
        const auto *request =                                                  \
            R"({"query_type": "init",)"                                        \
            R"("plugin_name": "processor", "plugin_type": "format" })"         \
            "\r\n";                                                            \
        auto expected = R"*({"status": 0, "message": ""})*"_json;              \
        auto actual   = sender.request(request);                               \
        ASSERT_EQ(expected, actual);                                           \
    } while (0)

TEST_F(FormatProcessor, Init) {
    Init(sender_);
}

TEST_F(FormatProcessor, ValidateConfig) {
    Init(sender_);

    const auto *request  = R"(
        {
            "query_type": "validate_config",
            "plugin_type": "format",
            "config": {}
        })"
                           "\r\n";

    auto        expected = R"*({"status": 0, "result": {}})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(FormatProcessor, Save) {
    Init(sender_);

    const auto *request  = R"(
    {
        "query_type": "save",
        "cards_path":
        "/home/blackdeer/projects/cpp/technopark/server/test_client/deck.json"
    })"
                           "\r\n";

    auto        expected = R"*({"message": "", "status": 0})*"_json;
    auto        actual   = sender_.request(request);
    ASSERT_EQ(expected, actual);
}

TEST_F(FormatProcessor, ListPlugins) {
    Init(sender_);

    const auto *request = R"(
        {
            "query_type": "list_plugins",
            "plugin_type": "format"
        })"
                          "\r\n";

    auto        expected =
        R"*({"result": {"success": ["processor"], "failed": []}, "status": 0})*"_json;
    auto actual = sender_.request(request);
    ASSERT_EQ(expected, actual);
}
