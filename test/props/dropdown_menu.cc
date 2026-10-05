#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace dmp = dropdown_menu::pro;

static DropdownMenu TestDropdownMenu {
    test::kWidgetProps,
    test::kThemeManager,
    dmp::Expanded { true },
    dmp::Anchor { new Widget { } },
    dmp::Offset { QPoint { 0, 0 } },
    dmp::ContainerColor { Qt::red },
    dmp::CornerRadius { 4.0 },
    dmp::OnDismissRequest { [] { } },
    new DropdownMenuItem { },
    dmp::MenuWidget<DropdownMenuItem> { new DropdownMenuItem { } },
};
