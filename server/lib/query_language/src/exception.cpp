#include "exception.hpp"
#include <cctype>

ComponentException::ComponentException(const char *message)
    : std::runtime_error(message), message_(message) {
}

const char *ComponentException::what() const throw() {
    return message_.c_str();
}