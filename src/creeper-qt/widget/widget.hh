#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

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
using namespace common::pro;
using namespace widget::pro;
}
