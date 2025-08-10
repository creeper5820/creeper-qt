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

    auto paint_event(const QPaintEvent& event) noexcept {
        auto painter = QPainter { &self };

        if (request_scale) regenerate_pixmap();

        if (!pixmap_render.isNull()) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::NoPen);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPixmap(event.rect(), pixmap_render);
        }
    }

private:
    auto regenerate_pixmap() noexcept -> void {
        const auto& _1 = pixmap_origin;
        const auto& _2 = self.size();
        pixmap_render  = content_scale.transform(_1, _2);
    }

public:
    ContentScale content_scale;
    bool request_scale = true;

    QPixmap pixmap_origin;
    QPixmap pixmap_render;

    Image& self;
};
