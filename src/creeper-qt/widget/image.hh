#pragma once

#include "creeper-qt/utility/api/pro/opacity.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/shape.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/content-scale.hh"
#include "creeper-qt/utility/painter-resource.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

namespace creeper {

class Image : public QWidget, public DSL {
    CREEPER_PIMPL_DEFINITION(Image)

public:
    explicit Image(auto&&... props)
        : Image { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto updatePixmap() noexcept -> void;

    auto setContentScale(ContentScale) noexcept -> void;
    auto contentScale() const noexcept -> ContentScale;

    auto setPainterResource(std::shared_ptr<PainterResource>) noexcept -> void;
    auto painterResource() const noexcept -> PainterResource;

    auto setOpacity(double) noexcept -> void;
    auto setRadius(double) noexcept -> void;
    auto setBorderWidth(double) noexcept -> void;
    auto setBorderColor(QColor) noexcept -> void;

protected:
    auto paintEvent(QPaintEvent*) -> void override;
    auto resizeEvent(QResizeEvent*) -> void override;
};

namespace image::pro {
    using ContentScale = ForwardProp<&Image::setContentScale>;
    struct PainterResource {
        using T = creeper::PainterResource;
        mutable std::shared_ptr<T> resource;

        explicit PainterResource(std::shared_ptr<T> resource) noexcept
            : resource { std::move(resource) } { }

        explicit PainterResource(auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : resource { std::make_shared<T>(std::forward<decltype(args)>(args)...) } { }

        friend auto dsl_invoke(Image& self, const PainterResource& prop) -> void {
            self.setPainterResource(std::move(prop.resource));
        }
    };
    using Pixmap = PainterResource;

    using api::pro::Opacity;

    using namespace api::scope::common;
    using namespace api::scope::shape;
    using namespace api::scope::widget;
}

}
