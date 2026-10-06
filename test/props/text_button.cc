#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace tbp = text_button::pro;

static TextButton TestTextButton {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    tbp::HoverColor { Qt::red },
    tbp::WaterRippleStatus { true },
    tbp::Text { "OK" },
    tbp::TextColor { Qt::white },
    tbp::WaterColor { Qt::blue },
    tbp::Clickable { [] { } },
};
