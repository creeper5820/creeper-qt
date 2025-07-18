#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/animation/motion-system.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "text-fields.hh"
#include <qpainter.h>
#include <qpainterpath.h>

using namespace creeper;
using namespace creeper::text_field::internal;

using util::animation::AnimationCore;

struct BasicTextField::Impl {
public:
    explicit Impl(BasicTextField& self) noexcept
        : self { self }
        , animation_core { [&self] { self.update(); }, AnimationCore::kHz } {
        self.setTextMargins(text_margin_l, text_margin_u, text_margin_r, text_margin_d);
        self.setAlignment(Qt::AlignVCenter);
    }

    /// @note https://m3.material.io/components/text-fields/specs
    auto set_color_scheme(const ColorScheme& scheme) -> void {

        self.color.enabled.container        = scheme.surface_container_highest;
        self.color.enabled.label_text       = scheme.on_surface_variant;
        self.color.enabled.leading_icon     = scheme.on_surface_variant;
        self.color.enabled.trailing_icon    = scheme.on_surface_variant;
        self.color.enabled.active_indicator = scheme.on_surface_variant;
        self.color.enabled.supporting_text  = scheme.on_surface_variant;
        self.color.enabled.input_text       = scheme.on_surface;
        self.color.enabled.caret            = scheme.primary;

        self.color.disabled.container = scheme.on_surface;
        self.color.disabled.container.setAlphaF(0.04);
        self.color.disabled.label_text = scheme.on_surface;
        self.color.disabled.label_text.setAlphaF(0.38);
        self.color.disabled.leading_icon = scheme.on_surface;
        self.color.disabled.leading_icon.setAlphaF(0.38);
        self.color.disabled.trailing_icon = scheme.on_surface;
        self.color.disabled.trailing_icon.setAlphaF(0.38);
        self.color.disabled.supporting_text = scheme.on_surface;
        self.color.disabled.supporting_text.setAlphaF(0.38);
        self.color.disabled.input_text = scheme.on_surface;
        self.color.disabled.input_text.setAlphaF(0.38);
        self.color.disabled.active_indicator = scheme.on_surface;
        self.color.disabled.active_indicator.setAlphaF(0.38);

        self.color.focused.container        = scheme.surface_container_highest;
        self.color.focused.label_text       = scheme.primary;
        self.color.focused.leading_icon     = scheme.on_surface_variant;
        self.color.focused.trailing_icon    = scheme.on_surface_variant;
        self.color.focused.input_text       = scheme.on_surface;
        self.color.focused.supporting_text  = scheme.on_surface_variant;
        self.color.focused.active_indicator = scheme.primary;

        self.color.error.container        = scheme.surface_container_highest;
        self.color.error.active_indicator = scheme.error;
        self.color.error.label_text       = scheme.error;
        self.color.error.input_text       = scheme.on_surface;
        self.color.error.supporting_text  = scheme.error;
        self.color.error.leading_icon     = scheme.on_surface_variant;
        self.color.error.trailing_icon    = scheme.error;
        self.color.error.caret            = scheme.error;

        self.color.state_layer = scheme.on_surface;
        self.color.state_layer.setAlphaF(0.08);
        self.color.selection_container = scheme.primary;
        self.color.selection_container.setAlphaF(0.38);

        const auto color = color_list();
        sync_basic_text_style(             //
            color.input_text,              //
            Qt::transparent,               //
            color.input_text,              //
            self.color.selection_container //
        );
        update_ui_state();
    }

    auto load_theme_manager(ThemeManager& manager) -> void {
        manager.append_handler(&self, [this](const ThemeManager& manager) { //
            set_color_scheme(manager.color_scheme());
        });
    }

    auto set_label_text(const QString& text) -> void { label_text = text; }

    auto set_leading_icon(const QString& code, const QString& font) -> void {
        leading_code = code;
        leading_font = font;

        has_adjust_margin = false;
        use_leading_icon  = true;
    }

