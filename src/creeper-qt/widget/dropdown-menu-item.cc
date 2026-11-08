#include "dropdown-menu-item.hh"

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/layout/surface.hh"
#include "creeper-qt/details/drawable/logic/if.hh"
#include "creeper-qt/details/drawable/shape/rect.hh"
#include "creeper-qt/details/drawable/shape/text.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"

#include <qevent.h>
#include <qpainter.h>

#include <cmath>

using namespace creeper;

struct DropdownMenuItem::Impl {
    enum class Trailing { NONE, ICON, TEXT };

    Measurements measurements;
    ColorSpace color_space;

    QString text;

    QString leading_icon_code;
    QFont leading_icon_font;

    Trailing trailing_kind = Trailing::NONE;
    QString trailing_icon_code;
    QFont trailing_icon_font;
    QString trailing_text;

    bool is_disabled = false;
    bool is_hovered  = false;
    bool is_pressed  = false;

    QFont label_font;

    Animatable animatable;
    WaterRipple water_ripple;
    QColor water_color;
    bool enable_water_ripple = true;

    DropdownMenuItem& self;

    explicit Impl(DropdownMenuItem& self) noexcept
        : animatable { self }
        , water_ripple { animatable }
        , self { self } {
        self.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        set_measurements(Measurements { });

        auto clip = QPainterPath { };
        clip.addRect(self.rect());

        water_ripple.setSize(self.size());
        water_ripple.setClipPath(clip);
        water_ripple.setColor(water_color);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void {
        color_space.enabled.label_text    = scheme.on_surface;
        color_space.enabled.leading_icon  = scheme.on_surface_variant;
        color_space.enabled.trailing_icon = scheme.on_surface_variant;

        color_space.disabled.label_text = scheme.on_surface;
        color_space.disabled.label_text.setAlphaF(0.38);
        color_space.disabled.leading_icon  = color_space.disabled.label_text;
        color_space.disabled.trailing_icon = color_space.disabled.label_text;

        color_space.hover_state_layer = scheme.on_surface;
        color_space.hover_state_layer.setAlphaF(0.08);
        color_space.pressed_state_layer = scheme.on_surface;
        color_space.pressed_state_layer.setAlphaF(0.12);

        water_color = scheme.on_surface;

        water_ripple.setColor(water_color);

        self.update();
    }

    auto bindThemeManager(ThemeManager& manager) -> void {
        manager.appendHandler(self);
    }

    auto set_measurements(const Measurements& value) noexcept -> void {
        measurements = value;

        label_font = self.font();
        label_font.setPixelSize(measurements.label_font_size);
        label_font.setWeight(QFont::Medium);

        leading_icon_font.setPixelSize(measurements.icon_size);
        trailing_icon_font.setPixelSize(measurements.icon_size);

        self.setFixedHeight(measurements.height);
        self.updateGeometry();
        self.update();
    }

    auto content_width() const -> int {
        const auto fm = QFontMetricsF { label_font };

        auto width = measurements.horizontal_padding * 1.;
        if (!leading_icon_code.isEmpty())
            width += measurements.icon_size + measurements.icon_text_spacing;
        width += fm.horizontalAdvance(text);
        if (trailing_kind == Trailing::ICON)
            width += measurements.icon_text_spacing + measurements.icon_size;
        if (trailing_kind == Trailing::TEXT)
            width += measurements.icon_text_spacing + fm.horizontalAdvance(trailing_text);
        width += measurements.horizontal_padding;

        return static_cast<int>(std::ceil(width));
    }

    auto set_text(const QString& value) -> void {
        text = value;
        self.updateGeometry();
        self.update();
    }

    auto set_leading_icon(const QString& code, const QString& font) -> void {
        leading_icon_code = code;
        if (!font.isEmpty()) leading_icon_font = QFont { font };
        leading_icon_font.setPixelSize(measurements.icon_size);
        self.updateGeometry();
        self.update();
    }

    auto set_trailing_icon(const QString& code, const QString& font) -> void {
        trailing_kind      = Trailing::ICON;
        trailing_icon_code = code;
        if (!font.isEmpty()) trailing_icon_font = QFont { font };
        trailing_icon_font.setPixelSize(measurements.icon_size);
        self.updateGeometry();
        self.update();
    }

    auto set_trailing_text(const QString& value) -> void {
        trailing_kind = Trailing::TEXT;
        trailing_text = value;
        self.updateGeometry();
        self.update();
    }

    auto set_disabled(bool value) -> void {
        is_disabled = value;
        is_hovered = is_pressed = false;
        self.update();
    }

    auto paint_event(QPaintEvent*) -> void {
        using namespace drawable;

        const auto& tokens     = is_disabled ? color_space.disabled : color_space.enabled;
        const auto state_layer = is_disabled ? QColor { Qt::transparent }
            : is_pressed                     ? color_space.pressed_state_layer
            : is_hovered                     ? color_space.hover_state_layer
                                             : QColor { Qt::transparent };

        const auto w = self.width() * 1.;
        const auto h = self.height() * 1.;

        const auto icon_y      = (h - measurements.icon_size) / 2.;
        const auto has_leading = !leading_icon_code.isEmpty();

        auto trailing_width = double { 0 };
        if (trailing_kind == Trailing::ICON) trailing_width = measurements.icon_size;
        if (trailing_kind == Trailing::TEXT)
            trailing_width = QFontMetricsF { label_font }.horizontalAdvance(trailing_text);

        const auto leading_rect = QRectF {
            measurements.horizontal_padding * 1.,
            icon_y,
            measurements.icon_size * 1.,
            measurements.icon_size * 1.,
        };
        const auto trailing_rect = QRectF {
            w - measurements.horizontal_padding - trailing_width,
            icon_y,
            trailing_width,
            measurements.icon_size * 1.,
        };

        const auto text_left  = has_leading
            ? leading_rect.right() + 1 + measurements.icon_text_spacing
            : measurements.horizontal_padding * 1.;
        const auto text_right = trailing_width > 0
            ? trailing_rect.left() - measurements.icon_text_spacing
            : w - measurements.horizontal_padding;
        const auto text_rect  = QRectF { text_left, 0, text_right - text_left, h };
        const auto show_text  = !text.isEmpty() && text_rect.width() > 0;
        const auto elided =
            QFontMetricsF { label_font }.elidedText(text, Qt::ElideRight, text_rect.width());

        auto painter = QPainter { &self };
        DrawHere {
            painter,
            Rect {
              .size  = self.size(),
              .color = state_layer,
            },
            If { !is_disabled, water_ripple },
            If {
              has_leading,
              Surface {
                {
                  .size   = leading_rect.size(),
                  .origin = leading_rect.topLeft(),
                },
                Text {
                  .text  = leading_icon_code,
                  .font  = leading_icon_font,
                  .size  = leading_rect.size(),
                  .color = tokens.leading_icon,
                },
              },
            },
            If {
              show_text,
              Surface {
                {
                  .size   = text_rect.size(),
                  .origin = text_rect.topLeft(),
                },
                Text {
                  .text  = elided,
                  .font  = label_font,
                  .size  = text_rect.size(),
                  .color = tokens.label_text,
                  .align = Qt::AlignVCenter | Qt::AlignLeft,
                },
              },
            },
            If {
              trailing_kind == Trailing::ICON,
              Surface {
                {
                  .size   = trailing_rect.size(),
                  .origin = trailing_rect.topLeft(),
                },
                Text {
                  .text  = trailing_icon_code,
                  .font  = trailing_icon_font,
                  .size  = trailing_rect.size(),
                  .color = tokens.trailing_icon,
                },
              },
            },
            If {
              trailing_kind == Trailing::TEXT,
              Surface {
                {
                  .size   = trailing_rect.size(),
                  .origin = trailing_rect.topLeft(),
                },
                Text {
                  .text  = trailing_text,
                  .font  = label_font,
                  .size  = trailing_rect.size(),
                  .color = tokens.trailing_icon,
                  .align = Qt::AlignVCenter | Qt::AlignLeft,
                },
              },
            },
        };
    }

    auto enter_event(qt::EnterEvent*) -> void {
        is_hovered = true;
        self.update();
    }

    auto leave_event(QEvent*) -> void {
        is_hovered = false;
        is_pressed = false;
        self.update();
    }

    auto mouse_press_event(QMouseEvent* event) -> void {
        if (event->button() == Qt::LeftButton && !is_disabled) {
            is_pressed = true;
            if (enable_water_ripple) water_ripple.press(event->pos());
            self.update();
        }
    }

    auto mouse_release_event(QMouseEvent* event) -> void {
        if (!is_pressed) return;

        is_pressed = false;
        water_ripple.release();
        self.update();

        if (is_disabled || !self.rect().contains(event->pos())) return;

        Q_EMIT self.clicked();
    }
};

DropdownMenuItem::DropdownMenuItem()
    : pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenuItem::~DropdownMenuItem() = default;

auto DropdownMenuItem::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->loadColorScheme(scheme);
}

