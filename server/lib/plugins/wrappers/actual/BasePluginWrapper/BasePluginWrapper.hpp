#ifndef BASE_PLUGIN_WRAPPER_H
#define BASE_PLUGIN_WRAPPER_H

#include <boost/python/errors.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/object_fwd.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <variant>

#include "IPluginWrapper.hpp"
#include "PyExceptionInfo.hpp"

class BasePluginWrapper : public virtual IPluginWrapper {
 public:
    BasePluginWrapper(const BasePluginWrapper &)                     = default;
    BasePluginWrapper(BasePluginWrapper &&)                          = default;
    auto operator=(const BasePluginWrapper &) -> BasePluginWrapper & = delete;
    auto operator=(BasePluginWrapper &&) -> BasePluginWrapper      & = default;
    ~BasePluginWrapper() override                                    = default;

    static auto build(const std::string           &name,
                      const boost::python::object &module)
        -> std::variant<BasePluginWrapper, PyExceptionInfo>;

    [[nodiscard]] auto name() const -> const std::string & override;
    auto               load() -> std::optional<PyExceptionInfo> override;
    auto               unload() -> std::optional<PyExceptionInfo> override;
    auto               get_config_description()
        -> std::variant<PyExceptionInfo, nlohmann::json> override;
    auto get_default_config()
        -> std::variant<PyExceptionInfo, nlohmann::json> override;
    auto validate_config(nlohmann::json &&new_config)
        -> std::variant<PyExceptionInfo, nlohmann::json> override;

 protected:
    struct CommonFunctions {
        static auto build(const boost::python::object &module)
            -> std::variant<CommonFunctions, PyExceptionInfo>;

        boost::python::object load;
        boost::python::object get_config_description;
        boost::python::object validate_config;
        boost::python::object get_default_config;
        boost::python::object unload;
    };

    BasePluginWrapper(const std::string &name, const CommonFunctions &common);

    std::string     name_;
    CommonFunctions common_;
    nlohmann::json  config_;
};

static_assert(is_plugin_wrapper<BasePluginWrapper>);

#endif  // !BASE_PLUGIN_WRAPPER_H
