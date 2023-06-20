#include "ResponseGenerators.hpp"
#include "AudiosProviderWrapper.hpp"
#include "DefinitionsProviderWrapper.hpp"
#include "FormatProcessorWrapper.hpp"
#include "IAudiosProviderWrapper.hpp"
#include "IFormatProcessorWrapper.hpp"
#include "IImagesProviderWrapper.hpp"
#include "IPluginWrapper.hpp"
#include "ISentencesProviderWrapper.hpp"
#include "ImagesProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include "SentencesProviderWrapper.hpp"
#include "exception.hpp"
#include "querying.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"

#include <cstdint>
#include <nlohmann/detail/exceptions.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <variant>

using nlohmann::json;
using std::string_literals::operator""s;

ResponseGenerator::ResponseGenerator(
    std::shared_ptr<PluginsProvider> plugins_provider)
    : plugins_provider_(std::move(plugins_provider)) {
}

static auto return_error(const std::string &message) -> json {
    json dst;
    dst["status"]  = 1;
    dst["message"] = message;
    SPDLOG_ERROR(message);
    return dst;
}

auto ResponseGenerator::handle(const std::string &request) -> json {
    SPDLOG_INFO("Got new request");
    json parsed_request;
    try {
        parsed_request = json::parse(request);
    } catch (json::exception &e) {
        return return_error(e.what());
    }
    if (!parsed_request.is_object()) {
        return return_error("Got non-json object as a request");
    }
    if (!parsed_request.contains("query_type")) {
        return return_error(
            "Every server request has to have \"query_type\" field");
    }
    json json_query_type = parsed_request[QUERY_TYPE_FIELD];
    if (!json_query_type.is_string()) {
        return return_error("\""s + QUERY_TYPE_FIELD +
                            "\" field is has to be a string");
    }
    std::string query_type = json_query_type;
    if (query_type == INIT_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request", INIT_QUERY_TYPE);
        return handle_init(parsed_request);
    }
    if (query_type == GET_DEFAULT_CONFIG_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request",
                    GET_DEFAULT_CONFIG_QUERY_TYPE);
        return handle_get_default_config(parsed_request);
    }
    if (query_type == GET_CONFIG_SCHEME_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request",
                    GET_CONFIG_SCHEME_QUERY_TYPE);
        return handle_get_config_scheme(parsed_request);
    }
    if (query_type == VALIDATE_CONFIG_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request",
                    VALIDATE_CONFIG_QUERY_TYPE);
        return handle_validate_config(parsed_request);
    }
    if (query_type == LIST_PLUGINS_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request", LIST_PLUGINS_QUERY_TYPE);
        return handle_list_plugins(parsed_request);
    }
    if (query_type == LOAD_NEW_PLUGINS_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request",
                    LOAD_NEW_PLUGINS_QUERY_TYPE);
        return handle_load_new_plugins(parsed_request);
    }
    if (query_type == GET_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request", GET_QUERY_TYPE);
        return handle_get(parsed_request);
    }
    if (query_type == GET_DICT_SCHEME_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request",
                    GET_DICT_SCHEME_QUERY_TYPE);
        return handle_get_dict_scheme(parsed_request);
    }
    if (query_type == SAVE_QUERY_TYPE) {
        SPDLOG_INFO("Started handling `{}` request", SAVE_QUERY_TYPE);
        return handle_save(parsed_request);
    }
    return return_error("Unknown query type: "s + query_type);
}

