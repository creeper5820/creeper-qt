#include "snackbar.hh"

#include "creeper-qt/details/token/sys/font.hh"
#include "creeper-qt/details/token/sys/shadow.hh"
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/tween.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/co/meta/continuation.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/shape.hh"
#include "creeper-qt/utility/wrapper/mutable-value.hh"

#include <qevent.h>
#include <qfontmetrics.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpointer.h>
#include <qtextlayout.h>
#include <qtimer.h>

#include <algorithm>
#include <cmath>
#include <memory>

using namespace creeper;
using Defaults = Snackbar::Defaults;

namespace {

enum class Button { NONE, ACTION, DISMISS };

/// @brief 固定行高的多行正文，行内按 Compose 的 LineHeightStyle.Alignment.Center 居中
struct Paragraph {
    QTextLayout layout;
    int width       = -1;
    int lines       = 1;
    int line_height = 0;
    int baseline    = 0;
    bool has_layout = false;

    auto set(const QString& text, const creeper::Font& font) -> void {
        layout.setText(text);
        layout.setFont(font);

        auto option = QTextOption { };
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        layout.setTextOption(option);

        const auto metrics = QFontMetricsF { font };
        line_height        = static_cast<int>(std::round(font.line_height));
        baseline           = static_cast<int>(std::round(
            (font.line_height - (metrics.ascent() + metrics.descent())) / 2 + metrics.ascent()));

        width      = -1;
        has_layout = false;
    }

    auto relayout(int max_width) -> void {
        if (has_layout && width == max_width) return;

        const auto metrics   = QFontMetricsF { layout.font() };
        const auto glyph_top = (line_height - (metrics.ascent() + metrics.descent())) / 2;

        lines = 0;
        layout.beginLayout();
        for (auto line = layout.createLine(); line.isValid(); line = layout.createLine()) {
            line.setLineWidth(std::max(max_width, 0));
            line.setPosition(QPointF { 0, lines * line_height + glyph_top });
            ++lines;
        }
        layout.endLayout();
        lines = std::max(lines, 1);

        width      = max_width;
        has_layout = true;
    }

    auto height() const -> int { return lines * line_height; }
    auto first_baseline() const -> int { return baseline; }
    auto last_baseline() const -> int { return (lines - 1) * line_height + baseline; }
};

struct Geometry {
    QRect surface;
    QPoint text;
    QRect action;
    QRect dismiss;
    int height = 0;
};

struct Graphics {
    /// @brief 绘制 Paragraph 的节点
    struct SupportingText {
        qt::point origin { };
        qt::size size { };
        const QTextLayout* layout = nullptr;
        qt::color color           = Qt::black;

        auto operator()(qt::painter& painter) const noexcept -> void {
            painter.save();
            painter.setPen(color);
            layout->draw(&painter, origin);
            painter.restore();
        }
    };

    /// @brief Icons.Filled.Close，24×24 视口
    struct CloseIcon {
        qt::point origin { };
        qt::size size { };
        qt::color color = Qt::black;

        auto operator()(qt::painter& painter) const noexcept -> void {
            static const auto path = [] {
                auto path = QPainterPath { };
                path.moveTo(19.00, 6.41);
                path.lineTo(17.59, 5.00);
                path.lineTo(12.00, 10.59);
                path.lineTo(6.41, 5.00);
                path.lineTo(5.00, 6.41);
                path.lineTo(10.59, 12.00);
                path.lineTo(5.00, 17.59);
                path.lineTo(6.41, 19.00);
                path.lineTo(12.00, 13.41);
                path.lineTo(17.59, 19.00);
                path.lineTo(19.00, 17.59);
                path.lineTo(13.41, 12.00);
                path.closeSubpath();
                return path;
            }();

            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.translate(origin);
            painter.scale(size.width() / 24.0, size.height() / 24.0);
            painter.setPen(Qt::NoPen);
            painter.setBrush(color);
            painter.drawPath(path);
            painter.restore();
        }
    };

