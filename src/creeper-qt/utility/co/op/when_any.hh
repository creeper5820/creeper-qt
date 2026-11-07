#pragma once

#include <cstdint>
#include <exception>
#include <expected>
#include <variant>

namespace creeper::co {

struct result_t {
  using union_t = std::variant<int, bool>;

  std::uint8_t task_id;
  std::expected<union_t, std::exception_ptr> result;

  operator std::uint8_t() { return task_id; }

  template <std::size_t Id>
  auto value() -> decltype(auto) {
    // using Id to compute the type in compile-time, and return
    // std::expected<true_type, std::exception_ptr>
    if constexpr (Id == 1) {
      return std::expected<int, std::exception_ptr>{0};
    } else if constexpr (Id == 2) {
      return std::expected<bool, std::exception_ptr>{false};
    }
  }

  template <std::size_t Id>
  auto on(auto&& lambda) {
    return *this;
  }
};

template <class... Ts>
struct when_any {
  when_any(Ts...) {}

  auto result() { return result_t{}; }
};

}  // namespace creeper::co

inline auto foo() {
  using namespace creeper;

  auto race = /* co_await */ co::when_any{
      // task1()
      // task2()
      // ......
  };
  auto result = race.result();

  switch (result) {
    case 1: {
      auto value = result.value<1>();
      break;
    }
    case 2: {
      auto value = result.value<2>();
      break;
    }
  }
}