auto ResponseGenerator::handle_init(const nlohmann::json &request)
    -> nlohmann::json {
    if (!request.contains(PLUGIN_TYPE_FIELD)) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[PLUGIN_TYPE_FIELD].is_string()) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" field is expected to be a string");
    }
    auto plugin_type = request[PLUGIN_TYPE_FIELD].get<std::string>();

    if (!request.contains(PLUGIN_NAME_FIELD)) {
        return return_error("\""s + PLUGIN_NAME_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[PLUGIN_NAME_FIELD].is_string()) {
        return return_error("\""s + PLUGIN_NAME_FIELD +
                            "\" field is expected to be a string");
    }
    auto plugin_name = request[PLUGIN_NAME_FIELD].get<std::string>();

    if (plugin_type == DEFINITION_PROVIDER_PLUGIN_TYPE) {
        SPDLOG_INFO("Handling {} definitions provider's initialization request",
                    plugin_name);

        auto requested_wrapper_option =
            plugins_provider_->get_definitions_provider(plugin_name);
        if (!requested_wrapper_option.has_value()) {
            return return_error("\"" + plugin_name +
                                "\" definition provider not found");
        }
        auto wrapper_variant = std::move(requested_wrapper_option.value());
        if (std::holds_alternative<PyExceptionInfo>(wrapper_variant)) {
            auto exception_info = std::get<PyExceptionInfo>(wrapper_variant);

            return return_error("Exception was thrown during \"" + plugin_name +
                                "\" definitions provider's construction:\n" +
                                exception_info.stack_trace());
        }

        auto wrapper =
            std::move(std::get<std::unique_ptr<
                          DefinitionsProviderWrapper,
                          std::function<void(IDefinitionsProviderWrapper *)>>>(
                wrapper_variant));

        plugins_bundle_.set_definitions_provider(std::move(wrapper));

        SPDLOG_INFO("Successfully handled {} definition provider's "
                    "initialization request",
                    plugin_name);
        return R"({"status": 0, "message": ""})"_json;
    }
    if (plugin_type == SENTENCES_PROVIDER_PLUGIN_TYPE) {
        SPDLOG_INFO("Handling {} sentences provider's initialization request",
                    plugin_name);

        auto requested_wrapper_option =
            plugins_provider_->get_sentences_provider(plugin_name);
        if (!requested_wrapper_option.has_value()) {
            return return_error("\"" + plugin_name +
                                "\" sentences provider not found");
        }
        auto &wrapper_variant = requested_wrapper_option.value();
        if (std::holds_alternative<PyExceptionInfo>(wrapper_variant)) {
            auto exception_info = std::get<PyExceptionInfo>(wrapper_variant);

            return return_error("Exception was thrown during \"" + plugin_name +
                                "\" sentence provider's construction:\n" +
                                exception_info.stack_trace());
        }
        auto wrapper =
            std::move(std::get<std::unique_ptr<
                          SentencesProviderWrapper,
                          std::function<void(ISentencesProviderWrapper *)>>>(
                wrapper_variant));
        plugins_bundle_.set_sentences_provider(std::move(wrapper));

        SPDLOG_INFO("Successfully handled {} sentences provider's "
                    "initialization request",
                    plugin_name);
        return R"({"status": 0, "message": ""})"_json;
    }
    if (plugin_type == IMAGES_PROVIDER_PLUGIN_TYPE) {
        SPDLOG_INFO("Handling {} images provider's initialization request",
                    plugin_name);

        auto requested_wrapper_option =
            plugins_provider_->get_images_provider(plugin_name);
        if (!requested_wrapper_option.has_value()) {
            return return_error("\"" + plugin_name +
                                "\" images provider not found");
        }
        auto &wrapper_variant = requested_wrapper_option.value();
        if (std::holds_alternative<PyExceptionInfo>(wrapper_variant)) {
            auto exception_info = std::get<PyExceptionInfo>(wrapper_variant);

            return return_error("Exception was thrown during \"" + plugin_name +
                                "\" images provider's construction:\n" +
                                exception_info.stack_trace());
        }
        auto wrapper = std::move(
            std::get<
                std::unique_ptr<ImagesProviderWrapper,
                                std::function<void(IImagesProviderWrapper *)>>>(
                wrapper_variant));
        plugins_bundle_.set_images_provider(std::move(wrapper));

        SPDLOG_INFO("Successfully handled {} images provider's "
                    "initialization request",
                    plugin_name);
        return R"({"status": 0, "message": ""})"_json;
    }
    if (plugin_type == AUDIOS_PROVIDER_PLUGIN_TYPE) {
        SPDLOG_INFO("Handling {} audios provider's initialization request",
                    plugin_name);

        auto requested_wrapper_option =
            plugins_provider_->get_audios_provider(plugin_name);
        if (!requested_wrapper_option.has_value()) {
            return return_error("\"" + plugin_name +
                                "\" audios provider not found");
        }
        auto &wrapper_variant = requested_wrapper_option.value();
        if (std::holds_alternative<PyExceptionInfo>(wrapper_variant)) {
            auto exception_info = std::get<PyExceptionInfo>(wrapper_variant);

            return return_error("Exception was thrown during \"" + plugin_name +
                                "\" audios provider's construction:\n" +
                                exception_info.stack_trace());
        }
        auto wrapper = std::move(
            std::get<
                std::unique_ptr<AudiosProviderWrapper,
                                std::function<void(IAudiosProviderWrapper *)>>>(
                wrapper_variant));
        plugins_bundle_.set_audios_provider(std::move(wrapper));

        SPDLOG_INFO("Successfully handled {} audios provider's "
                    "initialization request",
                    plugin_name);
        return R"({"status": 0, "message": ""})"_json;
    }
    if (plugin_type == FORMAT_PROCESSOR_PLUGIN_TYPE) {
        SPDLOG_INFO("Handling {} format processor's initialization request",
                    plugin_name);

        auto requested_wrapper_option =
            plugins_provider_->get_format_processor(plugin_name);
        if (!requested_wrapper_option.has_value()) {
            return return_error("\"" + plugin_name +
                                "\" format processor not found");
        }
        auto &wrapper_variant = requested_wrapper_option.value();
        if (std::holds_alternative<PyExceptionInfo>(wrapper_variant)) {
            auto exception_info = std::get<PyExceptionInfo>(wrapper_variant);

            return return_error("Exception was thrown during \"" + plugin_name +
                                "\" format processor's construction:\n" +
                                exception_info.stack_trace());
        }
        auto wrapper =
            std::move(std::get<std::unique_ptr<
                          FormatProcessorWrapper,
                          std::function<void(IFormatProcessorWrapper *)>>>(
                wrapper_variant));
        plugins_bundle_.set_format_processor(std::move(wrapper));

        SPDLOG_INFO("Successfully handled {} format processor's "
                    "initialization request",
                    plugin_name);
        return R"({"status": 0, "message": ""})"_json;
    }
    return return_error("Unknown plugin_type: " + plugin_type);
}