    static auto with_alpha(QColor color, double alpha) -> QColor {
        color.setAlphaF(std::clamp(alpha, 0.0, 1.0) * color.alphaF());
        return color;
    }

    static auto centered(const QRect& box, const QSize& size) -> qt::rect {
        return qt::rect {
            box.x() + (box.width() - size.width()) / 2.0,
            box.y() + (box.height() - size.height()) / 2.0,
            static_cast<qt::real>(size.width()),
            static_cast<qt::real>(size.height()),
        };
    }
};

}

struct Snackbar::Impl {
    using Tween = TransitionValue<TweenState<double>>;

    struct Indication {
        std::unique_ptr<Tween> hover;
        std::unique_ptr<Tween> focus;
        std::unique_ptr<Tween> press;
    };

    Snackbar& self;
    Animatable animatable;

    QString text;
    QString action_label;
    bool with_dismiss_action = false;
    bool action_on_new_line  = false;

    Measurements measurements;
    Colors colors;

    creeper::Font supporting_font;
    creeper::Font action_font;
    Paragraph paragraph;

    QPointer<Elevation> elevation;

    Indication action_indication;
    Indication dismiss_indication;

    Button hovered = Button::NONE;
    Button pressed = Button::NONE;
    Button focused = Button::NONE;

    bool focus_visible = false;

    explicit Impl(Snackbar& self) noexcept
        : self { self }
        , animatable { self } {
        for (auto* indication : { &action_indication, &dismiss_indication }) {
            indication->hover = make_transition(animatable, Tween::make_state());
            indication->focus = make_transition(animatable, Tween::make_state());
            indication->press = make_transition(animatable, Tween::make_state());
        }

        elevation = new Elevation { Defaults::kContainerElevation };
        self.setGraphicsEffect(elevation);

        auto policy = QSizePolicy { QSizePolicy::Preferred, QSizePolicy::Preferred };
        policy.setHeightForWidth(true);
        self.setSizePolicy(policy);
        self.setMouseTracking(true);

        update_fonts();
        update_focus_policy();
    }

    // 尺寸

    auto action_visual_size() const -> QSize {
        const auto metrics = QFontMetrics { action_font };
        const auto label   = metrics.horizontalAdvance(action_label);
        const auto padding = measurements.action_horizontal_padding.px();
        const auto line    = static_cast<int>(std::round(action_font.line_height));
        return QSize {
            std::max(measurements.action_min_width.px(), label + 2 * padding),
            std::max(measurements.action_min_height.px(),
                line + 2 * measurements.action_vertical_padding.px()),
        };
    }

    auto action_touch_size() const -> QSize {
        if (action_label.isEmpty()) return QSize { 0, 0 };
        const auto visual = action_visual_size();
        const auto target = measurements.touch_target_size.px();
        return QSize { std::max(visual.width(), target), std::max(visual.height(), target) };
    }

    /// @brief action 文字基线相对触控盒顶部的偏移
    auto action_baseline() const -> int {
        const auto touch   = action_touch_size();
        const auto visual  = action_visual_size();
        const auto line    = static_cast<int>(std::round(action_font.line_height));
        const auto metrics = QFontMetricsF { action_font };
        const auto inner   = static_cast<int>(
            std::round((action_font.line_height - (metrics.ascent() + metrics.descent())) / 2
                + metrics.ascent()));
        return (touch.height() - visual.height()) / 2 + (visual.height() - line) / 2 + inner;
    }

    auto dismiss_visual_size() const -> QSize {
        const auto size = measurements.dismiss_container_size.px();
        return QSize { size, size };
    }

    auto dismiss_touch_size() const -> QSize {
        if (!with_dismiss_action) return QSize { 0, 0 };
        const auto visual = dismiss_visual_size();
        const auto target = measurements.touch_target_size.px();
        return QSize { std::max(visual.width(), target), std::max(visual.height(), target) };
    }

    // 布局：照搬 LegacyOneRowSnackbar / LegacyNewLineButtonSnackbar

