#ifndef CARD_H
#define CARD_H

#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#include "Media.h"

struct Card {
    Card()                                                   = default;
    Card(const Card &)                                       = default;
    Card(Card &&)                                            = default;
    Card       &operator=(const Card &)                      = default;
    Card       &operator=(Card &&)                           = default;

    friend bool operator==(const Card &lhs, const Card &rhs) = default;

    std::string word;
    std::vector<std::string> special;
    std::string              definition;
    std::vector<std::string> examples;
    Media                    audios;
    Media                    images;
    nlohmann::json           tags;
    nlohmann::json           other;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Card,
                                   word,
                                   special,
                                   definition,
                                   examples,
                                   images,
                                   audios,
                                   tags,
                                   other);

std::ostream &operator<<(std::ostream &os, const Card &card);

void          traverse_tags(const nlohmann::json &tags,
                            const std::string    &prefix,
                            std::string          &result);
std::string   parse_tags(const nlohmann::json &tags);

std::pair<std::vector<Card>, std::string> load_cards(const std::string &path);
std::string save_cards(const std::vector<Card> &cards, const std::string &path);

#endif  // CARD_H