auto ResponseGenerator::handle_get_default_config(const nlohmann::json &request)
    -> nlohmann::json {
    return return_error("handle_get_default_config() is not implemented");
}

auto ResponseGenerator::handle_get_config_scheme(const nlohmann::json &request)
    -> nlohmann::json {
    return return_error("handle_get_config_scheme() is not implemented");
}

auto ResponseGenerator::handle_validate_config(const nlohmann::json &request)
    -> nlohmann::json {
    using std::string_literals::operator""s;

    if (!request.contains(PLUGIN_TYPE_FIELD)) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[PLUGIN_TYPE_FIELD].is_string()) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" field is expected to be a string");
    }
    auto plugin_type = request[PLUGIN_TYPE_FIELD].get<std::string>();

    if (!request.contains(CONFIG_FIELD)) {
        return return_error("\""s + CONFIG_FIELD +
                            "\" filed was not found in request");
    }
    auto            config = request[CONFIG_FIELD];

    IPluginWrapper *provider;
    if (plugin_type == DEFINITION_PROVIDER_PLUGIN_TYPE) {
        provider = plugins_bundle_.definitions_provider();
        if (provider == nullptr) {
            return return_error("Definitions provider is not initialized");
        }
    } else if (plugin_type == SENTENCES_PROVIDER_PLUGIN_TYPE) {
        provider = plugins_bundle_.sentences_provider();
        if (provider == nullptr) {
            return return_error("Sentences provider is not initialized");
        }
    } else if (plugin_type == IMAGES_PROVIDER_PLUGIN_TYPE) {
        provider = plugins_bundle_.images_provider();
        if (provider == nullptr) {
            return return_error("Images provider is not initialized");
        }
    } else if (plugin_type == AUDIOS_PROVIDER_PLUGIN_TYPE) {
        provider = plugins_bundle_.audios_provider();
        if (provider == nullptr) {
            return return_error("Audios provider is not initialized");
        }
    } else if (plugin_type == FORMAT_PROCESSOR_PLUGIN_TYPE) {
        provider = plugins_bundle_.format_processor();
        if (provider == nullptr) {
            return return_error("Format processor is not initialized");
        }
    } else {
        return return_error("Unknown plugin type for `validate_config`: " +
                            plugin_type);
    }

    auto res_or_error = provider->validate_config(std::move(config));

    if (std::holds_alternative<PyExceptionInfo>(res_or_error)) {
        auto error = std::get<PyExceptionInfo>(res_or_error);
        return return_error(error.stack_trace());
    }
    auto res           = std::get<nlohmann::json>(res_or_error);
    auto response      = nlohmann::json();
    response["status"] = 0;
    response["result"] = res;
    return response;
}

