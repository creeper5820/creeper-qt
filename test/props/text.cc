#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ttp = text::pro;

static Text TestText {
    test::kWidgetProps,
    test::kThemeManager,
    ttp::Color { Qt::white },
    ttp::WordWrap { true },
    ttp::AdjustSize { },
    ttp::Alignment { Qt::AlignCenter },
    ttp::TextInteractionFlags { Qt::TextSelectableByMouse },
    ttp::Text { "OK" },
};
