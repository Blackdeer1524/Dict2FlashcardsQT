#ifndef DECK_H
#define DECK_H

#include "word_cards.h"
#include "IDeck.h"
#include "IWordPluginWrapper.h"
#include <vector>
#include <memory>
#include <iostream>

class Deck: public IDeck {
public:
    Deck(std::unique_ptr<IWordPluginWrapper> wordPlugin);
    ~Deck() {std::cout << "Destructor of deck" << std::endl;}
    size_t size() const;
    std::string getWord(size_t index) const;
    const Card* getCard(size_t index) const;
    void next(size_t index);
    void prev(size_t index);
    void load(std::string word, std::string query);

    virtual int indexOfWord(const std::string &word) const;

private:
    const size_t batch_size = 20;
    std::vector<WordCards> cards_;
    std::unique_ptr<IWordPluginWrapper> wordPlugin_;
};

#endif // DECK_H