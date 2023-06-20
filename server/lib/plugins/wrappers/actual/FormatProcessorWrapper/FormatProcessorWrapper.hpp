#ifndef FORMAT_PROCESSOR_WRAPPER_H
#define FORMAT_PROCESSOR_WRAPPER_H

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#include "BasePluginWrapper.hpp"
#include "IFormatProcessorWrapper.hpp"
#include "PyExceptionInfo.hpp"

class FormatProcessorWrapper : public IFormatProcessorWrapper,
                               public BasePluginWrapper {
 public:
    FormatProcessorWrapper(const FormatProcessorWrapper &);
    FormatProcessorWrapper(FormatProcessorWrapper &&) = default;
    auto operator=(const FormatProcessorWrapper &)
        -> FormatProcessorWrapper & = delete;
    auto operator=(FormatProcessorWrapper &&)
        -> FormatProcessorWrapper & = default;

    static auto build(const std::string           &name,
                      const boost::python::object &module)
        -> std::variant<FormatProcessorWrapper, PyExceptionInfo>;

    auto save(const ResultFilesPaths &paths)
        -> std::variant<FormatProcessorWrapper::type, PyExceptionInfo> override;

    [[nodiscard]] auto name() const -> const std::string & override;

 protected:
    struct FormatProcessorsFunctions {
        static auto build(const boost::python::object &module)
            -> std::variant<FormatProcessorsFunctions, PyExceptionInfo>;

        boost::python::object save;
    };

    FormatProcessorsFunctions specifics_;

 private:
    explicit FormatProcessorWrapper(BasePluginWrapper &&base);
};

static_assert(is_plugin_wrapper<FormatProcessorWrapper>);

#endif
