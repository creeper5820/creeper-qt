#pragma once

#include <qobject.h>

namespace creeper::qcontext {

template <std::derived_from<QObject> Context>
auto sleep_for(Context* context, std::chrono::milliseconds duration) noexcept {
  struct awaitor {
    Context* context;
    std::chrono::milliseconds ms;
  };
  return awaitor{context, duration};
}

}  // namespace creeper::qcontext
