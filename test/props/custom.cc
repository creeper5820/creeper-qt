#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace cusp = custom::pro;

static CustomWidget TestCustomWidget {
    test::kWidgetProps,
    cusp::OnPaint { [](CustomWidget&) { } },
};
