#ifndef IAUDIOPLUGINWRAPPER_H
#define IAUDIOPLUGINWRAPPER_H

#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"
#include "Media.h"

struct IAudioPluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::pair<Media, std::string>
    get(const std::string &word, size_t batch_size, bool restart) = 0;
};

#endif  // IAUDIOPLUGINWRAPPER_H
