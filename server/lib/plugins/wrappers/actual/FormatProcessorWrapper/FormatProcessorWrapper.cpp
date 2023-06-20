#include "FormatProcessorWrapper.hpp"
#include "BasePluginWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include <boost/python/errors.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/import.hpp>

auto FormatProcessorWrapper::FormatProcessorsFunctions::build(
    const boost::python::object &module)
    -> std::variant<FormatProcessorsFunctions, PyExceptionInfo> {
    auto plugin_container = FormatProcessorsFunctions();
    try {
        plugin_container.save = module.attr("save");
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return plugin_container;
}

FormatProcessorWrapper::FormatProcessorWrapper(BasePluginWrapper &&base)
    : BasePluginWrapper(std::move(base)) {
}

auto FormatProcessorWrapper::name() const -> const std::string & {
    static auto base_name      = '`' + BasePluginWrapper::name() + '`';
    static auto typed_provider = "[FormatProcessorWrapper] " + base_name;
    return typed_provider;
}

FormatProcessorWrapper::FormatProcessorWrapper(
    const FormatProcessorWrapper &other)
    : BasePluginWrapper(other.name(), other.common_),
      specifics_(other.specifics_) {
}

auto FormatProcessorWrapper::build(const std::string           &name,
                                   const boost::python::object &module)
    -> std::variant<FormatProcessorWrapper, PyExceptionInfo> {
    auto base_or_error = BasePluginWrapper::build(name, module);
    if (std::holds_alternative<PyExceptionInfo>(base_or_error)) {
        return std::get<PyExceptionInfo>(base_or_error);
    }
    auto base = std::move(std::get<BasePluginWrapper>(base_or_error));

    auto specifics_or_error = FormatProcessorsFunctions::build(module);
    if (std::holds_alternative<PyExceptionInfo>(specifics_or_error)) {
        return std::get<PyExceptionInfo>(specifics_or_error);
    }
    auto specifics = std::get<FormatProcessorsFunctions>(specifics_or_error);

    auto wrapper   = FormatProcessorWrapper(std::move(base));
    wrapper.specifics_ = specifics;
    return wrapper;
}

auto FormatProcessorWrapper::save(const ResultFilesPaths &paths)
    -> std::variant<FormatProcessorWrapper::type, PyExceptionInfo> {
    std::string res;
    try {
        boost::python::object py_res = specifics_.save(paths.cards.string());
        res = boost::python::extract<std::string>(py_res);
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return res;
}
