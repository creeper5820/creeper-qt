#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace rrp = rounded_rect::pro;

static RoundedRect TestRoundedRect {
    test::kWidgetProps,
    test::kShapeProps,
    rrp::RadiusTopLeft { 4.0 },
    rrp::RadiusTopRight { 4.0 },
    rrp::RadiusBottomLeft { 4.0 },
    rrp::RadiusBottomRight { 4.0 },
    rrp::RadiusNxNy { 4.0 },
    rrp::RadiusPxPy { 4.0 },
    rrp::RadiusNxPy { 4.0 },
    rrp::RadiusPxNy { 4.0 },
};
