#pragma once
#include <stdexcept>

// Базовый класс исключений для компонентов (сканер, парсер, интерпретатор)
class ComponentException : public std::exception {
public:
  explicit ComponentException(const char *message);

  const char *what() const throw() override;

private:
  std::string message_;
};