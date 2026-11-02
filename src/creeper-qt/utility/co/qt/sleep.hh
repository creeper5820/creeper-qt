#pragma once

#include <qobject.h>
#include <qtimer.h>

#include <coroutine>

namespace creeper::qcontext {

template <std::derived_from<QObject> Context>
auto sleep_for(Context* context, std::chrono::milliseconds duration) noexcept {
  struct awaitor {
    Context* context;
    std::chrono::milliseconds ms;

    auto await_ready() const noexcept {
      return ms <= std::chrono::milliseconds::zero();
    }
    auto await_suspend(std::coroutine_handle<> handle) const noexcept {
      QTimer::singleShot(ms, context, [=] { handle.resume(); });
    }
    auto await_resume() const noexcept {}
  };
  return awaitor{context, duration};
}

}  // namespace creeper::qcontext
