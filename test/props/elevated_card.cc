#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace cp = elevated_card::pro;

static ElevatedCard TestElevatedCard {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    cp::Level { CardLevel::HIGHEST },
    cp::RadiusTopLeft { 4.0 },
    cp::RadiusTopRight { 4.0 },
    cp::RadiusBottomLeft { 4.0 },
    cp::RadiusBottomRight { 4.0 },
    cp::RadiusNxNy { 4.0 },
    cp::RadiusPxPy { 4.0 },
    cp::RadiusNxPy { 4.0 },
    cp::RadiusPxNy { 4.0 },
};
