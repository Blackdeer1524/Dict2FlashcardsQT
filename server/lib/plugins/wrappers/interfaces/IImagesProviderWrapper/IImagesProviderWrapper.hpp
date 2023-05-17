#ifndef I_IMAGES_PROVIDER_WRAPPER_H
#define I_IMAGES_PROVIDER_WRAPPER_H

#include <string>
#include <utility>
#include <vector>

#include "IPluginWrapper.hpp"

class IImagesProviderWrapper : public virtual IPluginWrapper {
 public:
    using type = std::pair<std::vector<std::string>, std::string>;

    virtual auto get(const std::string &word, uint64_t batch_size, bool restart)
        -> std::variant<type, std::string, PyExceptionInfo> = 0;
};

#endif  // !I_AUDIOS_PROVIDER_WRAPPER
