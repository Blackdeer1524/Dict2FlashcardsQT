#include "AudiosProviderWrapper.hpp"
#include "BasePluginWrapper.hpp"
#include "IAudiosProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <boost/python/exec.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/import.hpp>
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

auto AudiosProviderWrapper::AudiosProvidesFunctions::build(
    const boost::python::object &module)
    -> std::variant<AudiosProvidesFunctions, PyExceptionInfo> {
    auto plugin_container = AudiosProvidesFunctions();
    try {
        plugin_container.get = module.attr("get");
    } catch (const boost::python::error_already_set &) {
        return PyExceptionInfo::build().value();
    }
    return plugin_container;
}

auto AudiosProviderWrapper::name() const -> const std::string & {
    static auto base_name      = '`' + BasePluginWrapper::name() + '`';
    static auto typed_provider = "[AudiosProviderWrapper] " + base_name;
    return typed_provider;
}

AudiosProviderWrapper::AudiosProviderWrapper(const AudiosProviderWrapper &other)
    : BasePluginWrapper(other.name(), other.common_),
      specifics_(other.specifics_) {
}

AudiosProviderWrapper::AudiosProviderWrapper(BasePluginWrapper &&base)
    : BasePluginWrapper(std::move(base)) {
}

auto AudiosProviderWrapper::build(const std::string           &name,
                                  const boost::python::object &module)
    -> std::variant<AudiosProviderWrapper, PyExceptionInfo> {
    auto base_or_error = BasePluginWrapper::build(name, module);
    if (std::holds_alternative<PyExceptionInfo>(base_or_error)) {
        return std::get<PyExceptionInfo>(base_or_error);
    }
    auto base = std::move(std::get<BasePluginWrapper>(base_or_error));

    auto specifics_or_error = AudiosProvidesFunctions::build(module);
    if (std::holds_alternative<PyExceptionInfo>(specifics_or_error)) {
        return std::get<PyExceptionInfo>(specifics_or_error);
    }
    auto specifics     = std::get<AudiosProvidesFunctions>(specifics_or_error);

    auto wrapper       = AudiosProviderWrapper(std::move(base));
    wrapper.specifics_ = specifics;
    return wrapper;
}

auto AudiosProviderWrapper::get(const std::string &word,
                                uint64_t           batch_size,
                                bool               restart)
    -> std::variant<AudiosProviderWrapper::type, std::string, PyExceptionInfo> {
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

            auto audio_information = json_res[0].get<Media>();
            auto error_message     = json_res[1].get<std::string>();

            SPDLOG_INFO(
                "[{}] (Server side) successfully deserialized JSON data batch "
                "for word `{}`",
                name(),
                word);

            return std::make_pair(audio_information, error_message);
        } catch (const std::exception &error) {
            return error.what();
        }
    } catch (boost::python::error_already_set &) {
        SPDLOG_INFO("[{}] caught Python exception during response construction "
                    "for word `{}`. Destroying corresponding Python generator",
                    name(),
                    word);

        generators_.erase(word);
        auto        py_exc_info    = PyExceptionInfo::build().value();
        const auto &exception_type = py_exc_info.last_type();

        if (exception_type == "<class 'StopIteration'>") {
            SPDLOG_INFO("[{}] caught Python StopIteration exception during "
                        "response construction "
                        "for word `{}`. Respoding with empty object",
                        name(),
                        word);

            return {};
        }

        return py_exc_info;
    }
    Media empty;
    return std::make_pair(empty, "");
}
