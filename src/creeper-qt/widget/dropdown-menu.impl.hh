#pragma once
#include "dropdown-menu.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/widget/dropdown-menu-item.hh"

#include <algorithm>
#include <array>

#include <qboxlayout.h>
#include <qdebug.h>
#include <qevent.h>
#include <qgraphicseffect.h>
#include <qlist.h>
#include <qpainter.h>
#include <qscreen.h>
#include <qtimer.h>

using namespace creeper::dropdown_menu::details;

using MenuItemDetails = creeper::dropdown_menu_item::details::DropdownMenuItem;

struct DropdownMenu::Impl {
public:
    static constexpr auto kContainerVPadding = 8;

    static constexpr auto kWindowVMargin = 48;
    static constexpr auto kWindowHMargin = 8;

    static constexpr auto kShadowBlurRadius = 10;
    static constexpr auto kShadowOffsetY    = 2;
    static constexpr auto kShadowMargin     = 16;
    static constexpr auto kShadowOpacity    = 0.28;

    // M3 spec: 菜单容器最小宽度 112dp，最大宽度 280dp
    static constexpr auto kMinMenuWidth = 112;
    static constexpr auto kMaxMenuWidth = 280;

    QWidget* container          = nullptr;
    QWidget* viewport           = nullptr;
    QVBoxLayout* content_layout = nullptr;

    QWidget* anchor_widget = nullptr;
    QWidget* filter_anchor = nullptr;
    QPoint offset { 0, 0 };

    QColor theme_container_color;
    QColor container_color_override;
    double corner_radius                           = 4.0;
    QGraphicsDropShadowEffect* shadow_effect       = nullptr;
    QGraphicsOpacityEffect* content_opacity_effect = nullptr;

    QPointF transform_origin { 0.5, 0.0 };

    int scroll_offset   = 0;
    int highlight_index = -1;

    bool expanded_          = false;
    bool closing            = false;
    bool programmatic_close = false;

    Animatable animatable;
    std::unique_ptr<TransitionValue<SpringState<double>>> scale;
    std::unique_ptr<TransitionValue<SpringState<double>>> opacity;

    DropdownMenu& self;

    explicit Impl(DropdownMenu& self) noexcept
        : animatable { self }
        , self { self } {
        self.setAttribute(Qt::WA_TranslucentBackground);
        {
            auto effect = new QGraphicsDropShadowEffect { &self };
            effect->setBlurRadius(kShadowBlurRadius);
            effect->setOffset(0, kShadowOffsetY);
            self.setGraphicsEffect(effect);
            shadow_effect = effect;
        }
        {
            auto state            = std::make_shared<SpringState<double>>();
            state->config.k       = 1400.0;
            state->config.d       = 67.35;
            state->config.epsilon = 1e-3;
            scale                 = make_transition(animatable, std::move(state));
        }
        {
            auto state            = std::make_shared<SpringState<double>>();
            state->config.k       = 3800.0;
            state->config.d       = 123.29;
            state->config.epsilon = 1e-3;
            opacity               = make_transition(animatable, std::move(state));
        }

        container = new QWidget { &self };
        viewport  = new QWidget { container };

        // 菜单项是独立子 widget，不随 paintEvent 中的 painter 透明度变化，
        // 需要用 QGraphicsOpacityEffect 让整棵内容子树随展开动画一同渐变
        {
            auto effect = new QGraphicsOpacityEffect { container };
            effect->setOpacity(1.0);
            container->setGraphicsEffect(effect);
            content_opacity_effect = effect;
        }

        content_layout = new QVBoxLayout { viewport };
        content_layout->setContentsMargins(0, kContainerVPadding, 0, kContainerVPadding);
        content_layout->setSpacing(0);
    }

    auto set_color_scheme(const ColorScheme& scheme) -> void {
        theme_container_color = scheme.surface_container;

        auto shadow = scheme.shadow;
        shadow.setAlphaF(kShadowOpacity);
        shadow_effect->setColor(shadow);

        self.update();
    }

