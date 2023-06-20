#ifndef PLUGIN_WRAPPER_INTERFACE_H
#define PLUGIN_WRAPPER_INTERFACE_H

#include "PyExceptionInfo.hpp"
#include <boost/asio/require_concept.hpp>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <variant>

class IPluginWrapper {
 public:
    virtual ~IPluginWrapper()                                      = default;

    [[nodiscard]] virtual auto name() const -> const std::string & = 0;
    virtual auto               load() -> std::optional<PyExceptionInfo> = 0;
    virtual auto               get_config_description()
        -> std::variant<PyExceptionInfo, nlohmann::json> = 0;
    virtual auto get_default_config()
        -> std::variant<PyExceptionInfo, nlohmann::json> = 0;
    virtual auto validate_config(nlohmann::json &&new_config)
        -> std::variant<PyExceptionInfo, nlohmann::json>    = 0;
    virtual auto unload() -> std::optional<PyExceptionInfo> = 0;
};

template <typename T>
concept is_plugin_wrapper =
    std::derived_from<T, IPluginWrapper> &&
    requires(T                            instance,
             const std::string           &name,
             const boost::python::object &module) {
        {
            T::build(name, module)
        } -> std::same_as<std::variant<T, PyExceptionInfo>>;
    };

#endif  // !PLUGIN_WRAPPER_INTERFACE_H
