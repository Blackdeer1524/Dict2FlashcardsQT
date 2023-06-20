#ifndef IIMAGEPLUGINWRAPPER_H
#define IIMAGEPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"
#include "Media.h"

struct IImagePluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<Media, std::string>
    get(const std::string &word, size_t batch_size, bool restart) = 0;
};

#endif  // IIMAGEPLUGINWRAPPER_H
