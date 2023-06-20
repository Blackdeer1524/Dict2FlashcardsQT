#ifndef IWORDPLUGINWRAPPER_H
#define IWORDPLUGINWRAPPER_H

#include <string>

#include "Card.h"
#include "IBasicPluginWrapper.h"

class IWordPluginWrapper : virtual public IBasicPluginWrapper {
 public:
    virtual std::pair<std::vector<Card>, std::string>
                                                get(const std::string &word,
                                                    const std::string &query_language,
                                                    size_t             batch_size,
                                                    bool               restart) = 0;
    virtual std::pair<std::string, std::string> get_dict_scheme() = 0;
};

#endif  // IWORDPLUGINWRAPPER_H
