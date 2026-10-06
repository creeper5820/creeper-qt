#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace obp = outlined_button::pro;

static OutlinedButton TestOutlinedButton {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    obp::HoverColor { Qt::red },
    obp::WaterRippleStatus { true },
    obp::Text { "OK" },
    obp::TextColor { Qt::white },
    obp::WaterColor { Qt::blue },
    obp::Clickable { [] { } },
};
