#pragma once

#include "creeper-qt/utility/api/scope/common.hh"

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/widget.hh"
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

    using namespace common::pro;
    using namespace api::scope::common;
    using namespace widget::pro;
    using namespace theme::pro;

    using Color = ForwardProp<&creeper::Text::setColor>;

    using WordWrap = ForwardProp<&QLabel::setWordWrap>;

    using AdjustSize = ForwardProp<&QWidget::adjustSize>;

    using Alignment = ForwardProp<&QLabel::setAlignment>;

    using TextInteractionFlags = ForwardProp<&QLabel::setTextInteractionFlags>;

}

}
