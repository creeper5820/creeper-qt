#pragma once
#include "creeper-qt/utility/wrapper/property.hh"

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
    qt::align align  = qt::align {};
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

struct Token { };

using Size   = DerivedProp<Token, qt::size, [](auto& self, auto const& v) { self.size = v; }>;
using Origin = DerivedProp<Token, qt::point, [](auto& self, auto const& v) { self.origin = v; }>;

using ContainerColor =
    SetterProp<Token, qt::color, [](auto& self, auto const& v) { self.container_color = v; }>;
using OutlineColor =
    SetterProp<Token, qt::color, [](auto& self, auto const& v) { self.outline_color = v; }>;
using OutlineWidth =
    SetterProp<Token, qt::real, [](auto& self, auto const& v) { self.outline_width = v; }>;

struct Outline : Token {
    qt::color color;
    qt::real width;
    Outline(const qt::color& color, qt::real width)
        : color { color }
        , width { width } { }
    auto apply(auto& self) {
        self.outline_color = color;
        self.outline_width = width;
    }
};

/// Alias

using Fill = ContainerColor;

/// Export

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
}
