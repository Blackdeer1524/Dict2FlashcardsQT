#ifndef PLUGINS_PROVIDER_H
#define PLUGINS_PROVIDER_H

#include "AudiosProviderWrapper.hpp"
#include "DefinitionsProviderWrapper.hpp"
#include "FormatProcessorWrapper.hpp"
#include "IAudiosProviderWrapper.hpp"
#include "IDefinitionsProviderWrapper.hpp"
#include "IFormatProcessorWrapper.hpp"
#include "IImagesProviderWrapper.hpp"
#include "ISentencesProviderWrapper.hpp"
#include "ImagesProviderWrapper.hpp"
#include "PluginsLoader.hpp"
#include "SentencesProviderWrapper.hpp"
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>

class IPluginsProvider {
 public:
    virtual ~IPluginsProvider() = default;

    [[nodiscard]] virtual auto
    get_definitions_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<DefinitionsProviderWrapper,
                            std::function<void(IDefinitionsProviderWrapper *)>>,
            PyExceptionInfo>> = 0;

    [[nodiscard]] virtual auto
    get_sentences_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<SentencesProviderWrapper,
                            std::function<void(ISentencesProviderWrapper *)>>,
            PyExceptionInfo>> = 0;

    [[nodiscard]] virtual auto
    get_images_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<ImagesProviderWrapper,
                            std::function<void(IImagesProviderWrapper *)>>,
            PyExceptionInfo>> = 0;

    [[nodiscard]] virtual auto
    get_audios_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<AudiosProviderWrapper,
                            std::function<void(IAudiosProviderWrapper *)>>,
            PyExceptionInfo>> = 0;

    [[nodiscard]] virtual auto
    get_format_processor(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<FormatProcessorWrapper,
                            std::function<void(IFormatProcessorWrapper *)>>,
            PyExceptionInfo>>                             = 0;

    virtual auto               load_new_plugins() -> void = 0;

    [[nodiscard]] virtual auto list_definitions_providers() const
        -> PluginsInfo = 0;

    [[nodiscard]] virtual auto list_sentences_providers() const
        -> PluginsInfo                                                      = 0;

    [[nodiscard]] virtual auto list_images_providers() const -> PluginsInfo = 0;

    [[nodiscard]] virtual auto list_audios_providers() const -> PluginsInfo = 0;

    [[nodiscard]] virtual auto list_format_processors() const
        -> PluginsInfo = 0;
};

struct PluginTypesLocationsConfig {
    std::filesystem::path definitions_providers_dir;
    std::filesystem::path sentences_providers_dir;
    std::filesystem::path images_providers_dir;
    std::filesystem::path audios_providers_dir;
    std::filesystem::path format_processors_dir;
};

class PluginsProvider : public IPluginsProvider {
 public:
    explicit PluginsProvider(PluginTypesLocationsConfig &&config);

    [[nodiscard]] auto get_definitions_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<DefinitionsProviderWrapper,
                            std::function<void(IDefinitionsProviderWrapper *)>>,
            PyExceptionInfo>> override;

    [[nodiscard]] auto get_sentences_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<SentencesProviderWrapper,
                            std::function<void(ISentencesProviderWrapper *)>>,
            PyExceptionInfo>> override;

    [[nodiscard]] auto get_images_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<ImagesProviderWrapper,
                            std::function<void(IImagesProviderWrapper *)>>,
            PyExceptionInfo>> override;

    [[nodiscard]] auto get_audios_provider(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<AudiosProviderWrapper,
                            std::function<void(IAudiosProviderWrapper *)>>,
            PyExceptionInfo>> override;

    [[nodiscard]] auto get_format_processor(const std::string &name) const
        -> std::optional<std::variant<
            std::unique_ptr<FormatProcessorWrapper,
                            std::function<void(IFormatProcessorWrapper *)>>,
            PyExceptionInfo>> override;

    auto               load_new_plugins() -> void override;

    [[nodiscard]] auto list_definitions_providers() const
        -> PluginsInfo override;

    [[nodiscard]] auto list_sentences_providers() const -> PluginsInfo override;

    [[nodiscard]] auto list_images_providers() const -> PluginsInfo override;

    [[nodiscard]] auto list_audios_providers() const -> PluginsInfo override;

    [[nodiscard]] auto list_format_processors() const -> PluginsInfo override;

 private:
    std::unique_ptr<
        IPluginsLoader<DefinitionsProviderWrapper, IDefinitionsProviderWrapper>>
        definitions_providers_;
    std::unique_ptr<
        IPluginsLoader<SentencesProviderWrapper, ISentencesProviderWrapper>>
        sentences_providers_;
    std::unique_ptr<
        IPluginsLoader<ImagesProviderWrapper, IImagesProviderWrapper>>
        images_providers_;
    std::unique_ptr<
        IPluginsLoader<AudiosProviderWrapper, IAudiosProviderWrapper>>
        audios_providers_;
    std::unique_ptr<
        IPluginsLoader<FormatProcessorWrapper, IFormatProcessorWrapper>>
        format_processors_;
};

#endif  // !PLUGINS_PROVIDER_H
