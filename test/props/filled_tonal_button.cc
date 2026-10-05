#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ftp = filled_tonal_button::pro;

static FilledTonalButton TestFilledTonalButton {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    ftp::HoverColor { Qt::red },
    ftp::WaterRippleStatus { true },
    ftp::WaterRippleStep { 0.5 },
    ftp::Text { "OK" },
    ftp::TextColor { Qt::white },
    ftp::WaterColor { Qt::blue },
    ftp::Clickable { [] { } },
};
