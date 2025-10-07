#pragma once
#include "creeper-qt/utility/wrapper/property.hh"
#include <qobject.h>
#include <unordered_map>

namespace creeper {

template <typename T>
struct MutableValue final {
    T value;

    struct Functor {
        virtual ~Functor() noexcept   = default;
        virtual void update(const T&) = 0;
    };
    std::unordered_map<QObject*, std::unique_ptr<Functor>> callbacks;

    struct Nothing { };
    std::shared_ptr<Nothing> alive = std::make_shared<Nothing>();

    MutableValue(const MutableValue&) = delete;
    MutableValue(MutableValue&&)      = delete;

    template <typename... Args>
    explicit MutableValue(Args&&... args) noexcept
        requires std::constructible_from<T, Args&&...>
        : value { std::forward<Args>(args)... } { }

    constexpr auto get() const noexcept -> T { return value; }
    constexpr operator T() const noexcept { return get(); }

    template <typename U>
    auto set(U&& u) noexcept -> void
        requires requires(T& t, U&& u) { t = std::forward<U>(u); }
    {
        value = std::forward<U>(u);
        for (const auto& [_, f] : callbacks)
            f->update(value);
    }
    template <typename U>
    auto set_silent(U&& u) noexcept -> void
        requires requires(T& t, U&& u) { t = std::forward<U>(u); }
    {
        value = std::forward<U>(u);
    }
    template <typename U>
    auto operator=(U&& u) noexcept -> void
        requires requires(T& t, U&& u) { t = std::forward<U>(u); }
    {
        set(std::forward<U>(u));
    }
};

template <class P, typename T>
struct MutableForward final : public P {

    MutableValue<T>& mutable_value;

    explicit MutableForward(MutableValue<T>& m) noexcept
        requires std::constructible_from<P, T>
        : mutable_value { m }
        , P { m.get() } { }

    template <template <typename> class Smart>
    explicit MutableForward(const Smart<MutableValue<T>>& m) noexcept
        requires std::constructible_from<P, T>
        : MutableForward { *m } { }

    explicit MutableForward(const P&, MutableValue<T>& m) noexcept
        requires std::constructible_from<P, T>
        : MutableForward { m } { }

    template <template <typename> class Smart>
    explicit MutableForward(const P&, const Smart<MutableValue<T>>& m) noexcept
        requires std::constructible_from<P, T>
        : MutableForward { m } { }

    template <typename W, typename C>
    auto attach_callback_to_mutable(Declarative<W, C>& widget) noexcept {
        struct Functor : MutableValue<T>::Functor {
            Declarative<W, C>& w;
            Functor(Declarative<W, C>& w) noexcept
                : w { w } { }
            ~Functor() noexcept = default;
            auto update(const T& value) -> void override {
                w.apply(P { value });
                w.update();
            }
        };
        auto functor = std::make_unique<Functor>(widget);

        mutable_value.callbacks[&widget] = std::move(functor);

        auto alive = std::weak_ptr { mutable_value.alive };
        QObject::connect(&widget, &QObject::destroyed, [this, alive](auto* key) {
            if (alive.lock()) mutable_value.callbacks.erase(key);
        });
    }
    auto apply(auto& widget) noexcept -> void {
        P::apply(widget);
        attach_callback_to_mutable(widget);
    }
};

}
