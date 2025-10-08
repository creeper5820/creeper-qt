#pragma once
#include "creeper-qt/utility/wrapper/widget.hh"
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

    auto attach_callback_to_mutable(auto& widget) noexcept {
        struct Functor : MutableValue<T>::Functor {
            decltype(widget) w;
            Functor(decltype(w) w) noexcept
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

template <typename T, typename F>
struct MutableTransform : widget::pro::Token {
    F apply_function;
    MutableValue<T>& mutable_value;

    explicit MutableTransform(F f, MutableValue<T>& t) noexcept
        : mutable_value { t }
        , apply_function { std::move(f) } { }

    template <template <typename> class Smart>
    explicit MutableTransform(F f, const Smart<MutableValue<T>>& m) noexcept
        : MutableTransform { std::move(f), *m } { }

    auto attach_callback_to_mutable(auto& widget) noexcept {
        struct Functor : MutableValue<T>::Functor {
            decltype(widget) w;
            F apply_function;
            Functor(decltype(widget) w, F f) noexcept
                : w { w }
                , apply_function { std::move(f) } { }
            ~Functor() noexcept = default;
            auto update(const T& value) -> void override {
                apply_function(w, value);
                w.update();
            }
        };
        auto functor = std::make_unique<Functor>(widget, std::move(apply_function));

        mutable_value.callbacks[&widget] = std::move(functor);

        auto alive = std::weak_ptr { mutable_value.alive };
        QObject::connect(&widget, &QObject::destroyed, [this, alive](auto* key) {
            if (alive.lock()) mutable_value.callbacks.erase(key);
        });
    }
    auto apply(auto& widget) noexcept -> void {
        apply_function(widget, mutable_value.get());
        attach_callback_to_mutable(widget);
    }
};

using MutableQString = MutableValue<QString>;
using MutableDouble  = MutableValue<double>;
using MutableFloat   = MutableValue<float>;
using MutableUInt8   = MutableValue<uint8_t>;
using MutableUInt16  = MutableValue<uint16_t>;
using MutableInt8    = MutableValue<int8_t>;
using MutableInt16   = MutableValue<int16_t>;

}
