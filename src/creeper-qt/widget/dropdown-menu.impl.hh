#pragma once
#include "dropdown-menu.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/material-icon.hh"
#include "creeper-qt/utility/painter/common.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/painter/shape.hh"
#include "creeper-qt/widget/menu.hh"

#include <qfontmetrics.h>

using namespace creeper::dropdown_menu::details;

struct DropdownMenu::Impl {
public:
    static constexpr auto measure_text(
        const QFont& font, const QString& text, const QTextOption& options) {
        const auto fm   = QFontMetricsF(font);
        const auto size = fm.size(Qt::TextSingleLine, text);
        return size.width();
    }

    explicit Impl(DropdownMenu& self) noexcept
        : animatable(self)
        , self { self } {
        {
            auto state            = std::make_shared<PidState<double>>();
            state->config.kp      = 20.0;
            state->config.ki      = 00.0;
            state->config.kd      = 00.0;
            state->config.epsilon = 1e-2;
            label_position        = make_transition(animatable, std::move(state));
        }
        {
            popup = new creeper::Menu { };
            popup->setParent(
                &self, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

            QObject::connect(popup, &Menu::signal_index_changed, &self, [this](int index) {
                set_current_index(index);
                Q_EMIT this->self.signal_index_changed(index);

                expanded = false;
                update_label_position();
                this->self.update();
            });
            QObject::connect(popup, &Menu::signal_dismissed, &self, [this] {
                expanded = false;
                update_label_position();
                this->self.update();
            });
        }

        set_measurements(Measurements { });
    }

    auto set_color_scheme(const ColorScheme& scheme) -> void {
        color_space.enabled.container        = scheme.surface_container_highest;
        color_space.enabled.label_text       = scheme.on_surface_variant;
        color_space.enabled.selected_text    = scheme.on_surface;
        color_space.enabled.leading_icon     = scheme.on_surface_variant;
        color_space.enabled.active_indicator = scheme.on_surface_variant;
        color_space.enabled.supporting_text  = scheme.on_surface_variant;
        color_space.enabled.input_text       = scheme.on_surface;
        color_space.enabled.caret            = scheme.primary;
        color_space.enabled.outline          = scheme.outline;

        color_space.disabled.container = scheme.on_surface;
        color_space.disabled.container.setAlphaF(0.04);
        color_space.disabled.label_text = scheme.on_surface;
        color_space.disabled.label_text.setAlphaF(0.38);
        color_space.disabled.selected_text = scheme.on_surface;
        color_space.disabled.selected_text.setAlphaF(0.38);
        color_space.disabled.leading_icon = scheme.on_surface;
        color_space.disabled.leading_icon.setAlphaF(0.38);
        color_space.disabled.supporting_text = scheme.on_surface;
        color_space.disabled.supporting_text.setAlphaF(0.38);
        color_space.disabled.input_text = scheme.on_surface;
        color_space.disabled.input_text.setAlphaF(0.38);
        color_space.disabled.active_indicator = scheme.on_surface;
        color_space.disabled.active_indicator.setAlphaF(0.38);
        color_space.disabled.outline = scheme.outline;
        color_space.disabled.outline.setAlphaF(0.38);

        color_space.focused.container        = scheme.surface_container_highest;
        color_space.focused.label_text       = scheme.primary;
        color_space.focused.selected_text    = scheme.on_surface;
        color_space.focused.leading_icon     = scheme.on_surface_variant;
        color_space.focused.input_text       = scheme.on_surface;
        color_space.focused.supporting_text  = scheme.on_surface_variant;
        color_space.focused.active_indicator = scheme.primary;
        color_space.focused.outline          = scheme.primary;

        color_space.error.container        = scheme.surface_container_highest;
        color_space.error.active_indicator = scheme.error;
        color_space.error.label_text       = scheme.error;
        color_space.error.selected_text    = scheme.on_surface;
        color_space.error.input_text       = scheme.on_surface;
        color_space.error.supporting_text  = scheme.error;
        color_space.error.leading_icon     = scheme.on_surface_variant;
        color_space.error.caret            = scheme.error;
        color_space.error.outline          = scheme.error;

        color_space.state_layer = scheme.on_surface;
        color_space.state_layer.setAlphaF(0.08);

        self.update();
    }

    auto load_theme_manager(ThemeManager& manager) {
        manager.append_handler(&self,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
        popup->load_theme_manager(manager);
    }

    auto set_label_text(const QString& text) { label_text = text; }

    auto set_leading_icon(const QString& code, const QString& font) {
        leading_icon_code          = code;
        leading_icon_font          = font;
        is_update_component_status = false;
    }

    auto set_measurements(const Measurements& measurements) -> void {
        this->measurements = measurements;
        self.setFixedHeight(measurements.container_height + measurements.standard_font_height);
        is_update_component_status = false;
    }

    auto set_items(const QStringList& value) -> void {
        items          = value;
        selected_index = -1;

        popup->set_items(items);
        popup->set_current_index(-1);

        update_label_position();
        self.update();
    }

    auto set_current_index(int index) -> void {
        selected_index = std::clamp(index, -1, static_cast<int>(items.size()) - 1);

        popup->set_current_index(selected_index);

        update_label_position();
        self.update();
    }

    auto current_text() const -> QString { return items.value(selected_index); }

    auto set_expanded(bool value) -> void {
        if (expanded == value) return;
        if (value && items.isEmpty()) return;

        expanded = value;
        if (expanded) {
            popup->show_for(self, container_rect());
        } else {
            popup->hide();
        }

        update_label_position();
        self.update();
    }

    auto mouse_press_event(QMouseEvent*) -> void { set_expanded(!expanded); }

    auto key_press_event(QKeyEvent* event) -> void {
        switch (event->key()) {
        case Qt::Key_Down:
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Space:
            set_expanded(true);
            break;
        default:
            break;
        }
    }

    auto paint_filled(QPaintEvent*) -> void {
        const auto color = get_color_tokens();

        constexpr auto container_radius = 5;
        update_component_status();

        auto painter = QPainter { &self };

        // Draw container with fixed measurements height and vertically centered
        const auto container_rect = this->container_rect();

        {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color.container, Qt::transparent, 0, container_rect,
                    container_radius, container_radius, 0, 0);
        }

        // Active indicator at container bottom
        {
            const auto p0 = container_rect.bottomLeft();
            const auto p1 = container_rect.bottomRight();
            painter.setBrush(Qt::NoBrush);
            painter.setPen({ color.active_indicator, filled_line_width() });
            painter.drawLine(p0, p1);
        }

        // Icon positioned relative to container_rect
        const auto rect_icon = QRectF {
            container_rect.right() - margins.right() - measurements.icon_rect_size * 1.,
            container_rect.top() + (container_rect.height() - measurements.icon_rect_size) * 0.5,
            1. * measurements.icon_rect_size,
            1. * measurements.icon_rect_size,
        };
        const auto icon_center = rect_icon.center();

        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { color.leading_icon });
        painter.setFont(leading_icon_font);
        painter.translate(icon_center);
        painter.rotate(expanded ? 180.0 : 0.0);
        painter.translate(-icon_center);
        painter.drawText(rect_icon, leading_icon_code, { Qt::AlignCenter });
        painter.restore();

        if (!label_text.isEmpty()) {
            const auto center_label_y = container_rect.top()
                + (measurements.container_height - measurements.label_rect_size) / 2.0;

            const auto rect_center = QRectF {
                QPointF { static_cast<double>(margins.left()), center_label_y },
                QPointF(container_rect.right() - margins.right(),
                    center_label_y + measurements.label_rect_size),
            };

            const auto rect_top = QRectF {
                QPointF(margins.left(), container_rect.top() + measurements.col_padding),
                QPointF(container_rect.right() - margins.right(),
                    container_rect.top() + measurements.col_padding + measurements.label_rect_size),
            };

            const auto position     = selected_index < 0 ? *label_position : 1.;
            const auto label_rect   = animate::interpolate(rect_center, rect_top, position);
            const auto scale        = 1. - position * 0.25;
            const auto label_anchor = QPointF { label_rect.left(), label_rect.center().y() };

            painter.save();
            painter.translate(label_anchor);
            painter.scale(scale, scale);
            painter.translate(-label_anchor);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.label_text });
            painter.setFont(standard_text_font);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawText(label_rect, label_text, { Qt::AlignVCenter | Qt::AlignLeading });
            painter.restore();

