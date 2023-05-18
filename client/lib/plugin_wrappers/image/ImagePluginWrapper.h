#ifndef IMAGEPLUGINWRAPPER_H
#define IMAGEPLUGINWRAPPER_H

#include <memory>
#include <string>
#include <vector>

#include "IImagePluginWrapper.h"
#include "BasicPluginWrapper.h"

class ImagePluginWrapper : public BasicPluginWrapper,
                           virtual public IImagePluginWrapper {
 public:
    explicit ImagePluginWrapper(std::shared_ptr<IRequestable> connection);
    std::pair<std::vector<std::string>, std::string>
    get(const std::string &word, size_t batch_size) override;
};

#endif  // IMAGEPLUGINWRAPPER_H
