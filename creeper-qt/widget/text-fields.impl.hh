#include "text-fields.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/common.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/painter/shape.hh"

#include <qpainter.h>
#include <qpainterpath.h>

using namespace creeper::text_field::internal;

struct BasicTextField::Impl {
public:
    explicit Impl(BasicTextField& self) noexcept
        : self { self }
        , animatable { self } {

        {
            auto state = std::make_shared<PidState<double>>();

            state->config.kp      = 20.0;
            state->config.ki      = 00.0;
            state->config.kd      = 00.0;
            state->config.epsilon = 1e-2;

            label_position = make_transition(animatable, std::move(state));
        }

        self.setAlignment(Qt::AlignVCenter);
        set_measurements(Measurements {});
    }

    /// @note https://m3.material.io/components/text-fields/specs
    auto set_color_scheme(const ColorScheme& scheme) -> void {

        color_specs.enabled.container        = scheme.surface_container_highest;
        color_specs.enabled.label_text       = scheme.on_surface_variant;
        color_specs.enabled.leading_icon     = scheme.on_surface_variant;
        color_specs.enabled.trailing_icon    = scheme.on_surface_variant;
        color_specs.enabled.active_indicator = scheme.on_surface_variant;
        color_specs.enabled.supporting_text  = scheme.on_surface_variant;
        color_specs.enabled.input_text       = scheme.on_surface;
        color_specs.enabled.caret            = scheme.primary;
        color_specs.enabled.outline          = scheme.outline;

        color_specs.disabled.container = scheme.on_surface;
        color_specs.disabled.container.setAlphaF(0.04);
        color_specs.disabled.label_text = scheme.on_surface;
        color_specs.disabled.label_text.setAlphaF(0.38);
        color_specs.disabled.leading_icon = scheme.on_surface;
        color_specs.disabled.leading_icon.setAlphaF(0.38);
        color_specs.disabled.trailing_icon = scheme.on_surface;
        color_specs.disabled.trailing_icon.setAlphaF(0.38);
        color_specs.disabled.supporting_text = scheme.on_surface;
        color_specs.disabled.supporting_text.setAlphaF(0.38);
        color_specs.disabled.input_text = scheme.on_surface;
        color_specs.disabled.input_text.setAlphaF(0.38);
        color_specs.disabled.active_indicator = scheme.on_surface;
        color_specs.disabled.active_indicator.setAlphaF(0.38);
        color_specs.disabled.outline = scheme.outline;
        color_specs.disabled.outline.setAlphaF(0.38);

        color_specs.focused.container        = scheme.surface_container_highest;
        color_specs.focused.label_text       = scheme.primary;
        color_specs.focused.leading_icon     = scheme.on_surface_variant;
        color_specs.focused.trailing_icon    = scheme.on_surface_variant;
        color_specs.focused.input_text       = scheme.on_surface;
        color_specs.focused.supporting_text  = scheme.on_surface_variant;
        color_specs.focused.active_indicator = scheme.primary;
        color_specs.focused.outline          = scheme.primary;

        color_specs.error.container        = scheme.surface_container_highest;
        color_specs.error.active_indicator = scheme.error;
        color_specs.error.label_text       = scheme.error;
        color_specs.error.input_text       = scheme.on_surface;
        color_specs.error.supporting_text  = scheme.error;
        color_specs.error.leading_icon     = scheme.on_surface_variant;
        color_specs.error.trailing_icon    = scheme.error;
        color_specs.error.caret            = scheme.error;
        color_specs.error.outline          = scheme.error;

        color_specs.state_layer = scheme.on_surface;
        color_specs.state_layer.setAlphaF(0.08);
        color_specs.selection_container = scheme.primary;
        color_specs.selection_container.setAlphaF(0.38);

        const auto& color = get_color_tokens();
        sync_basic_text_style(              //
            color.input_text,               //
            Qt::transparent,                //
            color.input_text,               //
            color_specs.selection_container //
        );
    }

    auto load_theme_manager(ThemeManager& manager) -> void {
        manager.append_handler(&self, [this](const ThemeManager& manager) { //
            set_color_scheme(manager.color_scheme());
        });
    }

    auto set_label_text(const QString& text) -> void { label_text = text; }

    auto set_leading_icon(const QString& code, const QString& font) -> void {
        leading_icon_code = code;
        leading_font_name = font;

        is_update_component_status = false;
        use_leading_icon           = true;
    }

    auto set_measurements(const Measurements& measurements) noexcept -> void {
        this->measurements = measurements;
        self.setFixedHeight(measurements.container_height + measurements.standard_font_height);

        is_update_component_status = false;
    }