    auto load_theme_manager(ThemeManager& manager) -> void {
        manager.append_handler(&self,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }

    auto set_anchor(QWidget* widget) -> void {
        if (widget == nullptr) return;

        anchor_widget = widget;
        self.setParent(widget, self.windowFlags());

        if (self.isVisible()) {
            // 可见时换锚：迁移事件过滤器并重新定位
            uninstall_anchor_filters();
            install_anchor_filters(widget);
            reposition();
        } else if (expanded_) {
            show_menu();
        }
    }

    auto parent_changed() -> void {
        // Child<T> 等声明式挂载在 prop 求值之后才设置 parent，
        // 此时补齐因缺少锚点而未能执行的展开
        if (expanded_ && !self.isVisible()) show_menu();
    }

    auto anchor() const noexcept -> QWidget* { return anchor_widget; }

    auto expanded() const noexcept -> bool { return expanded_; }

    auto set_expanded(bool value) -> void {
        if (expanded_ == value) return;

        expanded_ = value;
        if (expanded_) {
            show_menu();
        } else {
            close_programmatically();
        }
    }

    auto set_offset(QPoint value) -> void {
        offset = value;
        if (self.isVisible()) reposition();
    }

    auto set_container_color(const QColor& color) -> void {
        container_color_override = color;
        self.update();
    }

    auto set_corner_radius(double value) -> void {
        corner_radius = value;
        self.update();
    }

    auto add_item(QWidget* widget) -> void {
        content_layout->addWidget(widget);
        if (expanded_ && !self.isVisible()) show_menu();
    }

    auto content_count() const noexcept -> int { return content_layout->count(); }

    auto paint_event(QPaintEvent*) -> void {
        const auto scale_value   = scale->get_value();
        const auto opacity_value = opacity->get_value();
        const auto margin        = kShadowMargin * 1.;

        content_opacity_effect->setOpacity(opacity_value);

        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setOpacity(opacity_value);

        const auto origin = QPointF {
            transform_origin.x() * self.width(),
            transform_origin.y() * self.height(),
        };
        painter.translate(origin);
        painter.scale(scale_value, scale_value);
        painter.translate(-origin);

        util::PainterHelper { painter }
            .set_render_hint(QPainter::Antialiasing)
            .rounded_rectangle(effective_container_color(), Qt::transparent, 0,
                QRectF { margin, margin, self.width() - 2. * margin, self.height() - 2. * margin },
                corner_radius, corner_radius);

        if (closing && opacity_value < 0.02) {
            QTimer::singleShot(0, &self, [this] { self.hide(); });
        }
    }

    auto hide_event(QHideEvent*) -> void {
        closing   = false;
        expanded_ = false;

        uninstall_anchor_filters();

        // 仅用户驱动的关闭（外部点击、Esc）才上报 dismiss_requested
        if (!programmatic_close) Q_EMIT self.dismiss_requested();
        programmatic_close = false;
    }

    auto event_filter(QObject* watched, QEvent* event) -> bool {
        if (watched == filter_anchor && event->type() == QEvent::Hide) {
            programmatic_close = true;
            self.hide();
            return false;
        }
        if (event->type() == QEvent::Move || event->type() == QEvent::Resize) {
            reposition();
        }
        return false;
    }

    auto wheel_event(QWheelEvent* event) -> void {
        scroll_offset = std::clamp(scroll_offset - event->angleDelta().y(), 0, max_scroll_offset());
        viewport->move(0, -scroll_offset);
    }

    auto key_press_event(QKeyEvent* event) -> void {
        const auto items = collect_items();
        switch (event->key()) {
        case Qt::Key_Down:
            move_highlight(items, +1);
            break;

        case Qt::Key_Up:
            move_highlight(items, -1);
            break;

        case Qt::Key_Return:
            [[fallthrough]];
        case Qt::Key_Enter:
            [[fallthrough]];
        case Qt::Key_Space:
            activate_highlight(items);
            break;

        case Qt::Key_Escape:
            close_by_user_request();
            break;

        default:
            return;
        }
    }

private:
    auto show_menu() -> void {
        if (content_layout->count() == 0) return;

        const auto anchor = resolved_anchor();
        if (anchor == nullptr) {
            qWarning() << "[DropdownMenu] cannot expand without an anchor widget";
            return;
        }

        install_anchor_filters(anchor);

        reposition();

        closing            = false;
        programmatic_close = false;
        highlight_index    = -1;
        scroll_offset      = 0;
        viewport->move(0, 0);

        scale->snap_to(0.8);
        opacity->snap_to(0.0);

        self.show();

        scale->transition_to(1.0);
        opacity->transition_to(1.0);
    }

    /// 程序性关闭：应用通过 set_expanded(false) 发起，不 emit dismiss_requested
    auto close_programmatically() -> void {
        if (!self.isVisible() || closing) return;

        programmatic_close = true;
        start_close_animation();
    }

    /// 用户请求关闭：Esc 等路径，hide 后 emit dismiss_requested
    auto close_by_user_request() -> void {
        if (!self.isVisible() || closing) return;

        programmatic_close = false;
        start_close_animation();
    }

    auto start_close_animation() -> void {
        closing = true;
        scale->transition_to(0.8);
        opacity->transition_to(0.0);
    }

    auto install_anchor_filters(QWidget* anchor) -> void {
        filter_anchor = anchor;
        anchor->installEventFilter(&self);
        if (anchor->window() != anchor) {
            anchor->window()->installEventFilter(&self);
        }
    }

    auto uninstall_anchor_filters() -> void {
        if (filter_anchor == nullptr) return;

        filter_anchor->removeEventFilter(&self);
        if (filter_anchor->window() != nullptr && filter_anchor->window() != filter_anchor) {
            filter_anchor->window()->removeEventFilter(&self);
        }
        filter_anchor = nullptr;
    }

    auto resolved_anchor() const noexcept -> QWidget* {
        return anchor_widget != nullptr ? anchor_widget : self.parentWidget();
    }

    auto effective_container_color() const noexcept -> QColor {
        return container_color_override.isValid() ? container_color_override
                                                  : theme_container_color;
    }

    auto collect_items() const -> QList<MenuItemDetails*> {
        auto result = QList<MenuItemDetails*> { };
        for (auto i = 0; i < content_layout->count(); ++i) {
            if (const auto item =
                    qobject_cast<MenuItemDetails*>(content_layout->itemAt(i)->widget())) {
                result.append(item);
            }
        }
        return result;
    }

    auto move_highlight(const QList<MenuItemDetails*>& items, int step) -> void {
        if (items.isEmpty()) return;

        const auto next = highlight_index < 0
            ? (step > 0 ? 0 : static_cast<int>(items.size()) - 1)
            : std::clamp(highlight_index + step, 0, static_cast<int>(items.size()) - 1);
        if (next == highlight_index) return;

        if (highlight_index >= 0 && highlight_index < items.size()) {
            auto leave = QEvent { QEvent::Leave };
            QCoreApplication::sendEvent(items.at(highlight_index), &leave);
        }

        highlight_index = next;

        const auto item   = items.at(highlight_index);
        const auto center = QPointF { item->rect().center() };
        auto enter        = QEnterEvent { center, center, item->mapToGlobal(center.toPoint()) };
        QCoreApplication::sendEvent(item, &enter);

        ensure_visible(item);
    }

    auto activate_highlight(const QList<MenuItemDetails*>& items) -> void {
        if (highlight_index < 0 || highlight_index >= items.size()) return;

        const auto item   = items.at(highlight_index);
        const auto center = QPointF { item->rect().center() };
        const auto global = item->mapToGlobal(center.toPoint());

        auto press = QMouseEvent { QEvent::MouseButtonPress, center, global, Qt::LeftButton,
            Qt::LeftButton, Qt::NoModifier };
        QCoreApplication::sendEvent(item, &press);

        auto release = QMouseEvent { QEvent::MouseButtonRelease, center, global, Qt::LeftButton,
            Qt::NoButton, Qt::NoModifier };
        QCoreApplication::sendEvent(item, &release);
    }

    auto ensure_visible(QWidget* item) -> void {
        const auto top     = item->geometry().top();
        const auto bottom  = item->geometry().bottom() + 1;
        const auto visible = container->height();

        if (top < scroll_offset) scroll_offset = top;
        if (bottom > scroll_offset + visible) scroll_offset = bottom - visible;

        scroll_offset = std::clamp(scroll_offset, 0, max_scroll_offset());
        viewport->move(0, -scroll_offset);
    }

    auto reposition() -> void {
        const auto anchor = resolved_anchor();
        if (anchor == nullptr) return;

        const auto anchor_rect = QRect { anchor->mapToGlobal(QPoint { 0, 0 }), anchor->size() };
        const auto screen_rect = anchor->screen()->availableGeometry();

        const auto content_size = content_layout->sizeHint();
        const auto menu_size    = QSize {
            std::min(std::clamp(content_size.width(), kMinMenuWidth, kMaxMenuWidth),
                screen_rect.width() - 2 * kWindowHMargin),
            std::min(content_size.height(), available_height(anchor_rect, screen_rect)),
        };
        const auto window_size = QSize {
            menu_size.width() + 2 * kShadowMargin,
            menu_size.height() + 2 * kShadowMargin,
        };
        auto position = QPoint { };
        {
            const auto x_candidates = std::array {
                anchor_rect.left(),
                anchor_rect.right() - menu_size.width() + 1,
                anchor_rect.center().x() < screen_rect.center().x()
                    ? screen_rect.left() + kWindowHMargin
                    : screen_rect.right() - kWindowHMargin - menu_size.width() + 1,
            };
            for (const auto x : x_candidates) {
                position.setX(
                    std::clamp(x, screen_rect.left(), screen_rect.right() - menu_size.width() + 1));
                if (x == position.x()) break;
            }

            const auto below = anchor_rect.bottom() + 1;
            const auto above = anchor_rect.top() - menu_size.height();
            if (below + menu_size.height() <= screen_rect.bottom() + 1) {
                position.setY(below);
            } else if (above >= screen_rect.top()) {
                position.setY(above);
            } else {
                const auto lo = screen_rect.top() + kWindowVMargin;
                const auto hi = screen_rect.bottom() - kWindowVMargin - menu_size.height() + 1;
                position.setY(std::clamp(
                    anchor_rect.center().y() < screen_rect.center().y() ? lo : hi, lo, hi));
            }
        }

        // 叠加用户偏移，RTL 布局下 x 方向取反
        position += QPoint {
            self.layoutDirection() == Qt::RightToLeft ? -offset.x() : offset.x(),
            offset.y(),
        };

        const auto window_position = position - QPoint { kShadowMargin, kShadowMargin };

        {
            const auto menu_rect   = QRect { position, menu_size };
            const auto window_rect = QRect { window_position, window_size };
            const auto pivot = [](int menu_min, int menu_max, int anchor_min, int anchor_max) {
                if (menu_min >= anchor_max) return 0.0;
                if (menu_max <= anchor_min) return 1.0;
                if (menu_max == menu_min) return 0.0;
                const auto center =
                    (std::max(menu_min, anchor_min) + std::min(menu_max, anchor_max)) / 2.0;
                return (center - menu_min) / (menu_max - menu_min);
            };
            const auto pivot_x = pivot(menu_rect.left(), menu_rect.right() + 1, anchor_rect.left(),
                anchor_rect.right() + 1);
            const auto pivot_y = pivot(menu_rect.top(), menu_rect.bottom() + 1, anchor_rect.top(),
                anchor_rect.bottom() + 1);
            transform_origin   = QPointF {
                (pivot_x * menu_size.width() + kShadowMargin) / window_size.width(),
                (pivot_y * menu_size.height() + kShadowMargin) / window_size.height(),
            };
        }

        self.setFixedSize(window_size);
        self.move(window_position);

        container->setGeometry(kShadowMargin, kShadowMargin, menu_size.width(), menu_size.height());
        viewport->resize(menu_size.width(), content_height());
        scroll_offset = std::clamp(scroll_offset, 0, max_scroll_offset());
        viewport->move(0, -scroll_offset);
    }

    auto content_height() const noexcept -> int { return content_layout->sizeHint().height(); }

    auto max_scroll_offset() const noexcept -> int {
        return std::max(0, content_height() - container->height());
    }

    auto available_height(const QRect& anchor_rect, const QRect& screen_rect) const noexcept
        -> int {
        const auto above = anchor_rect.top() - screen_rect.top() - kWindowVMargin;
        const auto below = screen_rect.bottom() - kWindowVMargin - anchor_rect.bottom();
        return std::max({ 0, above, below });
    }
};
