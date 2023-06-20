#include "PluginsBundle.hpp"
#include "IDefinitionsProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"
#include <cstddef>
#include <optional>

PluginsBundle::PluginsBundle() = default;

auto PluginsBundle::set_definitions_provider(
    std::unique_ptr<IDefinitionsProviderWrapper,
                    std::function<void(IDefinitionsProviderWrapper *)>>
        new_provider) -> void {
    definitions_provider_ = std::move(new_provider);
}

auto PluginsBundle::set_sentences_provider(
    std::unique_ptr<ISentencesProviderWrapper,
                    std::function<void(ISentencesProviderWrapper *)>>
        new_provider) -> void {
    sentences_provider_ = std::move(new_provider);
}

auto PluginsBundle::set_images_provider(
    std::unique_ptr<IImagesProviderWrapper,
                    std::function<void(IImagesProviderWrapper *)>> new_provider)
    -> void {
    images_provider_ = std::move(new_provider);
}

auto PluginsBundle::set_audios_provider(
    std::unique_ptr<IAudiosProviderWrapper,
                    std::function<void(IAudiosProviderWrapper *)>> new_provider)
    -> void {
    audios_provider_ = std::move(new_provider);
}

auto PluginsBundle::set_format_processor(
    std::unique_ptr<IFormatProcessorWrapper,
                    std::function<void(IFormatProcessorWrapper *)>>
        new_provider) -> void {
    format_processor_ = std::move(new_provider);
}

auto PluginsBundle::definitions_provider() -> IDefinitionsProviderWrapper * {
    return definitions_provider_.get();
}

auto PluginsBundle::sentences_provider() -> ISentencesProviderWrapper * {
    return sentences_provider_.get();
}

auto PluginsBundle::images_provider() -> IImagesProviderWrapper * {
    return images_provider_.get();
}

auto PluginsBundle::audios_provider() -> IAudiosProviderWrapper * {
    return audios_provider_.get();
}

auto PluginsBundle::format_processor() -> IFormatProcessorWrapper * {
    return format_processor_.get();
}
