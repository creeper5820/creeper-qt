#pragma once

#include "creeper-qt/utility/api/helper/signal-injection.hh"
#include "creeper-qt/utility/api/scope/common.hh"
#include "creeper-qt/utility/api/scope/layout.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"

#include <concepts>
#include <qstackedlayout.h>

namespace creeper {

class Stacked : public QStackedLayout, public DSL {
public:
    using QStackedLayout::QStackedLayout;

    explicit Stacked(auto&&... props) { construct_with(std::forward<decltype(props)>(props)...); }
};

namespace stacked::pro {
    /// @note: currentChanged(int index)
    template <typename F>
    using IndexChanged = api::helper::SignalInjection<F, &Stacked::currentChanged>;
    using CurrentIndex = ForwardProp<&Stacked::setCurrentIndex>;
    template <item_trait T>
    struct Item {
        T* item_pointer = nullptr;

        explicit Item(T* pointer) noexcept
            : item_pointer { pointer } { }

        explicit Item(auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

        friend auto dsl_invoke(Stacked& layout, const Item& prop) -> void {
            if constexpr (widget_trait<T>) {
                layout.addWidget(prop.item_pointer);
            }
        }
    };

    using namespace api::scope::common;
    using namespace api::scope::layout;
}

using NavHost = Stacked;

namespace nav_host::pro {
    using namespace stacked::pro;
}

}
