#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"

#include <concepts>
#include <functional>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace creeper::group {

/// 占位基类：当布局类型 T 已经继承 DSL 时用它替代，避免重复的 DSL 基类。
struct NoDslBase { };

template <typename F, typename T>
concept foreach_invoke_item_trait = requires {
    { std::invoke(std::declval<F>(), std::declval<T const&>()) } -> widget_pointer_trait;
};

template <typename F, typename T>
concept foreach_apply_item_trait = requires {
    { std::apply(std::declval<F>(), std::declval<T const&>()) } -> widget_pointer_trait;
};

template <typename F, typename T>
concept foreach_item_trait = foreach_invoke_item_trait<F, T> || foreach_apply_item_trait<F, T>;

template <typename R, typename F>
concept foreach_invoke_ranges_trait = foreach_item_trait<F, std::ranges::range_value_t<R>>;

}

namespace creeper {

template <layout_trait T, widget_trait W>
class Group : public T,
              public std::conditional_t<std::derived_from<T, DSL>, group::NoDslBase, DSL> {
public:
    using T::T;

    explicit Group(auto&&... args) { this->construct_with(std::forward<decltype(args)>(args)...); }

    std::vector<W*> widgets;

    template <std::ranges::range R, typename F>
        requires group::foreach_invoke_ranges_trait<R, F>
    constexpr auto compose(const R& ranges, F&& f, Qt::Alignment a = { }) noexcept -> void {
        for (const auto& item : ranges) {
            using ItemT = decltype(item);

            auto widget_pointer = (W*) { };

            if constexpr (group::foreach_invoke_item_trait<F, ItemT>)
                widget_pointer = std::invoke(f, item);

            else if constexpr (group::foreach_apply_item_trait<F, ItemT>)
                widget_pointer = std::apply(f, item);

            if (widget_pointer != nullptr) {
                T::addWidget(widget_pointer, 0, a);
                widgets.push_back(widget_pointer);
            }
        }
    }

    auto foreach_(this auto&& self, auto&& f) noexcept
        requires std::invocable<decltype(f), W&>
    {
        for (auto widget : self.widgets)
            std::invoke(f, *widget);
    }
};

}

namespace creeper::group::pro {
/// @note
/// 一种典型的用法，委托构造时，所传函数只能接受常量引用，
/// 放心使用 auto，类型是可以被推导出来的
///
/// group::pro::Compose {
///     std::array {
///         std::tuple(1, "xxxxxx"),
///         ......
///     },
///     [](auto index, auto text) {
///         return new TextButton { ... };
///     },
/// }
///
template <typename R, typename F>
    requires group::foreach_invoke_ranges_trait<R, F>
struct Compose {
    const R& ranges;
    F method;
    Qt::Alignment alignment;

    explicit Compose(const R& r, F f, Qt::Alignment a = { }) noexcept
        : ranges { r }
        , method { std::move(f) }
        , alignment { a } { }

    friend auto dsl_invoke(auto& self, const Compose& prop) -> void {
        self.compose(prop.ranges, prop.method, prop.alignment);
    }
};
/// @note
/// 函数参数是组件的引用:
///
/// group::pro::Foreach { [](Widget& button) { ... } },
///
template <typename F>
    requires(!std::invocable<F>)
struct Foreach {
    F function;

    explicit Foreach(F&& f) noexcept
        : function { std::forward<F>(f) } { }

    friend auto dsl_invoke(auto& self, const Foreach& prop) -> void {
        // 很遗憾，Qt 占用了 foreach 这个单词
        self.foreach_(prop.function);
    }
};

using namespace api::scope::common;
}
