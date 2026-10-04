#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qabstractbutton.h>

namespace creeper {

class Switch : public QAbstractButton, public DSL {
    CREEPER_PIMPL_DEFINITION(Switch)

public:
    explicit Switch(auto&&... props)
        : Switch { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;
    auto bindThemeManager(ThemeManager&) -> void;

    auto setDisabled(bool) -> void;
    auto disabled() const -> bool;

    auto setChecked(bool) -> void;
    auto checked() const -> bool;

    auto setTrackColorUnchecked(const QColor&) -> void;
    auto setTrackColorChecked(const QColor&) -> void;
    auto setTrackColorUncheckedDisabled(const QColor&) -> void;
    auto setTrackColorCheckedDisabled(const QColor&) -> void;

    auto setHandleColorUnchecked(const QColor&) -> void;
    auto setHandleColorChecked(const QColor&) -> void;
    auto setHandleColorUncheckedDisabled(const QColor&) -> void;
    auto setHandleColorCheckedDisabled(const QColor&) -> void;

    auto setOutlineColorUnchecked(const QColor&) -> void;
    auto setOutlineColorChecked(const QColor&) -> void;
    auto setOutlineColorUncheckedDisabled(const QColor&) -> void;
    auto setOutlineColorCheckedDisabled(const QColor&) -> void;

    auto setHoverColorUnchecked(const QColor&) -> void;
    auto setHoverColorChecked(const QColor&) -> void;

protected:
    // 添加 Hover 动画
    auto enterEvent(qt::EnterEvent* event) -> void override;
    auto leaveEvent(QEvent* event) -> void override;

    // 实现视觉效果
    auto paintEvent(QPaintEvent* event) -> void override;
};

namespace _switch::pro {

    using namespace common::pro;
    using namespace widget::pro;
    using namespace theme::pro;

    /// @note 碎碎念，这么多颜色，真的会用得上么...

    using TrackColorUnchecked = ForwardProp<&Switch::setTrackColorUnchecked>;
    using TrackColorChecked   = ForwardProp<&Switch::setTrackColorChecked>;

    using TrackColorUncheckedDisabled = ForwardProp<&Switch::setTrackColorUncheckedDisabled>;
    using TrackColorCheckedDisabled   = ForwardProp<&Switch::setTrackColorCheckedDisabled>;

    using HandleColorUnchecked = ForwardProp<&Switch::setHandleColorUnchecked>;
    using HandleColorChecked   = ForwardProp<&Switch::setHandleColorChecked>;

    using HandleColorUncheckedDisabled = ForwardProp<&Switch::setHandleColorUncheckedDisabled>;
    using HandleColorCheckedDisabled   = ForwardProp<&Switch::setHandleColorCheckedDisabled>;

    using OutlineColorUnchecked = ForwardProp<&Switch::setOutlineColorUnchecked>;
    using OutlineColorChecked   = ForwardProp<&Switch::setOutlineColorChecked>;

    using OutlineColorUncheckedDisabled = ForwardProp<&Switch::setOutlineColorUncheckedDisabled>;
    using OutlineColorCheckedDisabled   = ForwardProp<&Switch::setOutlineColorCheckedDisabled>;

    using HoverColorUnchecked = ForwardProp<&Switch::setHoverColorUnchecked>;
    using HoverColorChecked   = ForwardProp<&Switch::setHoverColorChecked>;

}
/// @note 使用时建议比例 w : h > 7 : 4 ，过冲动画会多占用一些宽度，倘若 w 过短，可能会出现 hover
/// 层画面被截断的情况
}
