#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/widget.hh"

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
        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        // 实现视觉效果
        void paintEvent(QPaintEvent* event) override;
    };
}
namespace _switch::pro {

    using Token = common::Token<internal::Switch>;

    /// @note 碎碎念，这么多颜色，真的会用得上么...

    struct TrackColorUnchecked : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_unchecked(*this); }
    };
    struct TrackColorChecked : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_checked(*this); }
    };
    struct TrackColorUncheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_unchecked_disabled(*this); }
    };
    struct TrackColorCheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_checked_disabled(*this); }
    };

    struct HandleColorUnchecked : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_unchecked(*this); }
    };
    struct HandleColorChecked : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_checked(*this); }
    };
    struct HandleColorUncheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_unchecked_disabled(*this); }
    };
    struct HandleColorCheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_checked_disabled(*this); }
    };

    struct OutlineColorUnchecked : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_unchecked(*this); }
    };
    struct OutlineColorChecked : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_checked(*this); }
    };
    struct OutlineColorUncheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_unchecked_disabled(*this); }
    };
    struct OutlineColorCheckedDisabled : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_checked_disabled(*this); }
    };

    struct HoverColorUnchecked : public QColor, Token {
        using QColor::QColor;
        explicit HoverColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_hover_color_unchecked(*this); }
    };
    struct HoverColorChecked : public QColor, Token {
        using QColor::QColor;
        explicit HoverColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_hover_color_checked(*this); }
    };
    template <typename Callback> using Clickable = common::pro::Clickable<Callback, Token>;

    template <class Switch>
    concept trait = std::derived_from<Switch, Token> //
        || widget::pro::trait<Switch>                        //
        || theme::pro::trait<Switch>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = trait<T>;
        };
    };

    using Disabled = common::pro::Disabled<Token>;
    using Checked  = common::pro::Checked<Token>;

    using namespace theme::pro;
    using namespace widget::pro;
}
/// @note 使用时建议比例 w : h > 7 : 4 ，过冲动画会多占用一些宽度，倘若 w 过短，可能会出现 hover
/// 层画面被截断的情况
using Switch = Declarative<_switch::internal::Switch, _switch::pro::checker>;
}
