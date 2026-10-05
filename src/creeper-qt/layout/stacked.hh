#pragma once

#include "creeper-qt/utility/api/helper/signal-injection.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/layout.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"

#include <qstackedlayout.h>

namespace creeper {

class Stacked : public QStackedLayout, public DSL {
public:
    using QStackedLayout::QStackedLayout;

    explicit Stacked(auto&&... props) { construct_with(std::forward<decltype(props)>(props)...); }

private:
    template <widget_pointer_trait W>
    friend auto dsl_invoke(Stacked& self, W widget) {
        self.addWidget(widget);
    }
};

namespace stacked::pro {
    /// @note: currentChanged(int index)
    template <typename F>
    using IndexChanged = api::helper::SignalInjection<F, &Stacked::currentChanged>;
    using CurrentIndex = ForwardProp<&Stacked::setCurrentIndex>;

    using namespace api::scope::common;
    using namespace api::scope::layout;
}

using NavHost = Stacked;

namespace nav_host::pro {
    using namespace stacked::pro;
}

}