    auto paint_filled(QPaintEvent*) -> void {
        const auto rect  = self.rect();
        const auto color = get_color_tokens();

        constexpr auto container_radius = 5;

        update_component_status(FieldType::FILLED);

        auto painter = QPainter { &self };
        // Container
        {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color.container, Qt::transparent, 0, rect, container_radius,
                    container_radius, 0, 0);
        }
        // Active Indicator
        {
            const auto p0 = rect.bottomLeft();
            const auto p1 = rect.bottomRight();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.active_indicator, filled_line_width() });
            painter.drawLine(p0, p1);
        }
        // Leading icon

        if (!leading_icon.isNull()) {
            //
        } else if (!leading_icon_code.isEmpty()) {

            const auto rect = QRectF {
                1.0 * measurements.row_padding_with_icons,
                0.5 * (measurements.container_height - measurements.icon_rect_size),
                1.0 * measurements.icon_rect_size,
                1.0 * measurements.icon_rect_size,
            };

            painter.save();

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.leading_icon });
            painter.setFont(leading_icon_font);
            painter.drawText(rect, leading_icon_code, { Qt::AlignCenter });

            painter.restore();
        }
        // Label Text
        if (!label_text.isEmpty()) {

            const auto position = self.text().isEmpty() ? *label_position : 1.;
            const auto margins  = self.textMargins();

            const auto center_label_padding =
                0.5 * (measurements.container_height - measurements.icon_rect_size);
            const auto rect_center = QRectF {
                QPointF(margins.left(), center_label_padding),
                QPointF(self.width() - margins.right(), self.height() - center_label_padding),
            };
            const auto rect_top = QRectF {
                QPointF(margins.left(), measurements.col_padding),
                QPointF(self.width() - margins.right(), margins.top()),
            };

            const auto rect = animate::interpolate(rect_center, rect_top, position);

            const auto scale = 1. - position * 0.25;

            const auto anchor = QPointF { rect.left(), rect.center().y() };

            painter.save();

            painter.translate(anchor);
            painter.scale(scale, scale);
            painter.translate(-anchor);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.label_text });
            painter.setFont(standard_text_font);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawText(rect, label_text, { Qt::AlignVCenter | Qt::AlignLeading });

            painter.restore();
        }
        // Hovered State Layer
        if (is_hovered) {
            util::PainterHelper { painter }
                .set_render_hint(QPainter::Antialiasing)
                .rounded_rectangle(color_specs.state_layer, Qt::transparent, 0, rect,
                    container_radius, container_radius, 0, 0);
        }
    }

    auto paint_outlined(QPaintEvent*) -> void {
        const auto& measurements = this->measurements;
        const auto& color_tokens = get_color_tokens();

        update_component_status(FieldType::OUTLINED);
        {
            using namespace painter;
            using namespace painter::common::pro;
            auto painter = qt::painter { &self };

            /// Cache Calculate
            const auto container_width  = self.width();
            const auto container_height = measurements.container_height;

            const auto input_leading_padding  = use_leading_icon
                 ? measurements.row_padding_with_icons + measurements.icon_rect_size
                    + measurements.padding_icons_text
                 : measurements.row_padding_without_icons;
            const auto input_trailing_padding = use_trailing_icon
                ? measurements.row_padding_with_icons + measurements.icon_rect_size
                    + measurements.padding_icons_text
                : measurements.row_padding_without_icons;
            const auto input_vertical_padding =
                0.5 * (measurements.container_height - measurements.icon_rect_size);

            /// Container
            const auto container_size      = qt::size(self.width(), container_height);
            const auto container_thickness = is_focused ? 2. : is_hovered ? 1.5 : 1.;

            /// Leading Icon
            const auto leading_box_size = use_leading_icon
                ? qt::size(measurements.icon_rect_size, container_height)
                : qt::size(0, 0);

            /// Label Text
            const auto position   = self.text().isEmpty() ? *label_position : 1.;
            const auto text_scale = animate::interpolate(1., 0.75, position);

            auto text_option = qt::text_option {};
            text_option.setWrapMode(QTextOption::NoWrap);
            text_option.setAlignment(Qt::AlignLeading | Qt::AlignVCenter);

            const auto text_width = measure_text(standard_text_font, label_text, text_option);

            auto label_origin = qt::point {};
            auto label_size   = qt::size {};
            {
                const auto begin_y = input_vertical_padding;
                const auto final_y = -0.5 * measurements.standard_font_height;

                const auto top_padding  = use_leading_icon ? measurements.row_padding_with_icons
                                                           : measurements.row_padding_without_icons;
                const auto begin_origin = qt::point(input_leading_padding, begin_y);
                const auto final_origin = qt::point(top_padding, final_y);

                const auto begin_size = qt::size(text_width, measurements.icon_rect_size);
                const auto final_size =
                    qt::size(text_scale * text_width, measurements.standard_font_height);

                label_origin = animate::interpolate(begin_origin, final_origin, position);
                label_size   = animate::interpolate(begin_size, final_size, position);
            }
            const auto label_background_size = qt::size {
                label_size.width() + 2 * measurements.row_padding_populated_label_text,
                label_size.height(),
            };

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
                    Paint::Box {
                        BoxImpl { leading_box_size, Qt::AlignCenter,
                            { static_cast<qreal>(measurements.row_padding_with_icons), 0 } },
                        Paint::Text {
                            TextOption { Qt::AlignCenter },
                            Size { leading_box_size },
                            Font { leading_icon_font },
                            Text { leading_icon_code },
                            Color { color_tokens.leading_icon },
                        },
                    },
                },
            }(painter);
        }
    }

    auto enter_event(qt::EnterEvent*) -> void {
        is_hovered = true;
        self.update();
    }

    auto leave_event(QEvent*) -> void {
        is_hovered = false;
        self.update();
    }

    auto focus_in(QFocusEvent*) -> void {
        is_focused = true;
        update_label_position();
    }

    auto focus_out(QFocusEvent*) -> void {
        is_focused = false;
        update_label_position();
    }

