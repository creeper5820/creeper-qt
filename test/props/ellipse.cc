#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ep = ellipse::pro;

static Ellipse TestEllipse {
    test::kWidgetProps,
    ep::Background { Qt::red },
    ep::BorderColor { Qt::black },
    ep::BorderWidth { 1.0 },
};
