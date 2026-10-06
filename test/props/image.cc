#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ip = image::pro;

static Image TestImage {
    test::kWidgetProps,
    ip::BorderColor { Qt::black },
    ip::BorderWidth { 1.0 },
    ip::Radius { 4.0 },
    ip::ContentScale { ContentScale::CROP },
    ip::Opacity { 1.0 },
    ip::PainterResource { std::string_view { } },
};
