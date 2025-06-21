#pragma once

#include <qabstractbutton.h>
#include <qpainter.h>

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include "creeper-qt/widget/widget.hh"

namespace creeper {
namespace icon_button::internal {

    constexpr auto kHoverOpacity = double { 0.1 };
    constexpr auto kWaterOpacity = double { 0.4 };
    constexpr auto kWidthRatio   = double { 1.25 };
    constexpr auto kOutlineWidth = double { 1 };

    class IconButton : public QAbstractButton {
        CREEPER_PIMPL_DEFINTION(IconButton);

    public:
        enum class Types { DEFAULT, TOGGLE_UNSELECTED, TOGGLE_SELECTED };
        enum class Shape { DEFAULT_ROUND, SQUARE };
        enum class Color { DEFAULT_FILLED, TONAL, OUTLINED, STANDARD };
        enum class Width { DEFAULT, NARROW, WIDE };

        static constexpr auto kSizeExtraSmall     = QSize { 32, 32 };
        static constexpr auto kIconSizeExtraSmall = int { 15 };
        static constexpr auto kSizeSmall          = QSize { 40, 40 };
        static constexpr auto kIconSizeSmall      = int { 24 };
        static constexpr auto kSizeMedium         = QSize { 56, 56 };
        static constexpr auto kIconSizeMedium     = int { 24 };
        static constexpr auto kSizeLarge          = QSize { 96, 96 };
        static constexpr auto kIconSizeLarge      = int { 32 };
        static constexpr auto kSizeExtraLarge     = QSize { 136, 136 };
        static constexpr auto kIconSizeExtraLarge = int { 40 };

    public:
        void set_color_scheme(const ColorScheme&);
        void load_theme_manager(ThemeManager&);

        void set_icon(const QString&);
        void set_icon(const QIcon&);

        void set_types(Types);
        void set_shape(Shape);
        void set_color(Color);
        void set_width(Width);

        // TODO: 详细的颜色自定义接口有缘再写

    protected:
        void enterEvent(QEvent*) override;
        void leaveEvent(QEvent*) override;

        void mouseReleaseEvent(QMouseEvent*) override;

        void paintEvent(QPaintEvent*) override;
    };
}
namespace icon_button::pro {
    using Token = common::Token<internal::IconButton>;

    struct Icon : public QIcon, Token {
        using QIcon::QIcon;
        explicit Icon(const QIcon& icon)
            : QIcon { icon } { }
        void apply(auto& self) const { self.set_icon(*this); }
    };
    struct FontIcon : public QString, Token {
        using QString::QString;
        explicit FontIcon(const QString& icon)
            : QString { icon } { }
        void apply(auto& self) const { self.set_icon(*this); }
    };

    struct Color : Token {
        using Enum = internal::IconButton::Color;
        Enum color;
        explicit Color(Enum p) { color = p; }
        void apply(auto& self) const { self.set_color(color); }
    };

    template <typename Callback, class Widget>
        requires std::invocable<Callback, Widget&>
    struct Clickable final : Token {
        Callback callback;
        explicit Clickable(Callback p) noexcept
            : callback(p) { }
        void apply(auto& self) const noexcept
            requires requires { &std::remove_cvref_t<decltype(self)>::clicked; }
        {
            QObject::connect(&self, &QAbstractButton::clicked, //
                [function = callback, &self] { function(self); });
        }
    };

    template <class T>
    concept property_concept = std::derived_from<T, Token> //
        || util::theme::pro::property_concept<T>           //
        || widget::pro::property_concept<T>;

    using namespace util::theme::pro;
    using namespace widget::pro;
}
class IconButton : public icon_button::internal::IconButton {
    CREEPER_DEFINE_CONSTROCTOR(IconButton, icon_button::pro);
};
}
