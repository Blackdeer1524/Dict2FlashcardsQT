#ifndef PLUGINS_LOADER_H
#define PLUGINS_LOADER_H

#include "IDefinitionsProviderWrapper.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <boost/python/errors.hpp>
#include <boost/python/import.hpp>
#include <complex>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <nlohmann/detail/macro_scope.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "IPluginWrapper.hpp"
#include "PyExceptionInfo.hpp"

struct PluginsInfo {
    std::vector<std::string> success;
    std::vector<std::string> failed;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PluginsInfo, success, failed);

template <typename Wrapper, typename IWrapper>
    requires is_plugin_wrapper<Wrapper>
class IPluginsLoader {
 public:
    virtual ~IPluginsLoader() = default;

    virtual auto get(const std::string &plugin_name) -> std::optional<
        std::variant<std::unique_ptr<Wrapper, std::function<void(IWrapper *)>>,
                     PyExceptionInfo>>                             = 0;

    virtual auto               load_new_plugins() -> void          = 0;

    [[nodiscard]] virtual auto list_plugins() const -> PluginsInfo = 0;
};

// #include "DefinitionsProviderWrapper.hpp"
// using Wrapper  = DefinitionsProviderWrapper;
// using IWrapper = IDefinitionsProviderWrapper;

// Почему через темплейты? Не знаю, надо будет переделать
// TODO(blackdeer): REWORK PluginsLoader
template <typename Wrapper, typename IWrapper>
    requires std::derived_from<Wrapper, IWrapper> && is_plugin_wrapper<Wrapper>
class PluginsLoader : public IPluginsLoader<Wrapper, IWrapper> {
 public:
    explicit PluginsLoader(std::filesystem::path &&plugins_dir) noexcept(
        false) {
        try {
            boost::python::object sys = boost::python::import("sys");
            sys.attr("path").attr("append")(plugins_dir.c_str());
        } catch (const boost::python::error_already_set &) {
            SPDLOG_THROW("Couldn't import sys module");
        }

        std::ranges::for_each(
            std::filesystem::directory_iterator(plugins_dir),
            [this](const std::filesystem::path &dir_entry) {
                using std::string_literals::operator""s;
                SPDLOG_INFO("Registering plugin from "s + dir_entry.string());

                if (!std::filesystem::is_directory(dir_entry)) {
                    return;
                }

                auto                  module_name = dir_entry.filename();
                boost::python::object loaded_module;
                try {
                    loaded_module = boost::python::import(module_name.c_str());
                } catch (const boost::python::error_already_set &) {
                    SPDLOG_INFO("Failed to import module: "s +
                                module_name.string());
                    auto error_info = PyExceptionInfo::build();
                    failed_containers_.emplace(std::move(module_name),
                                               std::move(error_info));
                    return;
                }

                auto wrapper_or_error =
                    Wrapper::build(module_name.string(), loaded_module);

                SPDLOG_INFO("Successfully imported Python module: "s +
                            module_name.string());

                if (std::holds_alternative<PyExceptionInfo>(wrapper_or_error)) {
                    auto info = std::get<PyExceptionInfo>(wrapper_or_error);
                    SPDLOG_INFO("Failed to Register plugin from "s +
                                dir_entry.string());
                    failed_containers_.emplace(std::move(module_name),
                                               std::move(info));
                } else if (std::holds_alternative<Wrapper>(wrapper_or_error)) {
                    auto wrapper =
                        std::move(std::get<Wrapper>(wrapper_or_error));
                    SPDLOG_INFO("Successfully registered plugin from "s +
                                dir_entry.string());
                    loaded_containers_.emplace(std::move(module_name),
                                               std::move(wrapper));
                } else {
                    SPDLOG_THROW("Unknown return from a container build");
                }
            });
    }

    auto get(const std::string &plugin_name) -> std::optional<
        std::variant<std::unique_ptr<Wrapper, std::function<void(IWrapper *)>>,
                     PyExceptionInfo>> override {
        using std::string_literals::operator""s;
        SPDLOG_INFO(plugin_name + " was requested");

        auto res = loaded_containers_.find(plugin_name);
        if (res == loaded_containers_.end()) {
            SPDLOG_INFO(plugin_name + " not found");
            return std::nullopt;
        }
        SPDLOG_INFO(plugin_name + " was found");
        auto &found_wrapper_usage_pair = res->second;
        if (++found_wrapper_usage_pair.usage_count == 1) {
            auto load_result = found_wrapper_usage_pair.wrapper.load();
            if (load_result.has_value()) {
                return load_result.value();
            }
        }

        auto wrapper_copy = found_wrapper_usage_pair.wrapper;
        auto wrapper_with_usage_counter =
            std::unique_ptr<Wrapper, std::function<void(IWrapper *)>>(
                new Wrapper(wrapper_copy), [&](IWrapper *contained_wrapper) {
                    if (--found_wrapper_usage_pair.usage_count == 0) {
                        auto unload_result = contained_wrapper->unload();
                        if (unload_result.has_value()) {
                            SPDLOG_ERROR("Wrapper {} unloaded with error: {}",
                                         contained_wrapper->name(),
                                         unload_result->stack_trace());
                        }
                    }
                    delete contained_wrapper;
                });
        return wrapper_with_usage_counter;
    }

    auto load_new_plugins() -> void override {
        throw std::runtime_error("load_new_plugins() is not implemented");
    }

    [[nodiscard]] auto list_plugins() const -> PluginsInfo override {
        std::vector<std::string> loaded_plugins_names{};
        loaded_plugins_names.reserve(loaded_plugins_names.size());
        for (const auto &item : loaded_containers_) {
            loaded_plugins_names.push_back(item.first);
        }

        std::vector<std::string> failed_plugins_names{};
        failed_plugins_names.reserve(failed_containers_.size());
        for (const auto &item : failed_containers_) {
            failed_plugins_names.push_back(item.first);
        }

        return {.success = std::move(loaded_plugins_names),
                .failed  = std::move(failed_plugins_names)};
    }

 private:
    struct WrapperUsageTracker {
        Wrapper  wrapper;
        uint64_t usage_count;
    };

    std::unordered_map<std::string, WrapperUsageTracker> loaded_containers_;
    std::unordered_map<std::string, std::optional<PyExceptionInfo>>
        failed_containers_;
};

#endif
