#pragma once

#include "creeper-qt/details/dsl/core.hh"
#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep

namespace creeper {

class Widget : public QWidget, public DSL {
public:
    using QWidget::QWidget;

    explicit Widget(auto&&... props)
        : Widget { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

}

namespace creeper::widget::pro {
using namespace api::scope::common;
using namespace api::scope::widget;
}
