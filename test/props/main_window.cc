#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace mwp = main_window::pro;

static MainWindow TestMainWindow {
    test::kWidgetProps,
    mwp::Central<Widget> { new Widget { } },
};