private:
    enum class FieldType { FILLED, OUTLINED };

    auto update_component_status(FieldType type) -> void {
        if (is_update_component_status) return;

        const auto padding_with_icon = measurements.row_padding_with_icons
            + measurements.icon_rect_size + measurements.padding_icons_text;
        const auto padding_without_icon = measurements.row_padding_without_icons;

        const auto left_padding = use_leading_icon ? padding_with_icon : padding_without_icon;
        const auto tail_padding = use_trailing_icon ? padding_with_icon : padding_without_icon;

        switch (type) {
        case FieldType::FILLED: {
            const auto top_padding = measurements.col_padding + measurements.label_rect_size;
            const auto bot_padding = measurements.col_padding;
            self.setTextMargins(left_padding, top_padding, tail_padding, bot_padding);
            break;
        }
        case FieldType::OUTLINED: {
            const auto input_padding =
                0.5 * (measurements.container_height - measurements.input_rect_size);
            self.setTextMargins(left_padding, input_padding, tail_padding, input_padding);
            break;
        }
        }

        auto font = self.font();
        font.setPixelSize(measurements.standard_font_height);
        self.setFont(font);

        standard_text_font = self.font();
        standard_text_font.setPixelSize(measurements.standard_font_height);

        leading_icon_font = QFont { leading_font_name };
        leading_icon_font.setPointSizeF(
            0.5 * (measurements.standard_font_height + measurements.icon_rect_size));

        is_update_component_status = true;
    }

    auto update_label_position() -> void { label_position->transition_to(is_focused ? 1.0 : 0.0); }

    auto sync_basic_text_style(const QColor& text, const QColor& background,
        const QColor& selection_text, const QColor& selection_background) -> void {

        constexpr auto to_rgba = [](const QColor& color) {
            return QStringLiteral("rgba(%1, %2, %3, %4)")
                .arg(color.red())
                .arg(color.green())
                .arg(color.blue())
                .arg(color.alpha());
        };

        constexpr auto kQLineEditStyle = R"(
            QLineEdit {
                border: none;
                color: %1;
                background-color: %2;
                selection-color: %3;
                selection-background-color: %4;
            }
        )";

        const auto qss = QString { kQLineEditStyle };
        self.setStyleSheet(qss.arg(to_rgba(text))
                .arg(to_rgba(background))
                .arg(to_rgba(selection_text))
                .arg(to_rgba(selection_background)));
    }

    auto get_color_tokens() const -> ColorSpecs::Tokens const& {
        return is_disable ? color_specs.disabled
            : is_error    ? color_specs.error
            : is_focused  ? color_specs.focused
                          : color_specs.enabled;
    }

    auto filled_line_width() const -> double {
        constexpr auto normal_width = 1;
        constexpr auto active_width = 3;
        return (is_focused && !is_disable) ? active_width : normal_width;
    }

    constexpr auto measure_text(const QFont& font, const QString& text, const QTextOption& options)
        -> double {
        const auto fm   = QFontMetricsF(font);
        const auto size = fm.size(Qt::TextSingleLine, text);
        return size.width();
    }

private:
    Measurements measurements;
    ColorSpecs color_specs;

    bool is_disable                 = false;
    bool is_hovered                 = false;
    bool is_focused                 = false;
    bool is_error                   = false;
    bool is_update_component_status = false;

    QString label_text;
    QString hint_text;
    QString supporting_text;

    bool use_leading_icon  = false;
    bool use_trailing_icon = false;

    QIcon leading_icon;
    QString leading_icon_code;
    QString leading_font_name;

    QIcon trailing_icon;
    QString trailing_code;
    QString trailing_font;

    QFont leading_icon_font;
    QFont standard_text_font;

    Animatable animatable;
    std::unique_ptr<TransitionValue<PidState<double>>> label_position;

    BasicTextField& self;
};