    auto compute(int widget_width) -> Geometry {
        const auto margin    = measurements.snackbar_padding.px();
        const auto available = std::max(widget_width - 2 * margin, 0);
        const auto max_width = measurements.container_max_width.px();

        const auto action  = action_touch_size();
        const auto dismiss = dismiss_touch_size();

        auto geometry = Geometry { };

        if (action_on_new_line && !action_label.isEmpty()) {
            const auto surface_width = std::min(available, max_width);
            const auto start         = measurements.horizontal_spacing.px();
            const auto text_end      = measurements.horizontal_spacing_button_side.px();

            paragraph.relayout(surface_width - start - text_end);

            const auto top =
                std::max(0, measurements.height_to_first_line.px() - paragraph.first_baseline());
            const auto bottom          = std::max(0,
                measurements.long_button_vertical_offset.px()
                    - (paragraph.height() - paragraph.last_baseline()));
            const auto text_box_height = top + paragraph.height() + bottom;

            const auto buttons_end = surface_width
                - (with_dismiss_action ? 0 : measurements.horizontal_spacing_button_side.px());
            const auto row_height = std::max(action.height(), dismiss.height());

            geometry.dismiss =
                QRect { QPoint { buttons_end - dismiss.width(), text_box_height }, dismiss };
            geometry.action = QRect {
                QPoint { geometry.dismiss.x() - action.width(), text_box_height },
                action,
            };
            geometry.text    = QPoint { start, top };
            geometry.surface = QRect { 0, 0, surface_width,
                text_box_height + row_height + measurements.separate_button_extra_y.px() };
        } else {
            const auto start = measurements.horizontal_spacing.px();
            const auto end =
                with_dismiss_action ? 0 : measurements.horizontal_spacing_button_side.px();
            const auto content_width = std::clamp(available - start - end, 0, max_width);

            const auto extra_spacing =
                with_dismiss_action ? 0 : measurements.text_end_extra_spacing.px();
            const auto text_max_width =
                std::max(content_width - action.width() - dismiss.width() - extra_spacing, 0);
            paragraph.relayout(text_max_width);

            const auto vertical_padding = measurements.vertical_padding.px();
            const auto text_height      = paragraph.height() + 2 * vertical_padding;
            const auto first_baseline   = vertical_padding + paragraph.first_baseline();

            auto container_height = 0;
            auto text_y           = 0;
            auto action_y         = 0;
            if (paragraph.lines == 1) {
                container_height = std::max({ measurements.single_line_container_height.px(),
                  action.height(), dismiss.height() });
                text_y           = (container_height - text_height) / 2;
                action_y         = text_y + first_baseline - action_baseline();
            } else {
                text_y = measurements.height_to_first_line.px() - first_baseline;
                container_height =
                    std::max(measurements.two_lines_container_height.px(), text_y + text_height);
                action_y = (container_height - action.height()) / 2;
            }
            const auto dismiss_y = (container_height - dismiss.height()) / 2;

            const auto dismiss_x = start + content_width - dismiss.width();
            geometry.dismiss     = QRect { QPoint { dismiss_x, dismiss_y }, dismiss };
            geometry.action  = QRect { QPoint { dismiss_x - action.width(), action_y }, action };
            geometry.text    = QPoint { start, text_y + vertical_padding };
            geometry.surface = QRect { 0, 0, start + content_width + end, container_height };
        }

        const auto offset = QPoint { margin + (available - geometry.surface.width()) / 2, margin };
        geometry.surface.translate(offset);
        geometry.action.translate(offset);
        geometry.dismiss.translate(offset);
        geometry.text += offset;
        geometry.height = geometry.surface.height() + 2 * margin;

        return geometry;
    }

    auto preferred_width() const -> int {
        const auto margin    = measurements.snackbar_padding.px();
        const auto max_width = measurements.container_max_width.px();
        if (action_on_new_line && !action_label.isEmpty()) return 2 * margin + max_width;

        const auto end = with_dismiss_action ? 0 : measurements.horizontal_spacing_button_side.px();
        return 2 * margin + measurements.horizontal_spacing.px() + max_width + end;
    }

