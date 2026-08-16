#include "dropdown-menu-item.hh"

#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/painter/helper.hh"

#include <cmath>

#include <qevent.h>
#include <qpainter.h>

using namespace creeper::dropdown_menu_item::details;

struct DropdownMenuItem::Impl {
    enum class Trailing { None, Icon, Text };

    Measurements measurements;
    ColorSpace color_space;

    QString text;

    QString leading_icon_code;
    QFont leading_icon_font;

    Trailing trailing_kind = Trailing::None;
    QString trailing_icon_code;
    QFont trailing_icon_font;
    QString trailing_text;

    bool is_disabled = false;
    bool is_hovered  = false;
    bool is_pressed  = false;

    QFont label_font;

    Animatable animatable;
    WaterRippleRenderer water_ripple;
    QColor water_color;
    bool enable_water_ripple = true;

    DropdownMenuItem& self;

    explicit Impl(DropdownMenuItem& self) noexcept
        : animatable { self }
        , water_ripple { animatable, 5.0 }
        , self { self } {
        self.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        set_measurements(Measurements { });
    }

    auto set_color_scheme(const ColorScheme& scheme) -> void {
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
        water_color.setAlphaF(0.12);

        self.update();
    }

    auto load_theme_manager(ThemeManager& manager) -> void { manager.append_handler(self); }

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
        if (trailing_kind == Trailing::Icon)
            width += measurements.icon_text_spacing + measurements.icon_size;
        if (trailing_kind == Trailing::Text)
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
        trailing_kind      = Trailing::Icon;
        trailing_icon_code = code;
        if (!font.isEmpty()) trailing_icon_font = QFont { font };
        trailing_icon_font.setPixelSize(measurements.icon_size);
        self.updateGeometry();
        self.update();
    }

    auto set_trailing_text(const QString& value) -> void {
        trailing_kind = Trailing::Text;
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
        const auto& tokens = is_disabled ? color_space.disabled : color_space.enabled;

        auto painter = QPainter { &self };

        if (!is_disabled && is_pressed) {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(
                    color_space.pressed_state_layer, Qt::transparent, 0, self.rect(), 0, 0);
        } else if (!is_disabled && is_hovered) {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(
                    color_space.hover_state_layer, Qt::transparent, 0, self.rect(), 0, 0);
        }

        if (!is_disabled) {
            auto clip = QPainterPath { };
            clip.addRect(self.rect());
            util::PainterHelper { painter }.apply(water_ripple.renderer(clip, water_color));
        }

        const auto w = self.width() * 1.;
        const auto h = self.height() * 1.;

        const auto icon_y      = (h - measurements.icon_size) / 2.;
        const auto has_leading = !leading_icon_code.isEmpty();

        auto trailing_width = double { 0 };
        if (trailing_kind == Trailing::Icon) trailing_width = measurements.icon_size;
        if (trailing_kind == Trailing::Text)
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

        auto helper = util::PainterHelper { painter }.set_render_hint(QPainter::Antialiasing);

        if (has_leading) {
            helper.simple_text(leading_icon_code, leading_icon_font, tokens.leading_icon,
                leading_rect, Qt::AlignCenter);
        }

        if (!text.isEmpty() && text_rect.width() > 0) {
            const auto elided =
                QFontMetricsF { label_font }.elidedText(text, Qt::ElideRight, text_rect.width());
            helper.simple_text(
                elided, label_font, tokens.label_text, text_rect, Qt::AlignVCenter | Qt::AlignLeft);
        }

        if (trailing_kind == Trailing::Icon) {
            helper.simple_text(trailing_icon_code, trailing_icon_font, tokens.trailing_icon,
                trailing_rect, Qt::AlignCenter);
        } else if (trailing_kind == Trailing::Text) {
            helper.simple_text(trailing_text, label_font, tokens.trailing_icon, trailing_rect,
                Qt::AlignVCenter | Qt::AlignLeft);
        }
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
            self.update();
        }
    }

    auto mouse_release_event(QMouseEvent* event) -> void {
        if (!is_pressed) return;

        is_pressed = false;
        self.update();

        if (is_disabled || !self.rect().contains(event->pos())) return;

        if (enable_water_ripple)
            water_ripple.clicked(event->pos(), std::max(self.width(), self.height()));

        Q_EMIT self.signal_clicked();
    }
};

DropdownMenuItem::DropdownMenuItem()
    : pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenuItem::~DropdownMenuItem() = default;

auto DropdownMenuItem::set_color_scheme(const ColorScheme& scheme) -> void {
    pimpl->set_color_scheme(scheme);
}

auto DropdownMenuItem::load_theme_manager(ThemeManager& manager) -> void {
    pimpl->load_theme_manager(manager);
}

auto DropdownMenuItem::set_measurements(const Measurements& measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

auto DropdownMenuItem::set_text(const QString& text) -> void { pimpl->set_text(text); }

auto DropdownMenuItem::set_leading_icon(const QString& code, const QString& font) -> void {
    pimpl->set_leading_icon(code, font);
}

auto DropdownMenuItem::set_trailing_icon(const QString& code, const QString& font) -> void {
    pimpl->set_trailing_icon(code, font);
}

auto DropdownMenuItem::set_trailing_text(const QString& text) -> void {
    pimpl->set_trailing_text(text);
}

auto DropdownMenuItem::set_disabled(bool disabled) -> void { pimpl->set_disabled(disabled); }

auto DropdownMenuItem::set_water_color(const QColor& color) -> void {
    pimpl->water_color = color;
    update();
}

auto DropdownMenuItem::set_water_ripple_status(bool enable) -> void {
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

auto DropdownMenuItem::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }
