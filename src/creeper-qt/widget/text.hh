#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/widget.hh"
#include <qlabel.h>

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

using Text = common::pro::Text<Token>;

using Color = SetterProp<Token, QColor, [](auto& self, const auto& v) { self.set_color(v); }>;

using WordWrap = SetterProp<Token, bool, [](auto& self, const auto& v) { self.setWordWrap(v); }>;

using AdjustSize = ActionProp<Token, [](auto& self) { self.adjustSize(); }>;

using Alignment =
    SetterProp<Token, Qt::Alignment, [](auto& self, const auto& v) { self.setAlignment(v); }>;

using TextInteractionFlags = SetterProp<Token, Qt::TextInteractionFlags,
    [](auto& self, const auto& v) { self.setTextInteractionFlags(v); }>;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace widget::pro;
using namespace theme::pro;
}
namespace creeper {

using Text = Declarative<text::internal::Text,
    CheckerOr<text::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
