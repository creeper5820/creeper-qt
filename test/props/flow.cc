#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace fp = flow::pro;

static Flow TestFlow {
    test::kLayoutProps,
    fp::RowSpacing { 4 },
    fp::ColSpacing { 4 },
    fp::RowLimit { 4 },
    fp::AddWidget<Widget> { new Widget { } },
};
