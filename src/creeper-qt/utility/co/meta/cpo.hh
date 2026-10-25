#pragma once

#include <utility>

namespace creeper::co::details {

auto co_invoke() -> void;

template <class Cpo, class... Args>
concept co_invocable =
    requires(const Cpo& cpo, Args&&... args) { co_invoke(cpo, std::forward<Args>(args)...); };

struct cpo_t {
    template <class Self, class... Args>
        requires co_invocable<Self, Args...>
    constexpr auto operator()(this const Self& self, Args&&... args) noexcept(
        noexcept(co_invoke(self, std::forward<Args>(args)...))) -> decltype(auto) {
        return co_invoke(self, std::forward<Args>(args)...);
    }
};

}