auto ResponseGenerator::handle_list_plugins(const nlohmann::json &request)
    -> nlohmann::json {
    if (!request.contains(PLUGIN_TYPE_FIELD)) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[PLUGIN_TYPE_FIELD].is_string()) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" field is expected to be a string");
    }
    auto        plugin_type = request[PLUGIN_TYPE_FIELD].get<std::string>();

    PluginsInfo listings;
    if (plugin_type == DEFINITION_PROVIDER_PLUGIN_TYPE) {
        listings = plugins_provider_->list_definitions_providers();
    } else if (plugin_type == SENTENCES_PROVIDER_PLUGIN_TYPE) {
        listings = plugins_provider_->list_sentences_providers();
    } else if (plugin_type == IMAGES_PROVIDER_PLUGIN_TYPE) {
        listings = plugins_provider_->list_images_providers();
    } else if (plugin_type == AUDIOS_PROVIDER_PLUGIN_TYPE) {
        listings = plugins_provider_->list_audios_providers();
    } else if (plugin_type == FORMAT_PROCESSOR_PLUGIN_TYPE) {
        listings = plugins_provider_->list_format_processors();
    } else {
        return return_error(
            "Unknown plugin type for `list_plugins` request: "s + plugin_type);
    }
    json res;
    res["status"] = 0;
    res["result"] = listings;
    return res;
}

auto ResponseGenerator::handle_load_new_plugins(const nlohmann::json &request)
    -> nlohmann::json {
    return return_error("handle_load_new_plugins() is not implemented");
}

auto ResponseGenerator::handle_get(const nlohmann::json &request)
    -> nlohmann::json {
    if (!request.contains(PLUGIN_TYPE_FIELD)) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[PLUGIN_TYPE_FIELD].is_string()) {
        return return_error("\""s + PLUGIN_TYPE_FIELD +
                            "\" field is expected to be a string");
    }
    auto plugin_type = request[PLUGIN_TYPE_FIELD].get<std::string>();

    if (plugin_type == DEFINITION_PROVIDER_PLUGIN_TYPE) {
        return handle_get_definitions(request);
    }
    if (plugin_type == SENTENCES_PROVIDER_PLUGIN_TYPE) {
        return handle_get_sentences(request);
    }
    if (plugin_type == IMAGES_PROVIDER_PLUGIN_TYPE) {
        return handle_get_images(request);
    }
    if (plugin_type == AUDIOS_PROVIDER_PLUGIN_TYPE) {
        return handle_get_audios(request);
    }
    return return_error("Unknown plugin type for `get` request: "s +
                        plugin_type);
}

