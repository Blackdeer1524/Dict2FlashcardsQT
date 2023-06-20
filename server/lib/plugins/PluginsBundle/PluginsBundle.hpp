#ifndef PLUGINS_BUNDLE_H
#define PLUGINS_BUNDLE_H

#include "IAudiosProviderWrapper.hpp"
#include "IDefinitionsProviderWrapper.hpp"
#include "IFormatProcessorWrapper.hpp"
#include "IImagesProviderWrapper.hpp"
#include "ISentencesProviderWrapper.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

class PluginsBundle {
 public:
    PluginsBundle();

    auto set_definitions_provider(
        std::unique_ptr<IDefinitionsProviderWrapper,
                        std::function<void(IDefinitionsProviderWrapper *)>>
            new_provider) -> void;

    auto set_sentences_provider(
        std::unique_ptr<ISentencesProviderWrapper,
                        std::function<void(ISentencesProviderWrapper *)>>
            new_provider) -> void;

    auto set_images_provider(
        std::unique_ptr<IImagesProviderWrapper,
                        std::function<void(IImagesProviderWrapper *)>>
            new_provider) -> void;

    auto set_audios_provider(
        std::unique_ptr<IAudiosProviderWrapper,
                        std::function<void(IAudiosProviderWrapper *)>>
            new_provider) -> void;

    auto set_format_processor(
        std::unique_ptr<IFormatProcessorWrapper,
                        std::function<void(IFormatProcessorWrapper *)>>
            new_provider) -> void;

    auto definitions_provider() -> IDefinitionsProviderWrapper *;

    auto sentences_provider() -> ISentencesProviderWrapper *;

    auto images_provider() -> IImagesProviderWrapper *;

    auto audios_provider() -> IAudiosProviderWrapper *;

    auto format_processor() -> IFormatProcessorWrapper *;

 private:
    std::unique_ptr<IDefinitionsProviderWrapper,
                    std::function<void(IDefinitionsProviderWrapper *)>>
        definitions_provider_ = nullptr;
    std::unique_ptr<ISentencesProviderWrapper,
                    std::function<void(ISentencesProviderWrapper *)>>
        sentences_provider_ = nullptr;
    std::unique_ptr<IImagesProviderWrapper,
                    std::function<void(IImagesProviderWrapper *)>>
        images_provider_ = nullptr;
    std::unique_ptr<IAudiosProviderWrapper,
                    std::function<void(IAudiosProviderWrapper *)>>
        audios_provider_ = nullptr;
    std::unique_ptr<IFormatProcessorWrapper,
                    std::function<void(IFormatProcessorWrapper *)>>
        format_processor_ = nullptr;
};

#endif  // !PLUGINS_BUNDLE_H
