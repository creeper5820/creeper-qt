#include "image.hh"

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/shape/rect.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpicture.h>
#include <qwidget.h>

using namespace creeper;

struct Image::Impl final {
public:
    explicit Impl(Image& self) noexcept
        : self { self } { }

    auto paint_event(const QPaintEvent&) noexcept {
        using namespace drawable;

        const auto radius = get_radius();
        const auto width  = self.width();
        const auto height = self.height();

        if (request_regenerate) regenerate_pixmap();

        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setOpacity(opacity);

        if (resource_render && !resource_render->isNull()) {
            const auto& pixmap = *resource_render;
            const auto& point  = QPointF {
                (width - pixmap.width()) / 2.,
                (height - pixmap.height()) / 2.,
            };
            auto path = QPainterPath { };
            path.addRoundedRect(border_width, border_width, width - 2 * border_width,
                height - 2 * border_width, radius - border_width, radius - border_width);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::NoPen);
            painter.setClipPath(path);
            painter.setClipping(true);
            painter.drawPixmap(point, pixmap);
        }
        {
            painter.setClipping(false);
            DrawHere {
                painter,
                Rect {
                  .size = self.size(),

                  .outline_color = border_color,
                  .outline_width = border_width,

                  .radius_tl = radius,
                  .radius_tr = radius,
                  .radius_br = radius,
                  .radius_bl = radius,
                },
            };
        }
    }
    auto resize_event(const QResizeEvent&) noexcept {
        request_regenerate = true;
    }

private:
    auto regenerate_pixmap() noexcept -> void {
        if (!resource_origin) return;
        if (resource_origin->isNull()) return;
        const auto& _1     = *resource_origin;
        const auto& _2     = self.size();
        *resource_render   = content_scale.transform(_1, _2);
        request_regenerate = false;
    }

    auto get_radius() const noexcept -> double {
        return radius < 0 ? std::min<double>(self.width(), self.height()) / 2. : radius;
    }

public:
    ContentScale content_scale;
    bool request_regenerate = true;

    double border_width = 02.;
    QColor border_color = Qt::transparent;

    double radius  = 10.;
    double opacity = 01.;

    std::shared_ptr<PainterResource> resource_origin { };
    std::shared_ptr<PainterResource> resource_render {
        std::make_shared<PainterResource>(QPixmap { }),
    };

    Image& self;
};

using namespace creeper;

Image::Image()
    : pimpl { std::make_unique<Impl>(*this) } { }

Image::~Image() = default;

auto Image::updatePixmap() noexcept -> void {
    pimpl->request_regenerate = true;
    this->update();
}

auto Image::setContentScale(ContentScale scale) noexcept -> void {
    pimpl->content_scale      = scale;
    pimpl->request_regenerate = true;
}
auto Image::contentScale() const noexcept -> ContentScale {
    return pimpl->content_scale;
}

auto Image::setPainterResource(std::shared_ptr<PainterResource> resource) noexcept -> void {
    pimpl->resource_origin = std::move(resource);
    pimpl->resource_origin->add_finished_callback([this](auto&) { update(); });
    pimpl->request_regenerate = true;
}
auto Image::painterResource() const noexcept -> PainterResource {
    return *pimpl->resource_origin;
}

auto Image::setOpacity(double opacity) noexcept -> void {
    pimpl->opacity = opacity;
    update();
}
auto Image::setRadius(double radius) noexcept -> void {
    pimpl->radius = radius;
    update();
}
auto Image::setBorderWidth(double width) noexcept -> void {
    pimpl->border_width = width;
    update();
}
auto Image::setBorderColor(QColor color) noexcept -> void {
    pimpl->border_color = color;
    update();
}

auto Image::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_event(*event);
}
auto Image::resizeEvent(QResizeEvent* event) -> void {
    pimpl->resize_event(*event);
}
