#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace ip = image::pro;

static Image TestImage {
    test::kWidgetProps,
    api::pro::BorderColor { Qt::black },
    api::pro::BorderWidth { 1.0 },
    api::pro::Radius { 4.0 },
    ip::ContentScale { ContentScale::CROP },
    ip::Opacity { 1.0 },
    ip::PainterResource { std::string_view { } },
};
