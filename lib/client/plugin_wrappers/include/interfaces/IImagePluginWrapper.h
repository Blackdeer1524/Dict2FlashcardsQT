#ifndef IIMAGEPLUGINWRAPPER_H
#define IIMAGEPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "interfaces/IBasicPluginWrapper.h"

struct IImagePluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t count) = 0;
};

#endif  // IIMAGEPLUGINWRAPPER_H