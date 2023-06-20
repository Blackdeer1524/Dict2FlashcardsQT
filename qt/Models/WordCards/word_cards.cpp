#include "word_cards.h"

WordCards::WordCards(const std::string& word)
    : word_(word)
    , pos_(0)
{
}

WordCards::WordCards(const std::string& word, std::vector<Card> cards) 
    : word_(word)
    , pos_(0)
    , cards_(cards)
{
}

size_t WordCards::size() const
{
    return cards_.size();
}

std::string WordCards::get_word() const
{
    return word_;
}

const Card* WordCards::get_card() const
{
    if (pos_ < 0 || pos_ >= size())
    {
        return nullptr;
    }
    return &cards_[pos_];
}

void WordCards::next()
{
    if (pos_ == size() - 1)
    {
        return;
    }
    ++pos_;
}

void WordCards::prev()
{
    if (pos_ == 0)
    {
        return;
    }
    --pos_;
}

void WordCards::addCard(Card card)
{
    cards_.push_back(card);
}

void WordCards::addCards(std::vector<Card> cards) {
    for (const Card& card: cards) {
        addCard(card);
    }
}