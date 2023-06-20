#include "Deck.hpp"
#include "word_cards.h"
#include <iostream>

Deck::Deck(std::unique_ptr<IWordPluginWrapper> wordPlugin) {
    wordPlugin_ = std::move(wordPlugin);
    wordPlugin_->init("definitions");
}

size_t Deck::size() const {
    return cards_.size();
}
std::string Deck::getWord(size_t index) const {
    return cards_.at(index).get_word();
}
const Card* Deck::getCard(size_t index) const {
    return cards_.at(index).get_card();
}

void Deck::next(size_t index) {
    cards_.at(index).next();
}

void Deck::prev(size_t index) {
    cards_.at(index).prev();
}

void Deck::load(std::string word, std::string query) {
    std::vector<Card> wordCards;
    std::string error;
    tie(wordCards, error) = wordPlugin_->get(word, query, batch_size, false);
    int i = indexOfWord(word);
    if (i == -1) {
        if (!wordCards.empty()) {
            cards_.push_back(WordCards(word, wordCards));
        }
    }
    else {
        cards_.at(i).addCards(wordCards);
    }
}

int Deck::indexOfWord(const std::string &word) const {
    for (int i = 0; i < cards_.size(); ++i) {
        if (cards_.at(i).get_word() == word) {
            return i;
        }
    }
    return -1;
}
