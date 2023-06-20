#ifndef IMAGEPLUGINWRAPPER_H
#define IMAGEPLUGINWRAPPER_H

#include <memory>
#include <string>
#include <vector>

#include "IImagePluginWrapper.h"
#include "BasicPluginWrapper.h"
#include "Media.h"

class ImagePluginWrapper : public BasicPluginWrapper,
                           virtual public IImagePluginWrapper {
 public:
    explicit ImagePluginWrapper(std::shared_ptr<IRequestable> connection);
    std::pair<Media, std::string>
    get(const std::string &word, size_t batch_size, bool restart) override;
};

#endif  // IMAGEPLUGINWRAPPER_H
