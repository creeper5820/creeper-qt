#pragma once
#include <qdebug.h>
#include <qpixmap.h>

namespace creeper {

struct ContentScale {
public:
    enum : uint8_t {
        NONE,
        FIT,
        CROP,
        FILL_WIDTH,
        FILL_HEIGHT,
        FILL_BOUNDS,
        INSIDE,
    } data;

    operator decltype(data)() const noexcept { return data; }

    auto transform(const QPixmap& pixmap, const QSize& size) const -> QPixmap {
        if (pixmap.isNull()) return {};

        auto image_size    = QPointF(pixmap.width(), pixmap.height());
        auto target_width  = static_cast<double>(size.width());
        auto target_height = static_cast<double>(size.height());

        constexpr auto mode = Qt::SmoothTransformation;

        switch (data) {
        case ContentScale::NONE: {
            return pixmap;
        }
        case ContentScale::FIT: {
            image_size *= target_width / image_size.x();
            if (image_size.y() > target_height) image_size *= target_height / image_size.y();
            return pixmap.scaled(image_size.x(), image_size.y(), Qt::IgnoreAspectRatio, mode);
        }
        case ContentScale::CROP: {
            image_size *= target_width / image_size.x();
            if (image_size.y() < target_height) image_size *= target_height / image_size.y();
            return pixmap.scaled(image_size.x(), image_size.y(), Qt::IgnoreAspectRatio, mode);
        }
        case ContentScale::INSIDE: {
            if (image_size.x() > target_width) image_size *= target_width / image_size.x();
            if (image_size.y() > target_height) image_size *= target_height / image_size.y();
            return pixmap.scaled(image_size.x(), image_size.y(), Qt::IgnoreAspectRatio, mode);
        }
        case ContentScale::FILL_BOUNDS: {
            return pixmap.scaled(size, Qt::IgnoreAspectRatio, mode);
        }
        case ContentScale::FILL_WIDTH:
            return pixmap.scaled(target_width, image_size.y() * target_width / image_size.x(),
                Qt::IgnoreAspectRatio, mode);
        case ContentScale::FILL_HEIGHT:
            return pixmap.scaled(image_size.x() * target_height / image_size.y(), target_height,
                Qt::IgnoreAspectRatio, mode);
        }

        return {};
    }
};

}
