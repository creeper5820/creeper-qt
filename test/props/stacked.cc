#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace stp = stacked::pro;

static Stacked TestStacked {
    test::kLayoutProps,
    stp::CurrentIndex { 0 },
    stp::IndexChanged { [](int) { } },
    new Widget { },
    stp::AddWidget<Widget> { new Widget { } },
};
