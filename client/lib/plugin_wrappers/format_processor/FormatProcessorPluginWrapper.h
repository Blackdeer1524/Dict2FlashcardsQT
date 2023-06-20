#ifndef FORMATPROCESSORPLUGINWRAPPER_H
#define FORMATPROCESSORPLUGINWRAPPER_H

#include <memory>
#include <string>

#include "BasicPluginWrapper.h"
#include "IFormatProcessorPluginWrapper.h"
#include "IRequestable.h"

class FormatProcessorPluginWrapper
    : public BasicPluginWrapper,
      virtual public IFormatProcessorPluginWrapper {
 public:
    explicit FormatProcessorPluginWrapper(
        std::shared_ptr<IRequestable> connection);
    std::string save(const std::string &cards_path) override;
};

#endif  // FORMATPROCESSORPLUGINWRAPPER_H
