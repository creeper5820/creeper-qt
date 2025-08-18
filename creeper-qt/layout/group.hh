#pragma once
#include "creeper-qt/utility/trait.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

namespace creeper::group::internal {

template <typename F, typename T>
concept foreach_item_trait = requires(F f) {
    { f(T {}) } -> widget_pointer_trait;
};

template <layout_trait T, widget_trait W> struct Group : public T {
    using T::T;

    template <std::ranges::range R, typename F>
        requires foreach_item_trait<F, std::ranges::range_value_t<R>>
    auto compose(R&& ranges, F&& f) noexcept -> void {
        for (auto&& v : std::forward<R>(ranges))
            T::addWidget(std::forward<F>(f)(std::forward<decltype(v)>(v)));
    }

    auto foreach_(std::invocable<W&> auto&& f) -> void { }
};

};

namespace creeper::group::pro {

using Token = common::Token<internal::Group<QLayout, QWidget>>;

template <std::ranges::range R, typename F>
    requires internal::foreach_item_trait<F, std::ranges::range_value_t<R>>
struct Compose : Token {
    R&& r;
    F&& f;

    explicit Compose(R&& r, F&& f) noexcept
        : r { std::forward<R>(r) }
        , f { std::forward<F>(f) } { }

    auto apply(auto& self) const noexcept -> void {
        self.compose(std::forward<R>(r), std::forward<F>(f));
    }
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
