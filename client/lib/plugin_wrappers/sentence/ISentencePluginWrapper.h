#ifndef ISENTENCEPLUGINWRAPPER_H
#define ISENTENCEPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"

struct ISentencePluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t batch_size, bool restart) = 0;
};

#endif  // ISENTENCEPLUGINWRAPPER_H