    auto minimum_width() const -> int {
        const auto margin = measurements.snackbar_padding.px();
        const auto end = with_dismiss_action ? 0 : measurements.horizontal_spacing_button_side.px();
        const auto extra = with_dismiss_action ? 0 : measurements.text_end_extra_spacing.px();
        return 2 * margin + measurements.horizontal_spacing.px() + action_touch_size().width()
            + dismiss_touch_size().width() + extra + end;
    }

    auto size_hint() -> QSize {
        const auto width = preferred_width();
        return QSize { width, compute(width).height };
    }

    /// @note 高度交给 heightForWidth，这里只给最宽时的高度，避免布局按最窄宽度累计出过高的最小高度
    auto minimum_size_hint() -> QSize {
        return QSize { minimum_width(), compute(preferred_width()).height };
    }

    auto height_for_width(int width) -> int { return compute(width).height; }

    // 状态

    auto update_fonts() -> void {
        supporting_font = creeper::Font { Defaults::kSupportingTextFont, self.font() };
        action_font     = creeper::Font { Defaults::kActionLabelTextFont, self.font() };
        paragraph.set(text, supporting_font);
        relayout();
    }

    auto update_focus_policy() -> void {
        const auto has_button = !action_label.isEmpty() || with_dismiss_action;
        self.setFocusPolicy(has_button ? Qt::StrongFocus : Qt::NoFocus);
        if (!has_button) set_focused(Button::NONE);
        else if (focused == Button::ACTION && action_label.isEmpty()) set_focused(first_button());
        else if (focused == Button::DISMISS && !with_dismiss_action) set_focused(first_button());
    }

    auto relayout() -> void {
        self.updateGeometry();
        self.update();
    }

    auto indication(Button button) -> Indication* {
        switch (button) {
        case Button::ACTION:
            return &action_indication;
        case Button::DISMISS:
            return &dismiss_indication;
        case Button::NONE:
            break;
        }
        return nullptr;
    }

    auto first_button() const -> Button {
        if (!action_label.isEmpty()) return Button::ACTION;
        if (with_dismiss_action) return Button::DISMISS;
        return Button::NONE;
    }

    auto last_button() const -> Button {
        if (with_dismiss_action) return Button::DISMISS;
        if (!action_label.isEmpty()) return Button::ACTION;
        return Button::NONE;
    }

    auto hit_test(const QPoint& position) -> Button {
        const auto geometry = compute(self.width());
        if (!action_label.isEmpty() && geometry.action.contains(position)) return Button::ACTION;
        if (with_dismiss_action && geometry.dismiss.contains(position)) return Button::DISMISS;
        return Button::NONE;
    }

    auto set_hovered(Button button) -> void {
        if (hovered == button) return;
        const auto fade = [](Indication& target, double to) {
            target.hover->get_state().config = {
                .duration = Defaults::kStateLayerDefaultDuration,
                .easing   = Defaults::kEasingLinearCubicBezier,
            };
            target.hover->transition_to(to);
        };
        if (auto* previous = indication(hovered)) fade(*previous, 0.0);
        hovered = button;
        if (auto* next = indication(hovered)) fade(*next, 1.0);
        self.update();
    }

    auto set_focused(Button button) -> void {
        if (focused == button) return;
        if (auto* previous = indication(focused)) {
            previous->focus->get_state().config = {
                .duration = Defaults::kStateLayerDefaultDuration,
                .easing   = Defaults::kEasingLinearCubicBezier,
            };
            previous->focus->transition_to(0.0);
        }
        focused = button;
        if (auto* next = indication(focused); next && focus_visible) {
            next->focus->get_state().config = {
                .duration = Defaults::kStateLayerFocusInDuration,
                .easing   = Defaults::kEasingLinearCubicBezier,
            };
            next->focus->transition_to(1.0);
        }
        self.update();
    }

