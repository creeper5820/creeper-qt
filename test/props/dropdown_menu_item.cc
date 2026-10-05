#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace dmip = dropdown_menu_item::pro;

static DropdownMenuItem TestDropdownMenuItem {
    test::kWidgetProps,
    test::kThemeManager,
    dmip::TrailingText { "trailing" },
    dmip::LeadingIcon { QString { }, QString { } },
    dmip::TrailingIcon { QString { }, QString { } },
    dmip::OnClicked { [] { } },
    dmip::Disabled { false },
    dmip::Text { "OK" },
    dmip::WaterColor { Qt::blue },
};
