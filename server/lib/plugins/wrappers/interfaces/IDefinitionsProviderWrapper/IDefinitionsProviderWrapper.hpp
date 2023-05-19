#ifndef I_DEFINITIONS_PROVIDER_WRAPPER_H
#define I_DEFINITIONS_PROVIDER_WRAPPER_H

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#include "IPluginWrapper.hpp"

struct Card {
    Card()                                 = default;
    Card(const Card &)                     = default;
    Card(Card &&)                          = default;
    auto operator=(const Card &) -> Card & = default;
    auto operator=(Card &&) -> Card      & = default;

 public:
    std::string              word;
    std::vector<std::string> special;
    std::string              definition;
    std::vector<std::string> examples;
    std::vector<std::string> image_links;
    std::vector<std::string> audio_links;
    nlohmann::json           tags;
    nlohmann::json           other;
};

// TODO(blackdeer): solve optional keys
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Card,
                                   word,
                                   special,
                                   definition,
                                   examples,
                                   image_links,
                                   audio_links,
                                   tags
                                   /* other */);

class IDefinitionsProviderWrapper : public virtual IPluginWrapper {
 public:
    using type = std::pair<std::vector<Card>, std::string>;

    virtual auto get_dictionary_scheme()
        -> std::variant<nlohmann::json, PyExceptionInfo> = 0;

    virtual auto get(const std::string &word,
                     const std::string &filter_query,
                     uint64_t           batch_size,
                     bool               restart)
        -> std::variant<type, std::string, PyExceptionInfo> = 0;
};

#endif
