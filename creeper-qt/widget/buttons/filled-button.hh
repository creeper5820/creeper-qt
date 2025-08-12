#pragma once

#include "button.hh"

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"

namespace creeper {
namespace filled_button::internal {

    class FilledButton : public QAbstractButton {
        CREEPER_PIMPL_DEFINITION(FilledButton);

    public:
        void set_color_scheme(const ColorScheme& pack);
        void load_theme_manager(ThemeManager& manager);

        void set_radius(double radius);
        void set_border_width(double border);

        void set_water_color(const QColor& color);
        void set_border_color(const QColor& color);
        void set_text_color(const QColor& color);
        void set_background(const QColor& color);
        void set_hover_color(const QColor& color);

        void set_water_ripple_status(bool enable);
        void set_water_ripple_step(double step);

    protected:
        void mouseReleaseEvent(QMouseEvent* event) override;

        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void paintEvent(QPaintEvent* event) override;
    };

}
namespace filled_button::pro {
    template <typename T>
    concept property_concept = util::theme::pro::concept_<T> || button::pro::property_concept<T>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = property_concept<T>;
        };
    };

    using namespace util::theme::pro;
    using namespace button::pro;
}

using FilledButton =
    Declarative<filled_button::internal::FilledButton, filled_button::pro::checker>;

}
