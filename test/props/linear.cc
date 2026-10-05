#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace lp = linear::pro;

static Row TestRow {
    test::kLayoutProps,
    lp::SpacingItem { 10 },
    lp::Stretch { 1 },
    lp::SpacerItem { new QSpacerItem { 1, 1 } },
    lp::AddWidget<Widget> { new Widget { } },
    new Widget { } + Row::Placement { 0, Qt::AlignLeft },
};

static Col TestCol {
    test::kLayoutProps,
    new Widget { },
    new Widget { } + Col::Placement { 0, Qt::AlignTop },
};
