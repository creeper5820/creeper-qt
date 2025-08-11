#pragma once
#include "creeper-qt/utility/content-scale.hh"
#include "creeper-qt/utility/painter-resource.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

#include <qwidget.h>

namespace creeper {
namespace image::internal {
    class Image : public QWidget {
        CREEPER_PIMPL_DEFINITION(Image)

    public:
        auto set_content_scale(ContentScale) noexcept -> void;
        auto set_painter_resource(std::unique_ptr<PainterResource>) noexcept -> void;

        auto content_scale() const noexcept -> ContentScale;
        auto painter_resource() const noexcept -> PainterResource;

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
        auto apply(auto& self) const noexcept -> void
            requires requires { self.set_painter_resource(std::move(resource)); }
        {
            self.set_painter_resource(std::move(resource));
        }
    };

    template <class T>
    concept property_concept = std::derived_from<T, Token> || widget::pro::property_concept<T>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = property_concept<T>;
        };
    };
}
using Image = Declarative<image::internal::Image, image::pro::checker>;
}
