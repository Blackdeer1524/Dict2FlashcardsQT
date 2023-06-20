#ifndef I_AUDIOS_PROVIDER_WRAPPER_H
#define I_AUDIOS_PROVIDER_WRAPPER_H

#include <nlohmann/detail/macro_scope.hpp>
#include <string>
#include <utility>
#include <vector>

#include "IPluginWrapper.hpp"
#include "Media.hpp"

// TODO(blackdeer): REWORK RETURN: ADD SUPPORT FOR LOCAL AND WEB MEDIA
class IAudiosProviderWrapper : public virtual IPluginWrapper {
 public:
    using type = std::pair<Media, std::string>;

    virtual auto get(const std::string &word, uint64_t batch_size, bool restart)
        -> std::variant<type, std::string, PyExceptionInfo> = 0;
};

#endif  // !I_AUDIOS_PROVIDER_WRAPPER
