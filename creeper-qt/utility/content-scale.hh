#pragma once
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

        constexpr auto mode = Qt::TransformationMode::SmoothTransformation;

        switch (data) {
        case ContentScale::NONE:
            return pixmap;
        case ContentScale::FIT:
            return pixmap.scaled(size, Qt::KeepAspectRatio, mode);
        case ContentScale::CROP:
            return pixmap.scaled(size, Qt::KeepAspectRatioByExpanding, mode);
        case ContentScale::FILL_WIDTH:
            return pixmap.scaled(size.width(), pixmap.height() * size.width() / pixmap.width(),
                Qt::IgnoreAspectRatio, mode);
        case ContentScale::FILL_HEIGHT:
            return pixmap.scaled(size.height(), pixmap.width() * size.height() / pixmap.height(),
                Qt::IgnoreAspectRatio, mode);
        case ContentScale::FILL_BOUNDS:
            return pixmap.scaled(size, Qt::IgnoreAspectRatio, mode);
        case ContentScale::INSIDE:
            return pixmap.scaled(size, Qt::KeepAspectRatio, mode);
        }
    }
};

}
