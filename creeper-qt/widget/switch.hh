#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common-property.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/widget.hh"

#include <qabstractbutton.h>

namespace creeper {
class Switch;

namespace _switch::internal {
    class Switch : public QAbstractButton {
        CREEPER_PIMPL_DEFINTION(Switch)

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

    struct TrackColorUnchecked final : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_unchecked(*this); }
    };
    struct TrackColorChecked final : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_checked(*this); }
    };
    struct TrackColorUncheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_unchecked_disabled(*this); }
    };
    struct TrackColorCheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit TrackColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_track_color_checked_disabled(*this); }
    };

    struct HandleColorUnchecked final : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_unchecked(*this); }
    };
    struct HandleColorChecked final : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_checked(*this); }
    };
    struct HandleColorUncheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_unchecked_disabled(*this); }
    };
    struct HandleColorCheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit HandleColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_handle_color_checked_disabled(*this); }
    };

    struct OutlineColorUnchecked final : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_unchecked(*this); }
    };
    struct OutlineColorChecked final : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_checked(*this); }
    };
    struct OutlineColorUncheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorUncheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_unchecked_disabled(*this); }
    };
    struct OutlineColorCheckedDisabled final : public QColor, Token {
        using QColor::QColor;
        explicit OutlineColorCheckedDisabled(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_outline_color_checked_disabled(*this); }
    };

    struct HoverColorUnchecked final : public QColor, Token {
        using QColor::QColor;
        explicit HoverColorUnchecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_hover_color_unchecked(*this); }
    };
    struct HoverColorChecked final : public QColor, Token {
        using QColor::QColor;
        explicit HoverColorChecked(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const { self.set_hover_color_checked(*this); }
    };

    template <class Switch>
    concept property_concept = std::derived_from<Switch, Token> //
        || widget::pro::property_concept<Switch>                //
        || util::theme::pro::property_concept<Switch>;

    using Disabled = common::pro::Disabled<Token>;
    using Checked  = common::pro::Checked<Token>;

    template <typename Callback> using Clickable = common::pro::Clickable<Callback, Token, Switch>;

    using namespace util::theme::pro;
    using namespace widget::pro;
}
/// @note 使用时建议比例 w : h > 7 : 4 ，过冲动画会多占用一些宽度，倘若 w 过短，可能会出现 hover
/// 层画面被截断的情况
class Switch : public _switch::internal::Switch {
    CREEPER_DEFINE_CONSTROCTOR(Switch, _switch::pro)
    using _switch::internal::Switch::Switch;
};
}
