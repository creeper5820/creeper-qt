#pragma once

#include <coroutine>
#include <exception>
#include <expected>
#include <utility>

namespace creeper::co {

template <typename R>
struct result {
  std::expected<R, std::exception_ptr> value;

  template <typename Var>
  auto return_value(Var&& var) {
    value.emplace(std::forward<Var>(var));
  }
};
template <>
struct result<void> {
  std::expected<void, std::exception_ptr> value;

  auto return_void() noexcept {}
};

template <typename R = void>
struct coroutine {
 public:
  struct promise_type : result<R> {
    std::coroutine_handle<> continuation;

    auto get_return_object() noexcept {
      return coroutine{handle_type::from_promise(*this)};
    }
    auto unhandled_exception() noexcept {
      result<R>::value = std::unexpected{std::current_exception()};
    }

    constexpr auto initial_suspend() const noexcept {
      return std::suspend_always{};
    }

    constexpr auto final_suspend() const noexcept {
      struct awaitor {
        auto await_suspend(handle_type handle) const noexcept {
          auto& continuation = handle.promise().continuation;
          return continuation ? continuation : std::noop_coroutine();
        }

        constexpr auto await_ready() const noexcept { return false; }
        constexpr auto await_resume() const noexcept {}
      };
      return awaitor{};
    }
  };
  using handle_type = std::coroutine_handle<promise_type>;

 public:
  explicit coroutine() = default;
  explicit coroutine(handle_type handle) noexcept : handle{handle} {}

  coroutine(const coroutine&) = delete;
  coroutine& operator=(const coroutine&) = delete;

  coroutine(coroutine&& other) noexcept
      : handle{std::exchange(other.handle, nullptr)} {}
  coroutine& operator=(coroutine&& other) noexcept {
    if (this != &other) {
      destroy();
      handle = std::exchange(other.handle, nullptr);
    }
    return *this;
  }

  ~coroutine() { destroy(); }

  auto operator co_await() const noexcept {
    struct awaitor {
      handle_type handle;

      auto await_ready() const noexcept { return !handle || handle.done(); }

      auto await_suspend(std::coroutine_handle<> parent) const noexcept {
        handle.promise().continuation = parent;
        return handle;
      }

      auto await_resume() const { return std::move(handle.promise().value); }
    };
    return awaitor{handle};
  }

  auto done() const noexcept {
    // dead or done
    return (!handle || handle.done());
  }
  auto resume() const {
    if (handle) {
      handle.resume();
    }
  }

  handle_type handle;

 private:
  auto destroy() noexcept {
    if (handle) {
      handle.destroy();
      handle = nullptr;
    }
  }
};
}  // namespace creeper::co