auto ResponseGenerator::handle_get_definitions(const nlohmann::json &request)
    -> nlohmann::json {
    SPDLOG_INFO("Starting handling `get` request for definitions");

    auto *provider = plugins_bundle_.definitions_provider();
    if (provider == nullptr) {
        return return_error("Definitions provider is not initialized");
    }

    if (!request.contains(WORD_FIELD)) {
        return return_error("\""s + WORD_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[WORD_FIELD].is_string()) {
        return return_error("\""s + WORD_FIELD +
                            "\" field is expected to be a string");
    }
    auto word = request[WORD_FIELD].get<std::string>();

    if (!request.contains(FILTER_QUERY_FIELD)) {
        return return_error("\""s + FILTER_QUERY_FIELD +
                            "\" fieled was not found in request");
    }
    if (!request[FILTER_QUERY_FIELD].is_string()) {
        return return_error("\""s + FILTER_QUERY_FIELD +
                            "\" field is expected to be a string");
    }
    std::string filter_query = request[FILTER_QUERY_FIELD].get<std::string>();

    if (!request.contains(BATCH_SIZE_FIELD)) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[BATCH_SIZE_FIELD].is_number()) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" field is expected to be a number");
    }
    auto batch_size = request[BATCH_SIZE_FIELD].get<uint64_t>();

    if (!request.contains(RESTART_FIELD)) {
        return return_error("\""s + RESTART_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[RESTART_FIELD].is_boolean()) {
        return return_error("\""s + RESTART_FIELD +
                            "\" field is expected to be a boolean");
    }
    auto restart = request[RESTART_FIELD].get<bool>();

    std::function<std::variant<bool, std::string>(const nlohmann::json &)>
        filter_function;
    try {
        filter_function = prepare_filter(filter_query);
    } catch (const ComponentException &error) {
        return return_error("Could'n parse query. Reason: "s + error.what());
    }
    DefinitionsProviderWrapper::type batch;
    while (batch_size && batch.second.empty()) {
        auto result_or_error = provider->get(word, batch_size, restart);
        if (std::holds_alternative<PyExceptionInfo>(result_or_error)) {
            auto exception_info = std::get<PyExceptionInfo>(result_or_error);
            return return_error("Exception was thrown during \"" +
                                provider->name() + "\" definitions request:\n" +
                                exception_info.stack_trace());
        }
        if (std::holds_alternative<DefinitionsProviderWrapper::type>(
                result_or_error)) {
            auto result =
                std::get<DefinitionsProviderWrapper::type>(result_or_error);

            if (result.first.empty()) {
                batch.second = std::move(result.second);
                break;
            }

            auto insertion_res =
                result.first |
                std::ranges::views::filter(
                    [&filter_function](const Card &item) -> bool {
                        auto filtration_res = filter_function(item);
                        if (std::holds_alternative<bool>(filtration_res)) {
                            auto res = std::get<bool>(filtration_res);
                            return res;
                        }
                        auto res = std::get<std::string>(filtration_res);
                        SPDLOG_WARN("Couldn't filter card, thus evaluating "
                                    "filter as `false`. Reason: {}",
                                    res);
                        return false;
                    });

            batch.first.insert(
                batch.first.end(), insertion_res.begin(), insertion_res.end());
            batch.second = std::move(result.second);
            batch_size   = (batch.first.size() >= batch_size)
                               ? 0
                               : batch_size - batch.first.size();
        } else {
            auto non_python_error_message =
                std::get<std::string>(result_or_error);

            auto json_message = R"({"status": 1, "message": ")"s +
                                non_python_error_message + R"("})";

            return json::parse(json_message);
        }
    }

    json res;
    res["status"] = static_cast<int>(!batch.second.empty());
    res["result"] = batch;
    SPDLOG_INFO("Successfully handled `get` request for definitions");
    return res;
}

