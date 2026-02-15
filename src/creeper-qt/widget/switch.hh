#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qabstractbutton.h>

namespace creeper {
namespace _switch::internal {
    class Switch : public QAbstractButton {
        CREEPER_PIMPL_DEFINITION(Switch)

    public:
        void set_color_scheme(const ColorScheme&);
        void load_theme_manager(ThemeManager&);

        void set_disabled(bool);
        bool disabled() const;

        void set_checked(bool);
        bool checked() const;

        void set_track_color_unchecked(const QColor&);
        void set_track_color_checked(const QColor&);
        void set_track_color_unchecked_disabled(const QColor&);
        void set_track_color_checked_disabled(const QColor&);

        void set_handle_color_unchecked(const QColor&);
        void set_handle_color_checked(const QColor&);
        void set_handle_color_unchecked_disabled(const QColor&);
        void set_handle_color_checked_disabled(const QColor&);

        void set_outline_color_unchecked(const QColor&);
        void set_outline_color_checked(const QColor&);
        void set_outline_color_unchecked_disabled(const QColor&);
        void set_outline_color_checked_disabled(const QColor&);

        void set_hover_color_unchecked(const QColor&);
        void set_hover_color_checked(const QColor&);

    protected:
        // 添加 Hover 动画
        void enterEvent(qt::EnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

        // 实现视觉效果
        void paintEvent(QPaintEvent* event) override;
    };
}
namespace _switch::pro {

    using Token = common::Token<internal::Switch>;

    /// @note 碎碎念，这么多颜色，真的会用得上么...

    using TrackColorUnchecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_track_color_unchecked(v); }>;

    using TrackColorChecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_track_color_checked(v); }>;

    using TrackColorUncheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_track_color_unchecked_disabled(v); }>;

    using TrackColorCheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_track_color_checked_disabled(v); }>;

    using HandleColorUnchecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_handle_color_unchecked(v); }>;

    using HandleColorChecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_handle_color_checked(v); }>;

    using HandleColorUncheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_handle_color_unchecked_disabled(v); }>;

    using HandleColorCheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_handle_color_checked_disabled(v); }>;

    using OutlineColorUnchecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_outline_color_unchecked(v); }>;

    using OutlineColorChecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_outline_color_checked(v); }>;

    using OutlineColorUncheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_outline_color_unchecked_disabled(v); }>;

    using OutlineColorCheckedDisabled = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_outline_color_checked_disabled(v); }>;

    using HoverColorUnchecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_hover_color_unchecked(v); }>;

    using HoverColorChecked = SetterProp<Token, QColor,
        [](auto& self, const QColor& v) { self.set_hover_color_checked(v); }>;

    template <typename Callback>
    using Clickable = common::pro::Clickable<Callback, Token>;

    using Disabled = common::pro::Disabled<Token>;
    using Checked  = common::pro::Checked<Token>;

    template <class Switch>
    concept trait = std::derived_from<Switch, Token>;

    CREEPER_DEFINE_CHECKER(trait);
    using namespace theme::pro;
    using namespace widget::pro;
}
/// @note 使用时建议比例 w : h > 7 : 4 ，过冲动画会多占用一些宽度，倘若 w 过短，可能会出现 hover
/// 层画面被截断的情况
using Switch = Declarative<_switch::internal::Switch,
    CheckerOr<_switch::pro::checker, widget::pro::checker, theme::pro::checker>>;
}
