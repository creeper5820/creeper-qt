#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace scp = scroll::pro;

static ScrollArea TestScrollArea {
    test::kWidgetProps,
    test::kThemeManager,
    scp::VerticalScrollBarPolicy { Qt::ScrollBarAsNeeded },
    scp::HorizontalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
    scp::ScrollBarPolicy { Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff },
    scp::ScrollItem<Widget> { new Widget { } },
};
