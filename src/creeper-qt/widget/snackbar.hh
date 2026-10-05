#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"  // IWYU pragma: keep

// TODO: 尚未实现。当前仅有占位类型 Snackbar 与 Message 结构，
//       绘制与交互待补全。
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"

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
    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

using SnackbarMessage = Message;

}