auto DropdownMenuItem::bindThemeManager(ThemeManager& manager) -> void {
    pimpl->bindThemeManager(manager);
}

auto DropdownMenuItem::setMeasurements(const Measurements& measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

auto DropdownMenuItem::setText(const QString& text) -> void {
    pimpl->set_text(text);
}

auto DropdownMenuItem::setLeadingIcon(const QString& code, const QString& font) -> void {
    pimpl->set_leading_icon(code, font);
}

auto DropdownMenuItem::setTrailingIcon(const QString& code, const QString& font) -> void {
    pimpl->set_trailing_icon(code, font);
}

auto DropdownMenuItem::setTrailingText(const QString& text) -> void {
    pimpl->set_trailing_text(text);
}

auto DropdownMenuItem::setDisabled(bool disabled) -> void {
    pimpl->set_disabled(disabled);
}

auto DropdownMenuItem::setWaterColor(const QColor& color) -> void {
    pimpl->water_color = color;
    pimpl->water_ripple.setColor(color);
    update();
}

auto DropdownMenuItem::setWaterRippleStatus(bool enable) -> void {
    pimpl->enable_water_ripple = enable;
}

auto DropdownMenuItem::sizeHint() const -> QSize {
    return { pimpl->content_width(), pimpl->measurements.height };
}

auto DropdownMenuItem::enterEvent(qt::EnterEvent* event) -> void {
    pimpl->enter_event(event);
    QWidget::enterEvent(event);
}

auto DropdownMenuItem::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event(event);
    QWidget::leaveEvent(event);
}

auto DropdownMenuItem::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(event);
    QWidget::mousePressEvent(event);
}

auto DropdownMenuItem::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event(event);
    QWidget::mouseReleaseEvent(event);
}

auto DropdownMenuItem::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_event(event);
}

auto DropdownMenuItem::resizeEvent(QResizeEvent* event) -> void {
    auto clip = QPainterPath { };
    clip.addRect(rect());

    pimpl->water_ripple.setSize(size());
    pimpl->water_ripple.setClipPath(clip);

    QWidget::resizeEvent(event);
}
