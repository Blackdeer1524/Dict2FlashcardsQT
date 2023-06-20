#ifndef WORDPLUGINWRAPPER_H
#define WORDPLUGINWRAPPER_H

#include <memory>
#include <string>
#include <vector>

#include "BasicPluginWrapper.h"
#include "Card.h"
#include "IRequestable.h"
#include "IWordPluginWrapper.h"

class WordPluginWrapper : public BasicPluginWrapper,
                          virtual public IWordPluginWrapper {
 public:
    explicit WordPluginWrapper(std::shared_ptr<IRequestable> connection);
    std::pair<std::vector<Card>, std::string>
                                        get(const std::string &word,
                                            const std::string &query_language,
                                            size_t             batch_size,
                                            bool               restart) override;
    std::pair<std::string, std::string> get_dict_scheme() override;
};

#endif  // WORDPLUGINWRAPPER_H
