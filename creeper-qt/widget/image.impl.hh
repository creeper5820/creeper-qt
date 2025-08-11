#include "image.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpicture.h>
#include <qwidget.h>

using namespace creeper::image::internal;

template <typename T>
concept drawable_c = requires(const T& t) {
    { true };
};

struct Image::Impl final {
public:
    explicit Impl(Image& self) noexcept
        : self { self } { }

    auto paint_event(const QPaintEvent&) noexcept {
        auto painter = QPainter { &self };

        if (request_regenerate) regenerate_pixmap();

        if (resource_render && !resource_render->isNull()) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::NoPen);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPixmap(self.rect(), *resource_render);
        }
    }
    auto resize_event(const QResizeEvent&) noexcept { request_regenerate = true; }

private:
    auto regenerate_pixmap() noexcept -> void {
        if (!resource_origin) return;
        const auto& _1   = *resource_origin;
        const auto& _2   = self.size();
        *resource_render = content_scale.transform(_1, _2);
        {
            qDebug() << "Regenerate pixmap";
            qDebug() << "Origin size:" << resource_origin->size();
            qDebug() << "Render size:" << resource_render->size();
        }
        request_regenerate = false;
    }

public:
    ContentScale content_scale;
    bool request_regenerate = true;

    std::unique_ptr<PainterResource> resource_origin {};
    std::unique_ptr<PainterResource> resource_render {
        std::make_unique<PainterResource>(QPixmap {}),
    };

    Image& self;
};
