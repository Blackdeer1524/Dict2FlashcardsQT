#ifndef SENTENCEPLUGINWRAPPER_H
#define SENTENCEPLUGINWRAPPER_H

#include <memory>
#include <string>
#include <vector>

#include "BasicPluginWrapper.h"
#include "ISentencePluginWrapper.h"

class SentencePluginWrapper : public BasicPluginWrapper,
                              virtual public ISentencePluginWrapper {
 public:
    explicit SentencePluginWrapper(std::shared_ptr<IRequestable> connection);
    std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t batch_size, bool restart) override;
};

#endif  // SENTENCEPLUGINWRAPPER_H
