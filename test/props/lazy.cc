#include <creeper-qt/creeper-qt.hh>

#include <vector>

#include "bundles.hh"

using namespace creeper;

static LazyColumn TestLazyColumn {
    test::kWidgetProps,
    lazy::pro::LazyWidget<Widget> { },
    lazy::pro::LazyWidgets<Widget> { std::vector<Widget*> { } },
};

static LazyRow TestLazyRow {
    test::kWidgetProps,
    lazy::pro::LazyWidget<Widget> { },
    lazy::pro::LazyWidgets<Widget> { std::vector<Widget*> { } },
};
