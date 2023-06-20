#include "Card.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using namespace nlohmann;

void traverse_tags(const json        &tags,
                   const std::string &prefix,
                   std::string       &result) {
    if (tags.is_object()) {
        for (auto &tag : tags.items()) {
            std::string new_prefix =
                prefix.empty() ? tag.key() : prefix + "::" + tag.key();
            traverse_tags(tag.value(), new_prefix, result);
        }
        return;
    }
    if (tags.is_array()) {
        for (auto &value : tags) {
            traverse_tags(value, prefix, result);
        }
        return;
    }
    result += (prefix.empty() ? "" : prefix + "::") + tags.get<std::string>() + ' ';
}

std::string parse_tags(const json &tags) {
    std::string result;
    traverse_tags(tags, "", result);
    if (result.back() == ' ') {
        result.pop_back();
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << json(card).dump(2);
    return os;
}

std::pair<std::vector<Card>, std::string> load_cards(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return {{}, "Can't open the file"};
    }
    std::stringstream buffer;
    if (buffer << file.rdbuf()) {
        try {
            return {json::parse(buffer.str()).get<std::vector<Card>>(), ""};
        } catch (...) {
            return {{}, "Wrong data format"};
        }
    }
    return {{}, "Can't read from file"};
}

std::string save_cards(const std::vector<Card> &cards,
                       const std::string       &path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return "Can't open the file";
    }
    std::string cards_dump = json(cards).dump();
    if (file << cards_dump) {
        return "";
    }
    return "Can't write into file";
}