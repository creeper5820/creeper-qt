#pragma once

#include "utility/theme/theme.hh"
#include "utility/wrapper/pimpl.hh"
#include "utility/wrapper/property.hh"
#include "widget/widget.hh"
#include <qabstractbutton.h>

namespace creeper {
namespace _switch::internal {
    class Switch : public QAbstractButton {
        CREEPER_PIMPL_DEFINTION(Switch)

    public:
        void set_color_scheme(const ColorScheme&);
        void load_theme_manager(ThemeManager&);

        void set_disabled(bool);
        void set_checked(bool);
        bool checked() const;

        void set_track_color_unchecked(QColor);
        void set_track_color_checked(QColor);
        void set_track_color_disabled(QColor);

        void set_handle_color_unchecked(QColor);
        void set_handle_color_checked(QColor);
        void set_handle_color_disabled(QColor);

        void set_outline_color_unchecked(QColor);
        void set_outline_color_checked(QColor);
        void set_outline_color_disabled(QColor);

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

    template <class Switch>
    concept property_concept = std::derived_from<Switch, Token> //
        || widget::pro::property_concept<Switch>                //
        || util::theme::pro::property_concept<Switch>;

    using namespace util::theme::pro;
    using namespace widget::pro;
}

class Switch : public _switch::internal::Switch {
    CREEPER_DEFINE_CONSTROCTOR(Switch, _switch::pro)
    using _switch::internal::Switch::Switch;
};

}
