#ifndef WORD_CARDS_H
#define WORD_CARDS_H

#include <string>
#include <vector>
#include "Card.h"

class WordCards {
public:
    explicit WordCards(const std::string& word);
    WordCards(const std::string& word, std::vector<Card> cards);
    size_t size() const;
    std::string get_word() const;
    const Card* get_card() const; // pointer invalidation is impossible
    void next();
    void prev();
    void addCards(std::vector<Card> cards);
    void addCard(Card card);

private:
    std::string word_;
    std::vector<Card> cards_;
    size_t pos_;
};

#endif // WORD_CARDS_H
