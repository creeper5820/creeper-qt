#include "creeper-qt/utility/painter/helper.hh"
#include "image.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpicture.h>
#include <qwidget.h>

using namespace creeper::image::internal;

struct Image::Impl final {
public:
    explicit Impl(Image& self) noexcept
        : self { self } { }

    auto paint_event(const QPaintEvent&) noexcept {
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
            auto path = QPainterPath {};
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
            util::PainterHelper { painter }.rounded_rectangle(
                Qt::transparent, border_color, border_width, self.rect(), radius, radius);
        }
    }
    auto resize_event(const QResizeEvent&) noexcept { request_regenerate = true; }

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

    std::unique_ptr<PainterResource> resource_origin {};
    std::unique_ptr<PainterResource> resource_render {
        std::make_unique<PainterResource>(QPixmap {}),
    };

    Image& self;
};
