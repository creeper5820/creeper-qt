#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ibp = icon_button::pro;

static IconButton TestIconButton {
    test::kWidgetProps,
    test::kThemeManager,
    ibp::Icon { QIcon { } },
    ibp::FontIcon { QString { } },
    ibp::Color { IconButton::Color::TONAL },
    ibp::Shape { IconButton::Shape::SQUARE },
    ibp::Types { IconButton::Types::DEFAULT },
    ibp::Width { IconButton::Width::WIDE },
    ibp::Clickable { [] { } },
};
