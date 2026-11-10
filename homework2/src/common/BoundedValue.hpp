#pragma once

#include <cmath>
#include <compare>
#include <limits>
#include <string>
#include <type_traits>

#include "common/Errors.hpp"

/*
По заданию нужно сделать шаблонный класс и реально его использовать. Авторский
пример с registry мне не понравился прям очень, потому что в моей архитектуре
можно хранить просто вектор указателей на Item и бед не знать, там шаблоны
вообще не нужны. Поэтому я сделал шаблонный красс, который хранит значение и
проверяет его корректность
*/
template <typename T>
class BoundedValue {
 private:
  const T value_;

 public:
  explicit BoundedValue(T value, T min, T max, const std::string& errorMessage)
      : value_(value) {
    bool valid = min <= max && value_ >= min && value_ <= max;

    if constexpr (std::is_floating_point_v<T>) {
      valid = valid && std::isfinite(value_) && std::isfinite(min) &&
              std::isfinite(max);
    }

    if (!valid) {
      throw InvalidArgumentError(errorMessage);
    }
  }

  // Ещё один конструктор для применения неявных преобразований
  BoundedValue(T value)
      : BoundedValue(value, T{0}, std::numeric_limits<T>::max(),
                     "Invalid value") {}

  T Get() const { return value_; }

  auto operator<=>(const BoundedValue& other) const = default;

  auto operator<=>(const T& other) const { return value_ <=> other; }

  template <typename U>
    requires std::is_arithmetic_v<U>
  auto operator+(U rhs) const {
    return value_ + rhs;
  }

  template <typename U>
    requires std::is_arithmetic_v<U>
  auto operator-(U rhs) const {
    return value_ - rhs;
  }

  template <typename U>
    requires std::is_arithmetic_v<U>
  auto operator*(U rhs) const {
    return value_ * rhs;
  }

  template <typename U>
    requires std::is_arithmetic_v<U>
  auto operator/(U rhs) const {
    if (rhs == 0) {
      throw InvalidArgumentError("Division by zero");
    }
    return value_ / rhs;
  }
};