            if (selected_index != -1) {
                painter.save();
                // Place selected text in the input area (below the floating label)
                const auto input_top =
                    container_rect.top() + measurements.col_padding + measurements.label_rect_size;
                const auto input_bottom = container_rect.bottom() - measurements.col_padding;
                const auto rect_center_selected = QRectF {
                    QPointF { static_cast<double>(margins.left()), static_cast<double>(input_top) },
                    QPointF(container_rect.right() - margins.right(),
                        static_cast<double>(input_bottom)),
                };

                // Draw selected text with input text color
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen { color.selected_text });
                painter.setFont(standard_text_font);
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawText(
                    rect_center_selected, current_text(), Qt::AlignVCenter | Qt::AlignLeading);

                painter.restore();
            }
        } else if (label_text.isEmpty() && selected_index != -1) {
            const auto input_top = container_rect.top()
                + (container_rect.height() - measurements.input_rect_size) / 2.0;
            const auto input_bottom  = input_top + measurements.input_rect_size;
            const auto rect_selected = QRectF {
                QPointF(margins.left(), input_top),
                QPointF(container_rect.right() - margins.right(), input_bottom),
            };

            // Draw selected text
            painter.save();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.selected_text });
            painter.setFont(standard_text_font);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawText(rect_selected, current_text(), Qt::AlignVCenter | Qt::AlignLeading);
            painter.restore();
        }

        if (is_hovered) {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color_space.state_layer, Qt::transparent, 0, container_rect,
                    container_radius, container_radius, 0, 0);
        }
    }

    auto paint_outlined(QPaintEvent*) -> void {
        const auto& measurements = this->measurements;
        const auto color_tokens  = get_color_tokens();

        update_component_status();

        using namespace painter;
        using namespace painter::common::pro;
        auto painter = qt::painter { &self };

        const auto container_width  = self.width();
        const auto container_height = measurements.container_height;
        const auto container_size   = qt::size(container_width, container_height);

        const auto container_thickness = expanded || is_focused ? 2. : is_hovered ? 1.5 : 1.;

        const auto position   = selected_index < 0 ? *label_position : 1.;
        const auto text_scale = animate::interpolate(1., 0.75, position);

        auto text_option = qt::text_option { };
        text_option.setWrapMode(QTextOption::NoWrap);
        text_option.setAlignment(Qt::AlignLeading | Qt::AlignVCenter);

        const auto text_width = measure_text(standard_text_font, label_text, text_option);

        auto label_origin = qt::point { };
        auto label_size   = qt::size { };
        {
            const auto begin_y = (container_height - measurements.label_rect_size) / 2.0;
            const auto final_y = -0.5 * measurements.standard_font_height;

            const auto begin_origin =
                qt::point(measurements.row_padding_widthout_icons * 1., begin_y);
            const auto final_origin =
                qt::point(measurements.row_padding_widthout_icons * 1., final_y);

            const auto begin_size = qt::size(text_width, measurements.label_rect_size * 1.);
            const auto final_size =
                qt::size(text_scale * text_width, measurements.standard_font_height * 1.);

            label_origin = animate::interpolate(begin_origin, final_origin, position);
            label_size   = animate::interpolate(begin_size, final_size, position);
        }
        const auto label_background_size = label_text.isEmpty()
            ? qt::size(0, 0)
            : qt::size(label_size.width() + 2 * measurements.row_padding_populated_label_text,
                  label_size.height());

        Paint::Box {
            BoxImpl { self.size(), Qt::AlignCenter },
            Paint::Surface {
                SurfaceImpl { container_size },
                Paint::Buffer {
                    BufferImpl { container_size },
                    Paint::RoundedRectangle {
                        Size { container_size },
                        Outline { color_tokens.outline, container_thickness },
                        Radiuses { 5 },
                    },
                    Paint::EraseRectangle {
                        Origin { label_origin },
                        Size { label_background_size },
                    },
                },
                Paint::Box {
                    BoxImpl { label_background_size, Qt::AlignHCenter, label_origin },
                    Paint::Text {
                        TextOption { text_option },
                        Font { standard_text_font },
                        Size { label_background_size },
                        Text { label_text },
                        Color { color_tokens.label_text },
                        Scale { text_scale },
                    },
                },
            },
        }(painter);

        const auto container_rect = this->container_rect();

        const auto rect_icon = QRectF {
            container_rect.right() - margins.right() - measurements.icon_rect_size * 1.,
            container_rect.top() + (container_rect.height() - measurements.icon_rect_size) * 0.5,
            1. * measurements.icon_rect_size,
            1. * measurements.icon_rect_size,
        };

        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { color_tokens.leading_icon });
        painter.setFont(leading_icon_font);
        painter.translate(rect_icon.center());
        painter.rotate(expanded ? 180.0 : 0.0);
        painter.translate(-rect_icon.center());
        painter.drawText(rect_icon, leading_icon_code, { Qt::AlignCenter });
        painter.restore();

        if (selected_index != -1) {
            const auto input_top = container_rect.top()
                + (container_rect.height() - measurements.input_rect_size) / 2.0;
            const auto text_rect = QRectF {
                measurements.row_padding_widthout_icons * 1.,
                input_top,
                rect_icon.left() - 2. * measurements.row_padding_widthout_icons,
                measurements.input_rect_size * 1.,
            };

            painter.save();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color_tokens.selected_text });
            painter.setFont(standard_text_font);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawText(text_rect, current_text(), Qt::AlignVCenter | Qt::AlignLeading);
            painter.restore();
        }
    }

    auto enter_event(qt::EnterEvent*) {
        is_hovered = true;
        self.update();
    }
    auto leave_event(QEvent*) {
        is_hovered = false;
        self.update();
    }

    auto focus_in(QFocusEvent*) {
        is_focused = true;
        update_label_position();
        self.update();
    }

    auto focus_out(QFocusEvent*) {
        is_focused = false;
        update_label_position();
        self.update();
    }