    auto press(Button button) -> void {
        pressed = button;
        if (auto* target = indication(button)) {
            target->press->get_state().config = {
                .duration = Defaults::kFadeInDuration,
                .easing   = Defaults::kEasingLinearCubicBezier,
            };
            target->press->transition_to(Defaults::kPressedStateLayerOpacity);
        }
        self.update();
    }

    auto release() -> void {
        if (auto* target = indication(pressed)) {
            target->press->get_state().config = {
                .duration = Defaults::kFadeOutDuration,
                .easing   = Defaults::kEasingLinearCubicBezier,
            };
            target->press->transition_to(0.0);
        }
        pressed = Button::NONE;
        self.update();
    }

    auto trigger(Button button) -> void {
        if (button == Button::ACTION) Q_EMIT self.actionPerformed();
        if (button == Button::DISMISS) Q_EMIT self.dismissed();
    }

    // setter

    auto load_color_scheme(const ColorScheme& scheme) -> void {
        Defaults::mapColors(scheme, colors);
        if (elevation) elevation->setShadowColor(scheme.shadow);
        self.update();
    }

    auto bind_theme_manager(ThemeManager& manager) -> void {
        manager.appendHandler(
            &self, [this](const ThemeManager& m) { load_color_scheme(m.colorScheme()); });
    }

    auto set_measurements(const Measurements& value) -> void {
        measurements = value;
        relayout();
    }

    auto set_colors(const Colors& value) -> void {
        colors = value;
        self.update();
    }

    auto set_text(const QString& value) -> void {
        if (text == value) return;
        text = value;
        paragraph.set(text, supporting_font);
        relayout();
    }

    auto set_action_label(const QString& value) -> void {
        if (action_label == value) return;
        action_label = value;
        update_focus_policy();
        relayout();
    }

    auto set_with_dismiss_action(bool on) -> void {
        if (with_dismiss_action == on) return;
        with_dismiss_action = on;
        update_focus_policy();
        relayout();
    }

    auto set_action_on_new_line(bool on) -> void {
        if (action_on_new_line == on) return;
        action_on_new_line = on;
        relayout();
    }

    // 事件

    auto change_event(QEvent* event) -> void {
        if (event->type() == QEvent::FontChange) update_fonts();
    }

    auto leave_event() -> void { set_hovered(Button::NONE); }

    auto mouse_move_event(QMouseEvent* event) -> void {
        set_hovered(hit_test(event->position().toPoint()));
    }

    auto mouse_press_event(QMouseEvent* event) -> void {
        if (event->button() != Qt::LeftButton) return;
        const auto button = hit_test(event->position().toPoint());
        if (button == Button::NONE) return;
        set_hovered(button);
        press(button);
    }

    auto mouse_release_event(QMouseEvent* event) -> void {
        if (event->button() != Qt::LeftButton || pressed == Button::NONE) return;
        const auto button = pressed;
        release();
        if (hit_test(event->position().toPoint()) == button) trigger(button);
    }

    auto focus_in_event(QFocusEvent* event) -> void {
        const auto reason = event->reason();
        focus_visible     = reason == Qt::TabFocusReason || reason == Qt::BacktabFocusReason;

        focused = Button::NONE;
        set_focused(reason == Qt::BacktabFocusReason ? last_button() : first_button());
    }

    auto focus_out_event() -> void {
        set_focused(Button::NONE);
        focus_visible = false;
    }

    auto focus_next_prev_child(bool next) -> bool {
        if (next && focused == Button::ACTION && with_dismiss_action) {
            focus_visible = true;
            set_focused(Button::DISMISS);
            return true;
        }
        if (!next && focused == Button::DISMISS && !action_label.isEmpty()) {
            focus_visible = true;
            set_focused(Button::ACTION);
            return true;
        }
        return false;
    }

