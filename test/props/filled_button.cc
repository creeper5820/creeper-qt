#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace fbp = filled_button::pro;

static FilledButton TestFilledButton {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    fbp::HoverColor { Qt::red },
    fbp::WaterRippleStatus { true },
    fbp::WaterRippleStep { 0.5 },
    fbp::Text { "OK" },
    fbp::TextColor { Qt::white },
    fbp::WaterColor { Qt::blue },
    fbp::Clickable { [] { } },
};
