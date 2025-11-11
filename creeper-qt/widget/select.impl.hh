#pragma once
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/common.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "select.hh"
#include <qfontmetrics.h>
#include <qabstractitemview.h>

using namespace creeper::select_widget::internal;

struct BasicSelect::Impl {
public:
    explicit Impl(BasicSelect& self) noexcept
        :self {self}, animatable_label(self) {
        {
            auto state = std::make_shared<PidState<double>>();
            state->config.kp        = 20.0;
            state->config.ki        = 00.0;
            state->config.kd        = 00.0;
            state->config.epsilon   = 1e-2;
            label_position = make_transition(animatable_label, std::move(state));
        }

        set_measurements(Measurements{});
    }

    auto set_color_scheme(const ColorScheme& scheme) -> void {
        color_space.enabled.container        = scheme.surface_container_highest;
        color_space.enabled.label_text       = scheme.on_surface_variant;
        color_space.enabled.selected_text    = scheme.on_surface_variant;
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
        color_space.disabled.selected_text.setAlphaF(0.04);
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
        color_space.focused.selected_text = scheme.on_surface_variant;
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
        color_space.selection_container = scheme.primary;
        color_space.selection_container.setAlphaF(0.38);

        const auto& color = get_color_tokens();
        sync_basic_text_style(
            color.input_text,
            scheme.surface_container_highest,
            color.input_text,
            color_space.selection_container,
            color_space.selection_container
        );
    }

    auto load_theme_manager(ThemeManager& manager) {
        manager.append_handler(&self, [this](const ThemeManager& manager) {
            set_color_scheme(manager.color_scheme());
        });
    }

    auto set_label_text(const QString& text) {
        label_text = text;
    }

    auto set_leading_icon(const QString& code, const QString& font) {
        leading_icon_code = code;
        leading_icon_font = font;
        is_update_component_status = false;
    }

    auto set_measurements(const Measurements& measurements) -> void {
        this->measurements = measurements;
        self.setFixedHeight(measurements.container_height + measurements.standard_font_height);
        is_update_component_status = false;
    }

    auto paint_filled(QPaintEvent*) -> void {
        const auto rect = self.rect();
        const auto color = get_color_tokens();


        constexpr auto container_radius = 5;
        update_component_status();

        auto painter = QPainter{&self};

        {
            util::PainterHelper{painter}
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color.container, Qt::transparent, 0,
                    rect, container_radius, container_radius, 0, 0);
        }

