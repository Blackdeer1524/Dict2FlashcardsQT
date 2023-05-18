#ifndef IIMAGEPLUGINWRAPPER_H
#define IIMAGEPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"

struct IImagePluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t batch_size) = 0;
};

#endif  // IIMAGEPLUGINWRAPPER_H
