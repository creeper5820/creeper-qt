#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace cpip = circular_progress_indicator::pro;

static CircularProgressIndicator TestCircularProgressIndicator {
    test::kWidgetProps,
    test::kThemeManager,
    cpip::Progress { 0.5 },
    cpip::Indeterminate { false },
    cpip::IndicatorColor { Qt::red },
    cpip::TrackColor { Qt::gray },
    cpip::StrokeWidth { 4.0 },
};
