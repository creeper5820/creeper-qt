#pragma once

#include <coroutine>
#include <optional>
#include <utility>

namespace creeper::co {

template <typename R>
struct continuation {
  continuation() = default;

  continuation(const continuation&) = delete;
  continuation& operator=(const continuation&) = delete;

  continuation(continuation&& other) noexcept
      : handle{std::exchange(other.handle, {})},
        result{std::move(other.result)} {}
  continuation& operator=(continuation&& other) noexcept {
    if (this != &other) {
      handle = std::exchange(other.handle, {});
      result = std::move(other.result);
    }
    return *this;
  }

  std::coroutine_handle<> handle;
  std::optional<R> result;

  auto await_ready() const noexcept { return false; }

  auto await_suspend(std::coroutine_handle<> handle) noexcept {
    continuation::handle = handle;
  }
  auto await_resume() noexcept { return std::move(result); }

  auto resume(R result) noexcept {
    if (handle) {
      continuation::result.emplace(std::move(result));
      std::exchange(handle, {}).resume();
    }
  }
};

}  // namespace creeper::co
