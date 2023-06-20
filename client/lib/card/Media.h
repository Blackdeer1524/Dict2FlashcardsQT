#ifndef MEDIA_H
#define MEDIA_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct SourceWithAdditionalInfo {
    std::string src;
    std::string info;

    friend bool operator==(const SourceWithAdditionalInfo &lhs, const SourceWithAdditionalInfo &rhs) = default;
};

inline void to_json(nlohmann ::json                &nlohmann_json_j,
                    const SourceWithAdditionalInfo &nlohmann_json_t) {
    nlohmann_json_j = {nlohmann_json_t.src, nlohmann_json_t.info};
}

inline void from_json(const nlohmann ::json    &nlohmann_json_j,
                      SourceWithAdditionalInfo &nlohmann_json_t) {
    nlohmann_json_j[0].get_to(nlohmann_json_t.src);
    nlohmann_json_j[1].get_to(nlohmann_json_t.info);
}

struct Media {
    std::vector<SourceWithAdditionalInfo> local;
    std::vector<SourceWithAdditionalInfo> web;

    friend bool operator==(const Media &lhs, const Media &rhs) = default;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Media, local, web);

#endif  // !MEDIA_H