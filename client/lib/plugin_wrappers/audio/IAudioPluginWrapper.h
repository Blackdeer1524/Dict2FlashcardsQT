#ifndef IAUDIOPLUGINWRAPPER_H
#define IAUDIOPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"

struct IAudioPluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<std::vector<std::pair<std::string, std::string>>,
                      std::string>
    get(const std::string &word, size_t batch_size) = 0;
};

#endif  // IAUDIOPLUGINWRAPPER_H