auto ResponseGenerator::handle_get_sentences(const nlohmann::json &request)
    -> nlohmann::json {
    SPDLOG_INFO("Starting handling `get` request for sentences");

    auto *provider = plugins_bundle_.sentences_provider();
    if (provider == nullptr) {
        return return_error("Sentences provider is not initialized");
    }

    if (!request.contains(WORD_FIELD)) {
        return return_error("\""s + WORD_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[WORD_FIELD].is_string()) {
        return return_error("\""s + WORD_FIELD +
                            "\" field is expected to be a string");
    }
    auto word = request[WORD_FIELD].get<std::string>();

    if (!request.contains(BATCH_SIZE_FIELD)) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[BATCH_SIZE_FIELD].is_number()) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" field is expected to be a number");
    }
    auto batch_size = request[BATCH_SIZE_FIELD].get<uint64_t>();

    if (!request.contains(RESTART_FIELD)) {
        return return_error("\""s + RESTART_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[RESTART_FIELD].is_boolean()) {
        return return_error("\""s + RESTART_FIELD +
                            "\" field is expected to be a boolean");
    }
    auto restart         = request[RESTART_FIELD].get<bool>();

    auto result_or_error = provider->get(word, batch_size, restart);
    if (std::holds_alternative<PyExceptionInfo>(result_or_error)) {
        auto exception_info = std::get<PyExceptionInfo>(result_or_error);
        return return_error("Exception was thrown during \"" +
                            provider->name() + "\" sentences request:\n" +
                            exception_info.stack_trace());
    }
    if (std::holds_alternative<SentencesProviderWrapper::type>(
            result_or_error)) {
        auto result = std::get<SentencesProviderWrapper::type>(result_or_error);
        json res;
        res["status"] = 0;
        res["result"] = result;
        SPDLOG_INFO("Successfully handled `get` request for sentences");
        return res;
    }
    auto non_python_error_message = std::get<std::string>(result_or_error);

    auto json_message =
        R"({"status": 1, "message": ")"s + non_python_error_message + R"("})";

    return json::parse(json_message);
}

auto ResponseGenerator::handle_get_images(const nlohmann::json &request)
    -> nlohmann::json {
    SPDLOG_INFO("Starting handling `get` request for images");

    auto *provider = plugins_bundle_.images_provider();
    if (provider == nullptr) {
        return return_error("Images provider is not initialized");
    }

    if (!request.contains(WORD_FIELD)) {
        return return_error("\""s + WORD_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[WORD_FIELD].is_string()) {
        return return_error("\""s + WORD_FIELD +
                            "\" field is expected to be a string");
    }
    auto word = request[WORD_FIELD].get<std::string>();

    if (!request.contains(BATCH_SIZE_FIELD)) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[BATCH_SIZE_FIELD].is_number()) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" field is expected to be a number");
    }
    auto batch_size = request[BATCH_SIZE_FIELD].get<uint64_t>();

    if (!request.contains(RESTART_FIELD)) {
        return return_error("\""s + RESTART_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[RESTART_FIELD].is_boolean()) {
        return return_error("\""s + RESTART_FIELD +
                            "\" field is expected to be a boolean");
    }
    auto restart         = request[RESTART_FIELD].get<bool>();

    auto result_or_error = provider->get(word, batch_size, restart);
    if (std::holds_alternative<PyExceptionInfo>(result_or_error)) {
        auto exception_info = std::get<PyExceptionInfo>(result_or_error);
        return return_error("Exception was thrown during \"" +
                            provider->name() + "\" images request:\n" +
                            exception_info.stack_trace());
    }
    if (std::holds_alternative<ImagesProviderWrapper::type>(result_or_error)) {
        auto result = std::get<ImagesProviderWrapper::type>(result_or_error);
        json res;
        res["status"] = 0;
        res["result"] = result;
        SPDLOG_INFO("Successfully handled `get` request for images");
        return res;
    }
    auto non_python_error_message = std::get<std::string>(result_or_error);

    auto json_message =
        R"({"status": 1, "message": ")"s + non_python_error_message + R"("})";

    return json::parse(json_message);
}