    auto key_press_event(QKeyEvent* event) -> bool {
        const auto key = event->key();
        if (key != Qt::Key_Space && key != Qt::Key_Return && key != Qt::Key_Enter) return false;
        if (focused == Button::NONE) return false;
        if (!event->isAutoRepeat()) press(focused);
        return true;
    }

    auto key_release_event(QKeyEvent* event) -> bool {
        const auto key = event->key();
        if (key != Qt::Key_Space && key != Qt::Key_Return && key != Qt::Key_Enter) return false;
        if (pressed == Button::NONE || event->isAutoRepeat()) return pressed != Button::NONE;
        const auto button = pressed;
        release();
        trigger(button);
        return true;
    }

    // 绘制

    auto state_layer_color(const QColor& content, const Indication& indication) const -> QColor {
        const auto alpha = std::max(Defaults::kHoverStateLayerOpacity * *indication.hover,
            Defaults::kFocusStateLayerOpacity * *indication.focus);
        return Graphics::with_alpha(content, alpha);
    }

    auto paint_event() -> void {
        const auto geometry = compute(self.width());

        const auto action_visual  = Graphics::centered(geometry.action, action_visual_size());
        const auto dismiss_visual = Graphics::centered(geometry.dismiss, dismiss_visual_size());
        const auto icon_size      = measurements.icon_size.value;
        const auto icon_rect      = Graphics::centered(
            geometry.dismiss, QSize { measurements.icon_size.px(), measurements.icon_size.px() });

        const auto has_action  = !action_label.isEmpty();
        const auto has_dismiss = with_dismiss_action;

        const auto transparent = QColor { Qt::transparent };

        const auto action_state =
            has_action ? state_layer_color(colors.action_color, action_indication) : transparent;
        const auto action_press  = has_action
            ? Graphics::with_alpha(colors.action_color, *action_indication.press)
            : transparent;
        const auto dismiss_state = has_dismiss
            ? state_layer_color(colors.dismiss_action_content_color, dismiss_indication)
            : transparent;
        const auto dismiss_press = has_dismiss
            ? Graphics::with_alpha(colors.dismiss_action_content_color, *dismiss_indication.press)
            : transparent;
        const auto icon_color =
            has_dismiss ? QColor { colors.dismiss_action_content_color } : transparent;

        auto label_option = QTextOption { };
        label_option.setAlignment(Qt::AlignCenter);

        using namespace painter;
        using namespace painter::common::pro;

        auto paint = Paint::Surface {
            SurfaceImpl { self.size() },
            Paint::RoundedRectangle {
              Origin { geometry.surface.topLeft() },
              Size { geometry.surface.size() },
              Fill { colors.color },
              Radiuses { measurements.container_shape.value },
            },
            Graphics::SupportingText {
              .origin = geometry.text,
              .size   = qt::size { static_cast<qt::real>(paragraph.width),
                static_cast<qt::real>(paragraph.height()) },
              .layout = &paragraph.layout,
              .color  = colors.content_color,
            },
            Paint::RoundedRectangle {
              Origin { action_visual.topLeft() },
              Size { action_visual.size() },
              Fill { action_state },
              Radiuses { action_visual.height() / 2 },
            },
            Paint::RoundedRectangle {
              Origin { action_visual.topLeft() },
              Size { action_visual.size() },
              Fill { action_press },
              Radiuses { action_visual.height() / 2 },
            },
            Paint::Text {
              Origin { action_visual.topLeft() },
              Size { action_visual.size() },
              painter::Text { has_action ? action_label : QString { } },
              painter::Font { action_font },
              painter::Color { colors.action_color },
              painter::TextOption { label_option },
            },
            Paint::RoundedRectangle {
              Origin { dismiss_visual.topLeft() },
              Size { dismiss_visual.size() },
              Fill { dismiss_state },
              Radiuses { dismiss_visual.height() / 2 },
            },
            Paint::RoundedRectangle {
              Origin { dismiss_visual.topLeft() },
              Size { dismiss_visual.size() },
              Fill { dismiss_press },
              Radiuses { dismiss_visual.height() / 2 },
            },
            Graphics::CloseIcon {
              .origin = icon_rect.topLeft(),
              .size   = qt::size { icon_size, icon_size },
              .color  = icon_color,
            },
        };

        auto painter = qt::painter { &self };
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        paint(painter);
    }
};

