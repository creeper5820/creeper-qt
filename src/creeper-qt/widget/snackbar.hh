#pragma once

// TODO: 尚未实现。当前仅有占位类型 Snackbar 与 Message 结构，
//       绘制与交互待补全。
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <string>
#include <utility>

namespace creeper {

struct Message {
    std::string text;
};

class Snackbar : public QWidget, public DSL {
public:
    using QWidget::QWidget;

    explicit Snackbar(auto&&... props) { construct_with(std::forward<decltype(props)>(props)...); }
};

namespace snackbar::pro {

    using namespace common::pro;
    using namespace widget::pro;
    using namespace theme::pro;

}

using SnackbarMessage = Message;

}
