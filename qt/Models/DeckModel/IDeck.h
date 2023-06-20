#ifndef IDECK_H
#define IDECK_H

#include <string>
#include "Card.h"

class IDeck {
public:
    virtual ~IDeck() = default;
    virtual size_t size() const = 0;
    virtual std::string getWord(size_t index) const = 0;
    virtual const Card* getCard(size_t index) const = 0;
    virtual void next(size_t index) = 0;
    virtual void prev(size_t index) = 0;
    virtual void load(std::string word, std::string query) = 0;
    virtual int indexOfWord(const std::string &word) const = 0;
};

#endif // IDECK_H
