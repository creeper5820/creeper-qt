#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace wp = wave_circle::pro;

static WaveCircle TestWaveCircle {
    test::kWidgetProps,
    api::pro::Background { Qt::red },
    api::pro::BorderColor { Qt::black },
    api::pro::BorderWidth { 1.0 },
    wp::FlangeNumber { 8 },
    wp::FlangeRadius { 10.0 },
    wp::OverallRadius { 20.0 },
    wp::ProtrudingRatio { 0.5 },
};
