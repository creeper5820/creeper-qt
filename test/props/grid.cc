#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace gp = grid::pro;

static Grid TestGrid {
    test::kLayoutProps,
    gp::RowSpacing { 4 },
    gp::ColSpacing { 4 },
    gp::GridItem<Widget> { Grid::Placement { 0, 0 }, new Widget { } },
    new Widget { } + Grid::Placement { 0, 1 },
};
