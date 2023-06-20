#ifndef IFORMATPROCESSORPLUGINWRAPPER_H
#define IFORMATPROCESSORPLUGINWRAPPER_H

#include <string>

#include "IBasicPluginWrapper.h"

struct IFormatProcessorPluginWrapper : virtual public IBasicPluginWrapper {
    virtual std::string save(const std::string &cards_path) = 0;
};

#endif  // IFORMATPROCESSORPLUGINWRAPPER_H