private:
    auto update_component_status() -> void {
        if (is_update_component_status) {
            return;
        }

        auto font = self.font();
        font.setPixelSize(measurements.standard_font_height);
        self.setFont(font);
        standard_text_font = self.font();
        standard_text_font.setPixelSize(measurements.standard_font_height);

        is_update_component_status = true;
    }

    auto update_label_position() -> void {
        if (is_focused || expanded) {
            label_position->transition_to(1.0);
        } else {
            label_position->transition_to(0.0);
        }
    }

    auto get_color_tokens() const -> ColorSpace::Tokens const& {
        return is_disable ? color_space.disabled
            : is_error    ? color_space.error
            : expanded    ? color_space.focused
            : is_focused  ? color_space.focused
                          : color_space.enabled;
    }

    auto filled_line_width() const -> double { return 1.5; }

    auto container_rect() const noexcept -> QRect {
        return QRect {
            0,
            (self.height() - measurements.container_height) / 2,
            self.width(),
            measurements.container_height,
        };
    }

public:
    Measurements measurements;
    ColorSpace color_space;
    QMargins margins { 13, 24, 13, 0 };

    QStringList items;
    int selected_index = -1;
    bool expanded      = false;

    creeper::Menu* popup = nullptr;

    bool is_disable = false;
    bool is_hovered = false;
    bool is_focused = false;
    bool is_error   = false;

    bool is_update_component_status = false;

    QString label_text;
    QIcon leading_icon;
    QString leading_icon_code = material::icon::kArrowDropDown;
    QFont leading_icon_font   = material::round::font_1;

    QFont standard_text_font;

    Animatable animatable;
    std::unique_ptr<TransitionValue<PidState<double>>> label_position;

    DropdownMenu& self;
};
