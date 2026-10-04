#pragma once
#include "creeper-qt/utility/wrapper/dsl.hh"

#include <qpainter.h>

namespace creeper::qt {
using painter     = QPainter;
using point       = QPointF;
using size        = QSizeF;
using rect        = QRectF;
using color       = QColor;
using real        = qreal;
using align       = Qt::Alignment;
using string      = QString;
using font        = QFont;
using text_option = QTextOption;
using icon        = QIcon;
}

namespace creeper::painter {

template <class T>
concept common_trait = requires(T t) {
    { auto { t.origin } } -> std::same_as<qt::point>;
    { auto { t.size } } -> std::same_as<qt::size>;
};

template <class T>
concept container_trait = requires(T t) {
    { auto { t.align } } -> std::same_as<qt::align>;
} && common_trait<T>;

template <class T>
concept shape_trait = requires(T t) {
    { auto { t.color_container } } -> std::same_as<qt::color>;
    { auto { t.color_outline } } -> std::same_as<qt::color>;
    { auto { t.thickness_outline } } -> std::same_as<qt::real>;
};

template <class T>
concept drawable_trait = common_trait<T> && std::invocable<T, qt::painter&>;

struct CommonProps {
    qt::point origin = qt::point { 0, 0 };
    qt::size size    = qt::size { 0, 0 };
    auto rect() const { return qt::rect { origin, size }; }
};

struct ContainerProps {
    qt::size size    = qt::size { 0, 0 };
    qt::align align  = qt::align { };
    qt::point origin = qt::point { 0, 0 };
    auto rect() const { return qt::rect { origin, size }; }
};

struct ShapeProps {
    qt::color container_color = Qt::transparent;
    qt::color outline_color   = Qt::transparent;
    qt::real outline_width    = 0;
};

}
namespace creeper::painter::common::pro {

struct Size {
    qt::size value;
    explicit Size(const qt::size& v)
        : value { v } { }
    friend auto dsl_invoke(auto& self, const Size& prop) -> void { self.size = prop.value; }
};

struct Origin {
    qt::point value;
    explicit Origin(const qt::point& v)
        : value { v } { }
    friend auto dsl_invoke(auto& self, const Origin& prop) -> void { self.origin = prop.value; }
};

struct ContainerColor {
    qt::color value;
    explicit ContainerColor(const qt::color& v)
        : value { v } { }
    friend auto dsl_invoke(auto& self, const ContainerColor& prop) -> void {
        self.container_color = prop.value;
    }
};

struct OutlineColor {
    qt::color value;
    explicit OutlineColor(const qt::color& v)
        : value { v } { }
    friend auto dsl_invoke(auto& self, const OutlineColor& prop) -> void {
        self.outline_color = prop.value;
    }
};

struct OutlineWidth {
    qt::real value;
    explicit OutlineWidth(qt::real v)
        : value { v } { }
    friend auto dsl_invoke(auto& self, const OutlineWidth& prop) -> void {
        self.outline_width = prop.value;
    }
};

struct Outline {
    qt::color color;
    qt::real width;

    Outline(const qt::color& color, qt::real width)
        : color { color }
        , width { width } { }

    friend auto dsl_invoke(auto& self, const Outline& prop) -> void {
        self.outline_color = prop.color;
        self.outline_width = prop.width;
    }
};

using Fill = ContainerColor;

}