auto ResponseGenerator::handle_get_audios(const nlohmann::json &request)
    -> nlohmann::json {
    SPDLOG_INFO("Starting handling `get` request for audios");

    auto *provider = plugins_bundle_.audios_provider();
    if (provider == nullptr) {
        return return_error("Audios provider is not initialized");
    }

    if (!request.contains(WORD_FIELD)) {
        return return_error("\""s + WORD_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[WORD_FIELD].is_string()) {
        return return_error("\""s + WORD_FIELD +
                            "\" field is expected to be a string");
    }
    auto word = request[WORD_FIELD].get<std::string>();

    if (!request.contains(BATCH_SIZE_FIELD)) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[BATCH_SIZE_FIELD].is_number()) {
        return return_error("\""s + BATCH_SIZE_FIELD +
                            "\" field is expected to be a number");
    }
    auto batch_size = request[BATCH_SIZE_FIELD].get<uint64_t>();

    if (!request.contains(RESTART_FIELD)) {
        return return_error("\""s + RESTART_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[RESTART_FIELD].is_boolean()) {
        return return_error("\""s + RESTART_FIELD +
                            "\" field is expected to be a boolean");
    }
    auto restart         = request[RESTART_FIELD].get<bool>();

    auto result_or_error = provider->get(word, batch_size, restart);
    if (std::holds_alternative<PyExceptionInfo>(result_or_error)) {
        auto exception_info = std::get<PyExceptionInfo>(result_or_error);
        return return_error("Exception was thrown during \"" +
                            provider->name() + "\" definitions request:\n" +
                            exception_info.stack_trace());
    }
    if (std::holds_alternative<AudiosProviderWrapper::type>(result_or_error)) {
        auto result = std::get<AudiosProviderWrapper::type>(result_or_error);
        json res;
        res["status"] = 0;
        res["result"] = result;
        SPDLOG_INFO("Successfully handled `get` request for audios");
        return res;
    }
    auto non_python_error_message = std::get<std::string>(result_or_error);

    return return_error(non_python_error_message);
}

auto ResponseGenerator::handle_save(const nlohmann::json &request)
    -> nlohmann::json {
    auto *provider = plugins_bundle_.format_processor();
    if (provider == nullptr) {
        return return_error("Format Processor is not initialized");
    }

    if (!request.contains(CARDS_PATH_FIELD)) {
        return return_error("\""s + CARDS_PATH_FIELD +
                            "\" filed was not found in request");
    }
    if (!request[CARDS_PATH_FIELD].is_string()) {
        return return_error("\""s + CARDS_PATH_FIELD +
                            "\" field is expected to be a string");
    }
    auto cards_path_field = request[CARDS_PATH_FIELD].get<std::string>();
    auto string_error_or_py_exception = provider->save({cards_path_field});

    if (std::holds_alternative<PyExceptionInfo>(string_error_or_py_exception)) {
        auto py_exception_info =
            std::get<PyExceptionInfo>(string_error_or_py_exception);
        return return_error(py_exception_info.stack_trace());
    }
    auto string_error = std::get<std::string>(string_error_or_py_exception);
    if (string_error.empty()) {
        SPDLOG_INFO("Successfully handled `save` request");
        return R"({"status": 0, "message": ""})"_json;
    }
    return return_error(string_error);
}

auto ResponseGenerator::handle_get_dict_scheme(const nlohmann::json &request)
    -> nlohmann::json {
    return return_error("handle_get_dict_scheme() is not implemented");
}
