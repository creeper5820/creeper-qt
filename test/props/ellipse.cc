#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;

static Ellipse TestEllipse {
    test::kWidgetProps,
    api::pro::Background { Qt::red },
    api::pro::BorderColor { Qt::black },
    api::pro::BorderWidth { 1.0 },
};
