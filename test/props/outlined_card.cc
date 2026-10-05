#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace cp = outlined_card::pro;

static OutlinedCard TestOutlinedCard {
    test::kWidgetProps,
    test::kShapeProps,
    test::kThemeManager,
    cp::Level { CardLevel::HIGHEST },
    cp::RadiusTopLeft { 4.0 },
    cp::RadiusTopRight { 4.0 },
    cp::RadiusBottomLeft { 4.0 },
    cp::RadiusBottomRight { 4.0 },
    api::pro::RadiusNxNy { 4.0 },
    api::pro::RadiusPxPy { 4.0 },
    api::pro::RadiusNxPy { 4.0 },
    api::pro::RadiusPxNy { 4.0 },
};
