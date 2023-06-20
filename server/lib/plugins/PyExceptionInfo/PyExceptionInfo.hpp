#ifndef PY_EXCEPTION_INFO_H
#define PY_EXCEPTION_INFO_H

#include <boost/python/object_fwd.hpp>
#include <optional>
#include <string>
#include <utility>

class PyExceptionInfo {
 public:
    static auto               build() -> std::optional<PyExceptionInfo>;

    [[nodiscard]] inline auto last_type() const -> const std::string & {
        return last_type_;
    }

    [[nodiscard]] inline auto last_value() const -> const std::string & {
        return last_value_;
    }

    [[nodiscard]] inline auto last_traceback() const -> const std::string & {
        return last_traceback_;
    }

    [[nodiscard]] inline auto stack_trace() const -> const std::string & {
        return stack_trace_;
    }

 private:
    std::string last_type_;
    std::string last_value_;
    std::string last_traceback_;
    std::string stack_trace_;

    PyExceptionInfo() = default;
};

#endif  // !PY_EXCEPTION_INFO_H
