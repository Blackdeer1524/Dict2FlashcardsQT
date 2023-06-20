#ifndef DEFINITIONS_PROVIDER_WRAPPER_H
#define DEFINITIONS_PROVIDER_WRAPPER_H

#include <boost/python/errors.hpp>
#include <boost/python/extract.hpp>
#include <cstdint>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "BasePluginWrapper.hpp"
#include "IDefinitionsProviderWrapper.hpp"
#include "PyExceptionInfo.hpp"

class DefinitionsProviderWrapper : public IDefinitionsProviderWrapper,
                                   public BasePluginWrapper {
 public:
    DefinitionsProviderWrapper(const DefinitionsProviderWrapper &);
    DefinitionsProviderWrapper(DefinitionsProviderWrapper &&) = default;
    auto operator=(const DefinitionsProviderWrapper &)
        -> DefinitionsProviderWrapper & = delete;
    auto operator=(DefinitionsProviderWrapper &&)
        -> DefinitionsProviderWrapper & = default;

    static auto build(const std::string           &name,
                      const boost::python::object &module)
        -> std::variant<DefinitionsProviderWrapper, PyExceptionInfo>;

    auto get_dictionary_scheme()
        -> std::variant<nlohmann::json, PyExceptionInfo> override;

    auto get(const std::string &word, uint64_t batch_size, bool restart)
        -> std::variant<DefinitionsProviderWrapper::type,
                        std::string,
                        PyExceptionInfo> override;

    [[nodiscard]] auto name() const -> const std::string & override;

 protected:
    struct DefinitionsProvidersFunctions {
        static auto build(const boost::python::object &module)
            -> std::variant<DefinitionsProvidersFunctions, PyExceptionInfo>;

        boost::python::object get;
    };

    DefinitionsProvidersFunctions specifics_;

 private:
    explicit DefinitionsProviderWrapper(BasePluginWrapper &&base);

    std::unordered_map<std::string, boost::python::object> generators_;
};

static_assert(is_plugin_wrapper<DefinitionsProviderWrapper>);

#endif  // !DEFINITIONS_PROVIDER_WRAPPER_H
