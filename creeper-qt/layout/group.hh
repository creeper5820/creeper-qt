#pragma once
#include "creeper-qt/utility/trait.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

namespace creeper::group::internal {

template <typename F, typename T>
concept foreach_item_trait = requires(F f) {
    { std::invoke(f, T {}) } -> widget_pointer_trait;
} || requires(F f) {
    { std::apply(f, T {}) } -> widget_pointer_trait;
};

template <typename R, typename F>
concept foreach_invoke_trait =
    std::ranges::range<R> && foreach_item_trait<F, std::ranges::range_value_t<R>>;

template <layout_trait T, widget_trait W> struct Group : public T {
    using T::T;
    std::vector<W*> widgets;

    template <std::ranges::range R, typename F>
        requires foreach_invoke_trait<R, F>
    auto compose(R&& ranges, F&& f) noexcept -> void {
        for (auto&& v : std::forward<R>(ranges)) {
            auto w = std::forward<F>(f)(std::forward<decltype(v)>(v));
            T::addWidget(w);
            widgets.push_back(w);
        }
    }
    auto foreach_(this auto&& self, auto&& f) noexcept
        requires std::invocable<decltype(f), W&>
    {
        for (auto widget : self.widgets)
            std::invoke(f, *widget);
    }
};

};

namespace creeper::group::pro {

using Token = common::Token<internal::Group<QLayout, QWidget>>;

template <typename R, typename F> struct Compose : Token {
    const R& r;
    const F& f;

    explicit Compose(const R& r, const F& f) noexcept
        requires internal::foreach_invoke_trait<R, F>
        : r { r }
        , f { f } { }

    auto apply(auto& self) const noexcept { self.compose(r, f); }
};

/// @tparam F [](auto& widget){ ... }
template <typename F> struct Foreach : Token {
    const F& f;

    explicit Foreach(const F& f) noexcept
        requires(!std::invocable<F>)
        : f { f } { }

    auto apply(auto& self) const noexcept { self.foreach_(f); }
};

template <class T>
concept trait = std::derived_from<T, Token>;

template <layout_trait L> struct checker final {
    template <class T> struct result {
        static constexpr auto v = trait<T> //
            || L::checker::template result<T>::v;
    };
};

};

namespace creeper {

template <layout_trait T, widget_trait W>
using Group = Declarative<group::internal::Group<T, W>, group::pro::checker<T>>;

}
