#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace swp = switch_::pro;

static Switch TestSwitch {
    test::kWidgetProps,
    test::kThemeManager,
    swp::Measurements { },
    swp::Colors { },
    swp::Checked { true },
    swp::Disabled { false },
    swp::Clickable { [] { } },
};
