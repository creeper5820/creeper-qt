#pragma once
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/layout.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <concepts>
#include <qstackedlayout.h>

namespace creeper::stacked::internal {
class Stacked : public QStackedLayout { };
}

namespace creeper::stacked::pro {

using Token = common::Token<internal::Stacked>;

template <typename F>
using IndexChanged = common::pro::SignalInjection<F, Token, &internal::Stacked::currentChanged>;

using Index = common::pro::Index<Token>;

template <typename T>
concept trait = std::derived_from<T, Token> || layout::pro::trait<T>;

CREEPER_DEFINE_CHECKER(trait);
using namespace layout::pro;

template <item_trait T>
struct Item : Token {
    T* item_pointer = nullptr;

    explicit Item(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit Item(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    void apply(stacked_trait auto& layout) const {
        if constexpr (widget_trait<T>) {
            layout.addWidget(item_pointer);
        }
    }
};
}

namespace creeper {
using Stacked = Declarative<stacked::internal::Stacked, stacked::pro::checker>;
using NavHost = Stacked;
}
