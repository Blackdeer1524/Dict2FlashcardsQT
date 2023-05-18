#ifndef MOCK_CLASSES_H
#define MOCK_CLASSES_H

#include "IRequestable.h"

#include <string>

struct Memorizer : public IRequestable {
    std::pair<bool, std::string> request(const std::string &message) override {
        received_message = message;
        return std::make_pair(true, "");
    }

    std::string received_message;
};

struct FixedAnswer : public IRequestable {
    explicit FixedAnswer(std::string answer, bool connected = true)
        : answer(std::move(answer)), connected(connected){};

    std::pair<bool, std::string> request(const std::string &message) override {
        return std::make_pair(connected, answer);
    }

    bool        connected;
    std::string answer;
};

#endif  // MOCK_CLASSES_H
