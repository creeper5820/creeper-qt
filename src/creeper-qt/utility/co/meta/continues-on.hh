#pragma once

#include <qthread.h>

#include <coroutine>

#include "creeper-qt/utility/co/meta/cpo.hh"

namespace creeper::co {

struct continues_on_t : details::cpo_t {};

inline constexpr continues_on_t continues_on{};

template <std::derived_from<QObject> Context>
inline auto co_invoke(const continues_on_t&, Context* context) {
  struct awaiter {
    QObject* context;

    auto await_ready() const noexcept -> bool {
      return context->thread() == QThread::currentThread();
    }

    auto await_suspend(std::coroutine_handle<> handle) const {
      QMetaObject::invokeMethod(
          context, [handle] { handle.resume(); }, Qt::QueuedConnection);
    }

    auto await_resume() const noexcept -> void {}
  };

  return awaiter{context};
}

}  // namespace creeper::co
