#pragma once

#include "creeper-qt/utility/api/scope/theme.hh" // IWYU pragma: keep

#include "creeper-qt/utility/api/pro/text.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep

#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include <qlabel.h>

namespace creeper {

class Text : public QLabel, public DSL {
public:
    using QLabel::QLabel;

    explicit Text(auto&&... props) { construct_with(std::forward<decltype(props)>(props)...); }

    auto loadColorScheme(const ColorScheme& scheme) noexcept -> void {
        setColor(scheme.on_surface);
    }
    auto bindThemeManager(ThemeManager& manager) noexcept -> void {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }

    auto setColor(QColor color) noexcept -> void {
        const auto name  = color.name(QColor::HexArgb);
        const auto style = QString("QLabel { color : %1; }");
        setStyleSheet(style.arg(name));
    }
};

namespace text::pro {
    using Color                = ForwardProp<&creeper::Text::setColor>;
    using WordWrap             = ForwardProp<&QLabel::setWordWrap>;
    using AdjustSize           = ForwardProp<&QWidget::adjustSize>;
    using Alignment            = ForwardProp<&QLabel::setAlignment>;
    using TextInteractionFlags = ForwardProp<&QLabel::setTextInteractionFlags>;

    using api::pro::Text;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