    auto paint_filled(QPaintEvent*) -> void {
        const auto rect  = self.rect();
        const auto color = color_list();

        if (use_leading_icon && !has_adjust_margin) {
            has_adjust_margin  = true;
            const auto margins = self.textMargins();
            self.setTextMargins(margins.left() + self.height() * 0.6, margins.top(),
                margins.right(), margins.bottom());
        }

        auto container_path = QPainterPath {};
        {
            const auto p0 = rect.bottomLeft();
            const auto p1 = rect.bottomRight();
            const auto p2 = rect.topRight();
            const auto p3 = rect.topLeft();
            const auto r  = int { 5 };

            container_path.moveTo(p0);
            container_path.lineTo(p1);
            container_path.lineTo(p2.x(), p2.y() + r);
            container_path.arcTo(p2.x() - 2 * r, p2.y(), 2 * r, 2 * r, 0, 90);
            container_path.lineTo(p3.x() + r, p3.y());
            container_path.arcTo(p3.x(), p3.y(), 2 * r, 2 * r, 90, 90);
            container_path.lineTo(p0);
        }

        auto painter = QPainter { &self };
        // Container
        {
            painter.setBrush(color.container);
            painter.setPen(Qt::NoPen);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPath(container_path);
        }
        // Active Indicator
        {
            const auto p0 = rect.bottomLeft();
            const auto p1 = rect.bottomRight();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.active_indicator, line_width() });
            painter.drawLine(p0, p1);
        }
        // Leading icon
        if (!leading_icon.isNull()) {
            //
        } else if (!leading_code.isEmpty()) {
            const auto height = self.height() / 3;
            const auto rect   = QRectF {
                QPointF(text_margin_l, 0),
                QPointF(self.width(), self.height()),
            };

            auto font = QFont { leading_font };
            font.setPointSizeF(height);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.leading_icon });
            painter.setFont(font);
            painter.drawText(rect, leading_code, { Qt::AlignVCenter });
        }
        // Label Text
        if (!label_text.isEmpty()) {
            const auto position = self.text().isEmpty() ? *label_position : 1.;
            const auto margins  = self.textMargins();
            const auto rect_1   = QRectF {
                QPointF(margins.left(), 0),
                QPointF(self.width() - margins.right(), self.height()),
            };
            const auto rect_2 = QRectF {
                QPointF(margins.left(), 0),
                QPointF(self.width() - margins.right(), margins.top()),
            };
            const auto rect = interpolate(rect_1, rect_2, position);

            auto font = self.font();
            auto size = font.pointSizeF() * (1. - 0.3 * position);
            font.setPointSizeF(size);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color.label_text });
            painter.setFont(font);
            painter.drawText(rect, label_text, { Qt::AlignVCenter });
        }
        // Hovered State Layer
        if (is_hovered) {
            painter.setBrush(self.color.state_layer);
            painter.setPen(Qt::NoPen);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPath(container_path);
        }
    }

    auto paint_outlined(QPaintEvent*) -> void { }

    auto enter_event(QEvent*) -> void {
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
    auto update_ui_state() -> void { }

    auto update_label_position() -> void {
        using Tracker = util::animation::FinitePidTracker<double>;

        *label_stop_token.value() = true;
        label_stop_token.emplace(std::make_shared<bool>(false));

        constexpr double kp = 15.0, ki = 0.0, kd = 0.0;
        animation_core.append(std::make_unique<Tracker>(label_position, is_focused ? 1.0 : 0.0,
            *label_stop_token, kp, ki, kd, AnimationCore::kHz, 1e-3));
    }

    auto sync_basic_text_style(const QColor& text, const QColor& container,
        const QColor& selection_text, const QColor& selection_container) -> void {

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
                .arg(to_rgba(container))
                .arg(to_rgba(selection_text))
                .arg(to_rgba(selection_container)));
    }

    auto color_list() const -> ColorList const& {
        return is_disable ? self.color.disabled
            : is_error    ? self.color.error
            : is_focused  ? self.color.focused
                          : self.color.enabled;
    }
    auto line_width() const -> double { return (is_focused && !is_disable) ? 3 : 1; }

    static constexpr auto interpolate(const QRectF& r1, const QRectF& r2, qreal position)
        -> QRectF {
        position = qBound(0.0, position, 1.0);
        auto _1  = r1.left() + (r2.left() - r1.left()) * position;
        auto _2  = r1.top() + (r2.top() - r1.top()) * position;
        auto _3  = r1.width() + (r2.width() - r1.width()) * position;
        auto _4  = r1.height() + (r2.height() - r1.height()) * position;
        return { _1, _2, _3, _4 };
    }

private:
    BasicTextField& self;
    AnimationCore animation_core;

    double text_margin_l = 10;
    double text_margin_r = 10;
    double text_margin_u = 20;
    double text_margin_d = 10;

    bool is_disable = false;
    bool is_hovered = false;
    bool is_focused = false;
    bool is_error   = false;

    QString label_text;
    QString hint_text;
    QString supporting_text;

    bool use_leading_icon  = false;
    bool has_adjust_margin = false;
    QIcon leading_icon;
    QString leading_code;
    QString leading_font;

    QIcon trailing_icon;
    QString trailing_code;
    QString trailing_font;

    std::shared_ptr<double> label_position = std::make_shared<double>(0);
    std::optional<std::shared_ptr<bool>> label_stop_token { std::make_shared<bool>() };
};
