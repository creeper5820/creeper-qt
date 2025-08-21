#pragma once
#include <atomic>
#include <concepts>

namespace creeper::animate {

struct StopToken final {

    explicit StopToken(bool token) noexcept
        : token { token } { }

    explicit operator bool(this auto&& self) noexcept {
        return self.token.load(std::memory_order::relaxed);
    }

    auto operator=(this StopToken& self, bool token) noexcept -> StopToken& {
        self.token.store(token, std::memory_order::relaxed);
        return self;
    }

    std::atomic<bool> token;
};

struct IEvent {
    virtual auto update() -> void = 0;
};

template <std::invocable F>
class Controller final {
public:
    explicit Controller(F&& f) noexcept
        : update_callback { f } { }

private:
    F update_callback;
};

inline auto foo() { }

}
