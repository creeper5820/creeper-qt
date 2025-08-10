#pragma once
#include "creeper-qt/utility/content-scale.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qwidget.h>

namespace creeper {
namespace image::internal {
    class Image : public QWidget {
        CREEPER_PIMPL_DEFINITION(Image)

    public:
        auto set_content_scale(ContentScale scale) noexcept -> void;

        auto content_scale() const noexcept -> ContentScale;
    };
}
namespace image::pro {

    using Token = common::Token<internal::Image>;

    struct ContentScale {
        using T = creeper::ContentScale;
        T content_scale;

        explicit ContentScale(T content_scale) noexcept
            : content_scale { content_scale } { }

        auto apply(auto& self) const noexcept -> void { }
    };

}
}
