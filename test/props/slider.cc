#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace slp = slider::pro;

static Slider TestSlider {
    test::kWidgetProps,
    test::kThemeManager,
    slp::Progress { 0.5 },
    slp::OnValueChange { [](double) { } },
    slp::OnValueChangeFinished { [](double) { } },
};
