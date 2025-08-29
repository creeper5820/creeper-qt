#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/widget/widget.hh"
#include <qlabel.h>
#include <qnamespace.h>

namespace creeper::text::internal {

class Text : public QLabel {
    using QLabel::QLabel;

public:
    auto set_color_scheme(const ColorScheme& scheme) noexcept -> void {
        set_color(scheme.on_surface);
    }
    auto load_theme_manager(ThemeManager& manager) noexcept -> void {
        manager.append_handler(this,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }

    auto set_color(QColor color) noexcept -> void {
        const auto name  = color.name(QColor::HexArgb);
        const auto style = QString("QLabel { color : %1; }");
        setStyleSheet(style.arg(name));
    }
};

}

namespace creeper::text::pro {
using Token = common::Token<internal::Text>;

struct Text : Token, QString {
    using QString::QString;
    using QString::operator=;

    explicit Text(std::convertible_to<QString> auto&& o) noexcept
        : QString { o } { }

    explicit Text(const std::string& text) noexcept
        : QString { QString::fromStdString(text) } { }

    auto apply(auto& self) const noexcept -> void
        requires requires { self.setText(QString {}); }
    {
        self.setText(*this);
    }
};
struct Color : Token, QColor {
    using QColor::QColor;
    using QColor::operator=;
    using QColor::operator==;
    auto apply(auto& self) const noexcept -> void
        requires requires { self.set_color(QColor {}); }
    {
        self.set_color(*this);
    }
};
struct WordWrap : Token {
    bool on;
    constexpr explicit WordWrap(bool on)
        : on { on } { }
    auto apply(auto& self) const noexcept -> void
        requires requires { self.setWordWrap(bool {}); }
    {
        self.setWordWrap(on);
    }
};
struct AdjustSize : Token {
    auto apply(auto& self) const noexcept -> void
        requires requires { self.adjustSize(); }
    {
        self.adjustSize();
    }
};
struct Alignment : Token {
    Qt::AlignmentFlag alignment;
    constexpr explicit Alignment(Qt::AlignmentFlag alignment) noexcept
        : alignment { alignment } { }
    auto apply(auto& self) const noexcept -> void
        requires requires { self.setAlignment(Qt::AlignCenter); }
    {
        self.setAlignment(alignment);
    }
};

template <class T>
concept trait = std::derived_from<T, Token> || widget::pro::trait<T> || theme::pro::trait<T>;

CREEPER_DEFINE_CHECK(trait);
using namespace widget::pro;
}

namespace creeper {
using Text = Declarative<text::internal::Text, text::pro::checker>;
}
