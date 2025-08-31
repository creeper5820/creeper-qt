#pragma once
#include "creeper-qt/utility/content-scale.hh"
#include "creeper-qt/utility/painter-resource.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {
namespace image::internal {
    class Image : public QWidget {
        CREEPER_PIMPL_DEFINITION(Image)

    public:
        auto update_pixmap() noexcept -> void;

        auto set_content_scale(ContentScale) noexcept -> void;
        auto content_scale() const noexcept -> ContentScale;

        auto set_painter_resource(std::unique_ptr<PainterResource>) noexcept -> void;
        auto painter_resource() const noexcept -> PainterResource;

        auto set_opacity(double) noexcept -> void;
        auto set_radius(double) noexcept -> void;
        auto set_border_width(double) noexcept -> void;
        auto set_border_color(QColor) noexcept -> void;

    protected:
        auto paintEvent(QPaintEvent*) -> void override;
        auto resizeEvent(QResizeEvent*) -> void override;
    };
}
namespace image::pro {

    using Token = common::Token<internal::Image>;

    struct ContentScale : Token {
        using T = creeper::ContentScale;
        T content_scale;
        explicit ContentScale(T content_scale) noexcept
            : content_scale { content_scale } { }
        explicit ContentScale(const auto& e) noexcept
            requires std::constructible_from<T, decltype(e)>
            : content_scale { e } { }
        auto apply(auto& self) const noexcept -> void
            requires requires { self.set_content_scale(content_scale); }
        {
            self.set_content_scale(content_scale);
        }
    };
    struct PainterResource : Token {
        using T = creeper::PainterResource;
        mutable std::unique_ptr<T> resource;

        explicit PainterResource(std::unique_ptr<T> resource) noexcept
            : resource { std::move(resource) } { }

        explicit PainterResource(auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : resource { std::make_unique<T>(std::forward<decltype(args)>(args)...) } { }

        auto apply(auto& self) const noexcept -> void
            requires requires { self.set_painter_resource(std::move(resource)); }
        {
            self.set_painter_resource(std::move(resource));
        }
    };
    using Pixmap = PainterResource;

    using Opacity     = common::pro::Opacity<Token>;
    using Radius      = common::pro::Radius<Token>;
    using BorderColor = common::pro::BorderColor<Token>;
    using BorderWidth = common::pro::BorderWidth<Token>;

    template <class T>
    concept trait = std::derived_from<T, Token>;

    CREEPER_DEFINE_CHECKER(trait);
    using namespace widget::pro;
}
using Image =
    Declarative<image::internal::Image, CheckerOr<image::pro::checker, widget::pro::checker>>;
}
