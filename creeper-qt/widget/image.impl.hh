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
        const auto& width  = self.width();
        const auto& height = self.height();

        if (request_regenerate) regenerate_pixmap();

        auto painter = QPainter { &self };
        if (resource_render && !resource_render->isNull()) {
            const auto& pixmap = *resource_render;
            const auto& point  = QPointF {
                (width - pixmap.width()) / 2.,
                (height - pixmap.height()) / 2.,
            };
            auto path = QPainterPath {};
            path.addRoundedRect(border, border, width - 2 * border, height - 2 * border,
                radius - border / 2, radius - border / 2);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::NoPen);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setClipPath(path);
            painter.drawPixmap(point, pixmap);
        }
        { }
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

public:
    ContentScale content_scale;
    bool request_regenerate = true;

    double border = 02.;
    double radius = 10.;
    double alpha  = 01.;

    QColor color_background = Qt::transparent;
    QColor color_border     = Qt::transparent;

    std::unique_ptr<PainterResource> resource_origin {};
    std::unique_ptr<PainterResource> resource_render {
        std::make_unique<PainterResource>(QPixmap {}),
    };

    Image& self;
};
