#include "BasePluginWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include "spdlog/spdlog.h"
#include <boost/python/import.hpp>
#include <variant>

auto BasePluginWrapper::CommonFunctions::build(
    const boost::python::object &module)
    -> std::variant<CommonFunctions, PyExceptionInfo> {
    auto plugin_container = CommonFunctions();
    try {
        plugin_container.load = module.attr("load");
        plugin_container.get_config_description =
            module.attr("get_config_description");
        plugin_container.validate_config    = module.attr("validate_config");
        plugin_container.get_default_config = module.attr("get_default_config");
        plugin_container.unload             = module.attr("unload");
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return plugin_container;
}

BasePluginWrapper::BasePluginWrapper(const std::string     &name,
                                     const CommonFunctions &common)
    : name_(name), common_(common) {
}

auto BasePluginWrapper::build(const std::string           &name,
                              const boost::python::object &module)
    -> std::variant<BasePluginWrapper, PyExceptionInfo> {
    auto common_or_error = CommonFunctions::build(module);
    if (std::holds_alternative<PyExceptionInfo>(common_or_error)) {
        auto info = std::get<PyExceptionInfo>(common_or_error);
        return info;
    }
    auto common = std::get<CommonFunctions>(common_or_error);
    return BasePluginWrapper(name, common);
}

[[nodiscard]] auto BasePluginWrapper::name() const -> const std::string & {
    return name_;
}

auto BasePluginWrapper::load() -> std::optional<PyExceptionInfo> {
    SPDLOG_INFO("{} is being loaded", name());
    try {
        boost::python::object &plugin_load = common_.load;
        plugin_load();
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return std::nullopt;
}

auto BasePluginWrapper::unload() -> std::optional<PyExceptionInfo> {
    SPDLOG_INFO("{} is being unloaded", name());
    try {
        boost::python::object &plugin_unload = common_.unload;
        plugin_unload();
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return std::nullopt;
}

auto BasePluginWrapper::get_config_description()
    -> std::variant<PyExceptionInfo, nlohmann::json> {
    nlohmann::json config_description;
    try {
        boost::python::object py_json       = boost::python::import("json");
        boost::python::object py_json_dumps = py_json.attr("dumps");

        boost::python::object py_plugin_conf_description =
            common_.get_config_description();
        boost::python::object py_str_json_conf_description =
            py_json_dumps(py_plugin_conf_description);

        std::string cpp_plugin_conf_description =
            boost::python::extract<std::string>(py_str_json_conf_description);
        config_description = nlohmann::json::parse(cpp_plugin_conf_description);
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return config_description;
}

auto BasePluginWrapper::get_default_config()
    -> std::variant<PyExceptionInfo, nlohmann::json> {
    nlohmann::json default_config;
    try {
        boost::python::object py_json       = boost::python::import("json");
        boost::python::object py_json_dumps = py_json.attr("dumps");

        boost::python::object py_plugin_default_conf =
            common_.get_default_config();
        boost::python::object py_str_json_default_conf =
            py_json_dumps(py_plugin_default_conf);

        std::string cpp_plugin_default_conf =
            boost::python::extract<std::string>(py_str_json_default_conf);
        default_config = nlohmann::json::parse(cpp_plugin_default_conf);
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return default_config;
}

auto BasePluginWrapper::validate_config(nlohmann::json &&new_config)
    -> std::variant<PyExceptionInfo, nlohmann::json> {
    nlohmann::json diagnostics;
    try {
        boost::python::object py_json       = boost::python::import("json");
        boost::python::object py_json_loads = py_json.attr("loads");
        boost::python::object py_json_dumps = py_json.attr("dumps");

        std::string           new_conf_str  = new_config.dump();
        boost::python::object py_new_conf   = py_json_loads(new_conf_str);
        boost::python::object py_conf_diagnostics =
            common_.validate_config(py_new_conf);
        boost::python::object py_conf_diagnostics_str =
            py_json_dumps(py_conf_diagnostics);
        std::string cpp_conf_diagnostics_str =
            boost::python::extract<std::string>(py_conf_diagnostics_str);
        diagnostics = nlohmann::json::parse(cpp_conf_diagnostics_str);
        return diagnostics;
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return diagnostics;
}
