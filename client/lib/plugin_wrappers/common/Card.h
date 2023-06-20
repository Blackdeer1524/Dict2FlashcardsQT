#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

struct Card {
    std::string              word;
    std::vector<std::string> special;
    std::string              definition;
    std::vector<std::string> examples;
    std::vector<std::string> image_links;
    std::vector<std::string> audio_links;
    nlohmann::json           tags;
    nlohmann::json           other;
};

inline void from_json(const nlohmann::json &j, Card &card) {
    j.at("word").get_to(card.word);
    j.at("special").get_to(card.special);
    j.at("definition").get_to(card.definition);
    j.at("examples").get_to(card.examples);
    j.at("image_links").get_to(card.image_links);
    j.at("audio_links").get_to(card.audio_links);
    j.at("tags").get_to(card.tags);
    j.at("other").get_to(card.other);
}

inline bool operator==(const Card &lhs, const Card &rhs) {
    return lhs.word == rhs.word && lhs.special == rhs.special &&
           lhs.definition == rhs.definition && lhs.examples == rhs.examples &&
           lhs.image_links == rhs.image_links &&
           lhs.audio_links == rhs.audio_links && lhs.tags == rhs.tags &&
           lhs.other == rhs.other;
}

#endif  // CARD_H
