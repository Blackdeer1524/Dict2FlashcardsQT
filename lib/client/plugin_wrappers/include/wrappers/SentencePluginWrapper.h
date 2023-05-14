#ifndef SENTENCEPLUGINWRAPPER_H
#define SENTENCEPLUGINWRAPPER_H

#include <memory>
#include <string>
#include <vector>

#include "interfaces/ISentencePluginWrapper.h"
#include "wrappers/BasicPluginWrapper.h"

class SentencePluginWrapper : public BasicPluginWrapper,
                              virtual public ISentencePluginWrapper {
 public:
    SentencePluginWrapper(std::shared_ptr<IRequestable> connection);
    std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t batch_size) override;
};

#endif  // SENTENCEPLUGINWRAPPER_H