Snackbar::Snackbar()
    : pimpl(std::make_unique<Impl>(*this)) { }

Snackbar::~Snackbar() = default;

auto Snackbar::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->load_color_scheme(scheme);
}

auto Snackbar::bindThemeManager(ThemeManager& manager) -> void {
    pimpl->bind_theme_manager(manager);
}

auto Snackbar::setMeasurements(const Measurements& measurements) -> void {
    pimpl->set_measurements(measurements);
}

auto Snackbar::setColors(const Colors& colors) -> void { pimpl->set_colors(colors); }

auto Snackbar::setText(const QString& text) -> void { pimpl->set_text(text); }

auto Snackbar::text() const -> QString { return pimpl->text; }

auto Snackbar::setActionLabel(const QString& label) -> void { pimpl->set_action_label(label); }

auto Snackbar::actionLabel() const -> QString { return pimpl->action_label; }

auto Snackbar::setWithDismissAction(bool on) -> void { pimpl->set_with_dismiss_action(on); }

auto Snackbar::withDismissAction() const -> bool { return pimpl->with_dismiss_action; }

auto Snackbar::setActionOnNewLine(bool on) -> void { pimpl->set_action_on_new_line(on); }

auto Snackbar::actionOnNewLine() const -> bool { return pimpl->action_on_new_line; }

auto Snackbar::sizeHint() const -> QSize { return pimpl->size_hint(); }

auto Snackbar::minimumSizeHint() const -> QSize { return pimpl->minimum_size_hint(); }

auto Snackbar::hasHeightForWidth() const -> bool { return true; }

auto Snackbar::heightForWidth(int width) const -> int { return pimpl->height_for_width(width); }

auto Snackbar::resizeEvent(QResizeEvent* event) -> void {
    QWidget::resizeEvent(event);
    update();
}

auto Snackbar::changeEvent(QEvent* event) -> void {
    pimpl->change_event(event);
    QWidget::changeEvent(event);
}

auto Snackbar::enterEvent(qt::EnterEvent* event) -> void { QWidget::enterEvent(event); }

auto Snackbar::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event();
    QWidget::leaveEvent(event);
}

auto Snackbar::mouseMoveEvent(QMouseEvent* event) -> void {
    pimpl->mouse_move_event(event);
    QWidget::mouseMoveEvent(event);
}

auto Snackbar::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(event);
    QWidget::mousePressEvent(event);
}

auto Snackbar::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event(event);
    QWidget::mouseReleaseEvent(event);
}

auto Snackbar::focusInEvent(QFocusEvent* event) -> void {
    pimpl->focus_in_event(event);
    QWidget::focusInEvent(event);
}

auto Snackbar::focusOutEvent(QFocusEvent* event) -> void {
    pimpl->focus_out_event();
    QWidget::focusOutEvent(event);
}

auto Snackbar::focusNextPrevChild(bool next) -> bool {
    return pimpl->focus_next_prev_child(next) || QWidget::focusNextPrevChild(next);
}

auto Snackbar::keyPressEvent(QKeyEvent* event) -> void {
    if (!pimpl->key_press_event(event)) QWidget::keyPressEvent(event);
}

auto Snackbar::keyReleaseEvent(QKeyEvent* event) -> void {
    if (!pimpl->key_release_event(event)) QWidget::keyReleaseEvent(event);
}

auto Snackbar::paintEvent(QPaintEvent*) -> void { pimpl->paint_event(); }

namespace creeper {

struct SnackbarHost::State::Impl {
    co::continuation<SnackbarResult> continuation;
    QTimer timer;

    struct Data {
        // ...
    };
    MutableValue<Data> data;
};

struct SnackbarHost::Impl { };

}
