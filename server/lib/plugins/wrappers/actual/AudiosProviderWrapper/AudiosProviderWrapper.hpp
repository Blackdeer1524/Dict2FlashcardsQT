#ifndef AUDIOS_PROVIDER_WRAPPER_H
#define AUDIOS_PROVIDER_WRAPPER_H

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "BasePluginWrapper.hpp"
#include "IAudiosProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"

class AudiosProviderWrapper : public BasePluginWrapper,
                              public IAudiosProviderWrapper {
 public:
    AudiosProviderWrapper(const AudiosProviderWrapper &);
    AudiosProviderWrapper(AudiosProviderWrapper &&) = default;
    auto operator=(const AudiosProviderWrapper &)
        -> AudiosProviderWrapper & = delete;
    auto operator=(AudiosProviderWrapper &&)
        -> AudiosProviderWrapper    & = default;
    ~AudiosProviderWrapper() override = default;

    static auto build(const std::string           &name,
                      const boost::python::object &module)
        -> std::variant<AudiosProviderWrapper, PyExceptionInfo>;

    auto get(const std::string &word, uint64_t batch_size, bool restart)
        -> std::variant<AudiosProviderWrapper::type,
                        std::string,
                        PyExceptionInfo> override;

    [[nodiscard]] auto name() const -> const std::string & override;

 protected:
    struct AudiosProvidesFunctions {
        static auto build(const boost::python::object &module)
            -> std::variant<AudiosProvidesFunctions, PyExceptionInfo>;

        boost::python::object get;
    };

    AudiosProvidesFunctions specifics_;

 private:
    explicit AudiosProviderWrapper(BasePluginWrapper &&base);

    std::unordered_map<std::string, boost::python::object> generators_;
};

static_assert(is_plugin_wrapper<AudiosProviderWrapper>);

#endif  // !AUDIOS_PROVIDER_WRAPPER_H
