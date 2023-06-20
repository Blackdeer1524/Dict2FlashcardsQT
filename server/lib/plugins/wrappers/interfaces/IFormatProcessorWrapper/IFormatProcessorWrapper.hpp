#ifndef I_FORMAT_PROCESSOR_WRAPPER_H
#define I_FORMAT_PROCESSOR_WRAPPER_H

#include "IPluginWrapper.hpp"

#include <filesystem>
#include <nlohmann/detail/macro_scope.hpp>
#include <string>

struct ResultFilesPaths {
    std::filesystem::path cards;
    // std::filesystem::path audios;
    // std::filesystem::path images;
};

inline void to_json(nlohmann ::json        &nlohmann_json_j,
                    const ResultFilesPaths &nlohmann_json_t) {
    nlohmann_json_j[0] = nlohmann_json_t.cards;
}

inline void from_json(const nlohmann ::json &nlohmann_json_j,
                      ResultFilesPaths      &nlohmann_json_t) {
    nlohmann_json_j[0].get_to(nlohmann_json_t.cards);
}

class IFormatProcessorWrapper : public virtual IPluginWrapper {
 public:
    using type = std::string;

    virtual auto save(const ResultFilesPaths &paths)
        -> std::variant<type, PyExceptionInfo> = 0;
};

#endif
