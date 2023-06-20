#include "DefinitionsProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include "pyerrors.h"
#include "pythonrun.h"
#include "spdlog/spdlog.h"
#include <boost/python/exec.hpp>
#include <boost/python/import.hpp>
#include <exception>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

auto DefinitionsProviderWrapper::DefinitionsProvidersFunctions::build(
    const boost::python::object &module)
    -> std::variant<DefinitionsProvidersFunctions, PyExceptionInfo> {
    auto plugin_container = DefinitionsProvidersFunctions();
    try {
        plugin_container.get = module.attr("get");
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return plugin_container;
}

auto DefinitionsProviderWrapper::name() const -> const std::string & {
    static auto base_name      = '`' + BasePluginWrapper::name() + '`';
    static auto typed_provider = "[DefinitionsProviderWrapper] " + base_name;
    return typed_provider;
}

DefinitionsProviderWrapper::DefinitionsProviderWrapper(BasePluginWrapper &&base)
    : BasePluginWrapper(std::move(base)) {
}

DefinitionsProviderWrapper::DefinitionsProviderWrapper(
    const DefinitionsProviderWrapper &other)
    : BasePluginWrapper(other.name(), other.common_),
      specifics_(other.specifics_) {
}

auto DefinitionsProviderWrapper::build(const std::string           &name,
                                       const boost::python::object &module)
    -> std::variant<DefinitionsProviderWrapper, PyExceptionInfo> {
    SPDLOG_INFO("Trying to build " + name + " definition provider");
    auto base_or_error = BasePluginWrapper::build(name, module);
    if (std::holds_alternative<PyExceptionInfo>(base_or_error)) {
        return std::get<PyExceptionInfo>(base_or_error);
    }
    SPDLOG_INFO("Trying to build " + name + " definition provider");
    auto base = std::move(std::get<BasePluginWrapper>(base_or_error));

    auto specifics_or_error = DefinitionsProvidersFunctions::build(module);
    if (std::holds_alternative<PyExceptionInfo>(specifics_or_error)) {
        return std::get<PyExceptionInfo>(specifics_or_error);
    }
    auto specifics =
        std::get<DefinitionsProvidersFunctions>(specifics_or_error);

    auto wrapper       = DefinitionsProviderWrapper(std::move(base));
    wrapper.specifics_ = specifics;
    return wrapper;
}

auto DefinitionsProviderWrapper::get(const std::string &word,
                                     uint64_t           batch_size,
                                     bool               restart) -> std::
    variant<DefinitionsProviderWrapper::type, std::string, PyExceptionInfo> {
    SPDLOG_INFO(
        "[{}] Handling request: word: `{}`, batch_size: `{}`, restart: `{}`",
        name(),
        word,
        batch_size,
        restart);

    if (restart) {
        auto found_item = generators_.find(word);
        if (found_item != generators_.end()) {
            SPDLOG_INFO(
                "[{}] Restarting generator for word `{}`", name(), word);

            generators_.erase(found_item);
        } else {
            SPDLOG_WARN("[{}] restart request was given but no generator for "
                        "word `{}` was found",
                        name(),
                        word);
        }
    }
    if (generators_.find(word) == generators_.end()) {
        SPDLOG_INFO("[{}] Initializing generator for word `{}`", name(), word);

        boost::python::object generator;
        try {
            generator = specifics_.get(word);
            generator.attr("__next__")();
        } catch (const boost::python::error_already_set &) {
            return PyExceptionInfo::build().value();
        }
        generators_[word] = generator;
    }

    try {
        SPDLOG_INFO("[{}] (Python side) Loading JSON module", name());

        boost::python::object py_json       = boost::python::import("json");
        boost::python::object py_json_dumps = py_json.attr("dumps");

        SPDLOG_INFO(
            "[{}] (Python side) Trying to obtain data batch for word `{}`",
            name(),
            word);

        boost::python::object py_res =
            generators_[word].attr("send")(batch_size);

        SPDLOG_INFO("[{}] (Python side) successfully obtained data batch for "
                    "word `{}`. Trying to "
                    "serialize it to JSON",
                    name(),
                    word);

        boost::python::object py_json_res = py_json_dumps(py_res);

        SPDLOG_INFO("[{}] (Python side) successfully serialized data batch for "
                    "word `{}` to JSON",
                    name(),
                    word);

        std::string str_res = boost::python::extract<std::string>(py_json_res);
        nlohmann::json json_res = nlohmann::json::parse(str_res);

        try {
            SPDLOG_INFO(
                "[{}] (Server side) Trying to deserialize JSON data batch "
                "for word `{}`",
                name(),
                word);

            auto error_message = json_res[1].get<std::string>();
            auto cards         = json_res[0].get<std::vector<Card>>();

            SPDLOG_INFO(
                "[{}] (Server side) successfully deserialized JSON data batch "
                "for word `{}`",
                name(),
                word);

            return std::make_pair(cards, error_message);
        } catch (const std::exception &error) {
            return error.what();
        }
    } catch (boost::python::error_already_set &) {
        SPDLOG_INFO("[{}] caught Python exception during response construction "
                    "for word `{}`. Destroying corresponding Python generator",
                    name(),
                    word);

        auto        py_exc_info    = PyExceptionInfo::build().value();
        const auto &exception_type = py_exc_info.last_type();

        generators_.erase(word);
        if (exception_type == "<class 'StopIteration'>") {
            SPDLOG_INFO("[{}] caught Python StopIteration exception during "
                        "response construction "
                        "for word `{}`. Respoding with empty object",
                        name(),
                        word);
            return {};
        }

        // SPDLOG_ERROR("[{}] caught Python exception of type: {} during "
        //              "response construction "
        //              "for word `{}`. Forwarding exception info to the
        //              caller", name(), py_exc_info.last_type(), word);

        return py_exc_info;
    }
    std::vector<Card> empty(0);
    return std::make_pair(empty, "");
}

auto DefinitionsProviderWrapper::get_dictionary_scheme()
    -> std::variant<nlohmann::json, PyExceptionInfo> {
    return {};
}
