#include "filled-button.hh"

#include "utility/painter/helper.hh"
using creeper::util::PainterHelper;

#include "utility/animation/core.hh"
#include "utility/animation/gradient-color.hh"
#include "utility/animation/water-ripple.hh"
using namespace creeper::util::animation;

#include "utility/theme/theme.hh"

namespace creeper::filled_button::internal {

struct FilledButton::Impl {
public:
    AnimationCore animation_core;
    WaterRippleContainer water_ripples;

    bool enable_water_ripple = true;
    double water_ripple_step = 5.;

    double radius     = 0;
    QColor text_color = Qt::black;
    QColor background = Qt::white;

    QColor border_color = Qt::red;
    double border_width = 0;

    QColor water_color = Qt::black;

    QColor kHoverColor  = QColor { 0, 0, 0, 30 };
    QColor hover_color  = Qt::transparent;
    bool is_mouse_hover = false;

public:
    explicit Impl(QAbstractButton& self)
        : animation_core([&self] { self.update(); }, 90) { }

    void paint_event(QAbstractButton& self, QPaintEvent* event) {
        const auto water_renderer = [&self, this](QPainter& painter) {
            water_ripples.render(painter, self.rect(), self.rect());
        };

        auto painter = QPainter { &self };
        PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)
            .set_opacity(1.)
            .rounded_rectangle(background, border_color, border_width, self.rect(), radius, radius)
            .apply(water_renderer)
            .set_opacity(1.)
            .rounded_rectangle(hover_color, Qt::transparent, 0, self.rect(), radius, radius)
            .set_opacity(1.)
            .simple_text(self.text(), self.font(), text_color, self.rect(), Qt::AlignCenter);
    }

    void mouse_release_event(QAbstractButton& self, QMouseEvent* event) {
        if (enable_water_ripple) {

            const auto button_path  = make_rounded_rectangle_path(self.rect(), radius);
            const auto max_distance = std::max<double>(self.width(), self.height());

            animation_core.append(std::make_unique<WaterRipple>(water_color, button_path,
                event->pos(), water_ripple_step, max_distance,
                [this](std::unique_ptr<WaterRipple::Result> result) -> bool {
                    return water_ripples.append(std::move(result)), false;
                }));
        }
    }

    void enter_event(QAbstractButton& self, QEvent* event) {
        animation_core.append(std::make_unique<GradientColor>(hover_color, kHoverColor, 0.1,
            [this](const QColor& color) { return hover_color = color, !is_mouse_hover; }));
        is_mouse_hover = true;
    }

    void leave_event(QAbstractButton& self, QEvent* event) {
        animation_core.append(std::make_unique<GradientColor>(hover_color, Qt::transparent, 0.1,
            [this](const QColor& color) { return hover_color = color, is_mouse_hover; }));
        is_mouse_hover = false;
    }

private:
    static QPainterPath make_rounded_rectangle_path(const QRectF& rect, double radius) {
        auto path = QPainterPath {};
        path.addRoundedRect(rect, radius, radius);
        return path;
    }
};

FilledButton::FilledButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

FilledButton::~FilledButton() = default;

void FilledButton::set_color_scheme(const ColorScheme& color_scheme) {
    pimpl->background = color_scheme.primary;
    pimpl->text_color = color_scheme.on_primary;

    if (color_scheme.primary.lightness() > 128) {
        pimpl->water_color = color_scheme.primary.darker(130);
        pimpl->kHoverColor = QColor { 000, 000, 000, 30 };
    } else {
        pimpl->water_color = color_scheme.primary.lighter(130);
        pimpl->kHoverColor = QColor { 255, 255, 255, 30 };
    }
    pimpl->water_color.setAlphaF(0.25);

    update();
}

void FilledButton::load_theme_manager(ThemeManager& manager) {
    manager.append_handler(this, [this](const ThemeManager& manager) {
        const auto color_mode   = manager.color_mode();
        const auto theme_pack   = manager.theme_pack();
        const auto color_scheme = color_mode == ColorMode::LIGHT //
            ? theme_pack.light
            : theme_pack.dark;
        set_color_scheme(color_scheme);
    });
}

// 属性设置接口实现

void FilledButton::set_radius(double radius) { pimpl->radius = radius; }

void FilledButton::set_border_width(double border) { pimpl->border_width = border; }

void FilledButton::set_border_color(const QColor& color) { pimpl->border_color = color; }

void FilledButton::set_water_color(const QColor& color) { pimpl->water_color = color; }

void FilledButton::set_text_color(const QColor& color) { pimpl->text_color = color; }

void FilledButton::set_background(const QColor& color) { pimpl->background = color; }

void FilledButton::set_hover_color(const QColor& color) { pimpl->kHoverColor = color; }

void FilledButton::set_water_ripple_status(bool enable) { pimpl->enable_water_ripple = enable; }

void FilledButton::set_water_ripple_step(double step) { pimpl->water_ripple_step = step; }

// Qt 接口重载

void FilledButton::mouseReleaseEvent(QMouseEvent* event) {
    pimpl->mouse_release_event(*this, event);
    QAbstractButton::mouseReleaseEvent(event);
}
void FilledButton::paintEvent(QPaintEvent* event) {
    pimpl->paint_event(*this, event);
    /* Disable QAbstractButton::paintEvent */;
}
void FilledButton::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, event);
    QAbstractButton::enterEvent(event);
}
void FilledButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, event);
    QAbstractButton::leaveEvent(event);
}

}