        {
            const auto p0 = rect.bottomLeft();
            const auto p1 = rect.bottomRight();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen{color.active_indicator, filled_line_width()});
            painter.drawLine(p0, p1);
        }

        const auto rect_icon = QRectF {
            1.0 * (self.width() - self.textMargins().right() - measurements.icon_rect_size),
            0.5 * (self.height() - measurements.icon_rect_size),
            1.0 * measurements.icon_rect_size,
            1.0 * measurements.icon_rect_size,
        };

        painter.save();

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen {color.leading_icon});
        painter.setFont(leading_icon_font);

        const auto icon_center = rect_icon.center();
        painter.translate(icon_center);
        const bool is_active = (self.view() && self.view()->isVisible());
        painter.rotate(is_active ? 180.0 : 0.0);
        painter.translate(-icon_center);
        painter.drawText(rect_icon, leading_icon_code, {Qt::AlignCenter});

        painter.restore();

        if (!label_text.isEmpty()) {
            const auto margins = self.textMargins();

            const auto center_label_padding = 0.5 * (measurements.container_height);
            const auto rect_center = QRectF {
                QPointF {static_cast<double>(margins.left()), center_label_padding},
                QPointF (self.width() - margins.right(), self.height() - center_label_padding),
            };
            const auto rect_top = QRectF {
                QPointF (margins.left(), measurements.col_padding),
                QPointF (self.width() - margins.right(), margins.top()),
            };

            const auto position = self.currentText().isEmpty() ? *label_position : 1.;
            const auto label_rect = animate::interpolate(rect_center, rect_top, position);
            const auto scale = 1. - position * 0.25;

            painter.save();
            const auto label_anchor = QPointF {label_rect.left(), label_rect.center().y()};
            painter.translate(label_anchor);
            painter.scale(scale, scale);
            painter.translate(-label_anchor);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen {color.label_text});
            painter.setFont(standard_text_font);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawText(label_rect, label_text, {Qt::AlignVCenter | Qt::AlignLeading});
            painter.restore();


            if (self.currentIndex() != -1) {
                painter.save();
                const auto rect_center_selected = QRectF {
                    QPointF {static_cast<double>(margins.left()), center_label_padding},
                    QPointF (self.width() - margins.right(), self.height() - 0.5 * center_label_padding),
                };
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen {color.selected_text});
                painter.setFont(standard_text_font);
                painter.setRenderHint(QPainter::Antialiasing);
                painter.drawText(rect_center_selected, self.currentText(), Qt::AlignVCenter | Qt::AlignLeading);
                painter.restore();
            }
        }
        else if (!label_text.isEmpty() && self.currentIndex() != -1) {
            const auto selected_text = self.currentText();
            const auto position = self.currentText().isEmpty() ? *label_position : 1.;
        }

        if (is_hovered) {
            util::PainterHelper{painter}
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color_space.state_layer, Qt::transparent, 0, rect,
                    container_radius, container_radius, 0, 0);
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

    auto show_popup() {
        if (self.count() == 0) {
            return;
        }

        is_active = true;
        self.QComboBox::showPopup();
        update_label_position();
        self.update();
    }

    auto hide_popup() -> void {
        is_active = false;
        self.QComboBox::hidePopup();
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
        if ((is_focused || is_active) && self.currentIndex() != -1) {
            label_position->transition_to(1.0);
        }
        else if (is_focused || is_active) {
            label_position->transition_to(1.0);
        }
        else {
            label_position->transition_to(0.0);
        }
    }

    auto sync_basic_text_style(const QColor& text, const QColor& background,
        const QColor& selection_text, const QColor& selection_background,
        const QColor& hover) -> void {
        constexpr auto to_rgba = [](const QColor& color) {
            return QStringLiteral("rgba(%1, %2, %3, %4)")
                .arg(color.red())
                .arg(color.green())
                .arg(color.blue())
                .arg(color.alpha());
        };

        constexpr auto kQComboBoxStyle = R"(
            QComboBox QAbstractItemView {
                border: none;
                background-color: %1;
                outline: none;
            }
            QComboBox QAbstractItemView::item {
                padding: 3px 5px;
                color: %2;
            }
            QComboBox QAbstractItemView::item:selected {
                background-color: %3;
                color: %4;
            }
            QComboBox QAbstractItemView::item:hover {
                background-color: %5;
                color: %6;
            }
        )";

        const auto qss = QString {kQComboBoxStyle};
        self.setStyleSheet(qss.arg(to_rgba(background))
            .arg(to_rgba(text))
            .arg(to_rgba(selection_background))
            .arg(to_rgba(selection_text))
            .arg(to_rgba(hover))
            .arg(to_rgba(text))
        );
    }

    auto get_color_tokens() const -> ColorSpace::Tokens const& {
        return is_disable ? color_space.disabled
            : is_error    ? color_space.error
            : is_active   ? color_space.focused
            : is_focused  ? color_space.focused
                          : color_space.enabled;
    }

    auto filled_line_width() const -> double {
        constexpr auto normal_width = 1;
        constexpr auto active_width = 3;
        return (is_focused && !is_disable) ? active_width : normal_width;
    }

    constexpr auto measure_text(const QFont& font, const QString& text, const QTextOption& options) {
        const auto fm = QFontMetricsF(font);
        const auto size = fm.size(Qt::TextSingleLine, text);
        return size.width();
    }

private:
    Measurements measurements;
    ColorSpace color_space;
    bool is_disable;
    bool is_hovered;
    bool is_focused;
    bool is_error;
    bool is_update_component_status;
    bool is_active;

    QString label_text;
    QIcon leading_icon;
    QString leading_icon_code = material::icon::kArrowDropDown;
    QFont leading_icon_font = material::round::font_1;

    QFont standard_text_font;

    Animatable animatable_label;
    std::unique_ptr<TransitionValue<PidState<double>>> label_position;
    std::unique_ptr<TransitionValue<PidState<double>>> label_size;
    BasicSelect& self;
};