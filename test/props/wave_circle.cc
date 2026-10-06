#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace wp = wave_circle::pro;

static WaveCircle TestWaveCircle {
    test::kWidgetProps,
    wp::Background { Qt::red },
    wp::BorderColor { Qt::black },
    wp::BorderWidth { 1.0 },
    wp::FlangeNumber { 8 },
    wp::FlangeRadius { 10.0 },
    wp::OverallRadius { 20.0 },
    wp::ProtrudingRatio { 0.5 },
};
