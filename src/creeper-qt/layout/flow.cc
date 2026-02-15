#include "flow.hh"
#include <qstyle.h>

using namespace creeper::flow::internal;

struct Flow::Impl {
    QList<Item*> items;

    int row_spacing = 10;
    int col_spacing = 10;

    int row_limit = std::numeric_limits<int>::max();

    Flow& self;

    explicit Impl(Flow& self) noexcept
        : self { self } { }

    auto calculate_spacing(QStyle::PixelMetric pm) const -> int {
        const auto parent = self.parent();
        if (!parent) {
            return -1;
        } else if (parent->isWidgetType()) {
            const auto pw = static_cast<QWidget*>(parent);
            return pw->style()->pixelMetric(pm, nullptr, pw);
        } else {
            return static_cast<QLayout*>(parent)->spacing();
        }
    }
    auto horizontal_spacing() const -> int {
        if (row_spacing >= 0) {
            return row_spacing;
        } else {
            return calculate_spacing(QStyle::PM_LayoutHorizontalSpacing);
        }
    }
    auto vertical_spacing() const -> int {
        if (col_spacing >= 0) {
            return col_spacing;
        } else {
            return calculate_spacing(QStyle::PM_LayoutVerticalSpacing);
        }
    }

    auto update_items_geometry(Item* item, const QPoint& point) const {
        // TODO: 显然，这个接口未来可以拓展成带动画的位姿更新
        item->setGeometry({ point, item->sizeHint() });
    }

    // Flow 理应是一个常用的布局，但 Qt 中没有对应的实现，
    // https://doc.qt.io/archives/qt-5.15/qtwidgets-layouts-flowlayout-example.html
    auto calculate_layout(const QRect& rect, bool apply) const -> int {

        int left, top, right, bottom;
        self.getContentsMargins(&left, &top, &right, &bottom);

        const auto effective_rect = rect.adjusted(+left, +top, -right, -bottom);

        auto current_x = effective_rect.x();
        auto current_y = effective_rect.y();

        auto line_height = int { 0 };
        auto line_length = int { 0 };
        for (auto item : std::as_const(items)) {
            const auto widget  = item->widget();
            const auto spacing = [widget](Qt::Orientation o) {
                return widget->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, o);
            };

            auto space_x = row_spacing;
            if (space_x == -1) space_x = spacing(Qt::Horizontal);

            auto space_y = col_spacing;
            if (space_y == -1) space_y = spacing(Qt::Vertical);

            auto next_x = current_x + item->sizeHint().width() + space_x;

            const auto area_flag = next_x - space_x > effective_rect.right();
            const auto size_flag = line_length > row_limit;
            if ((area_flag || size_flag) && line_height > 0) {
                current_x   = effective_rect.x();
                current_y   = current_y + line_height + space_y;
                next_x      = current_x + item->sizeHint().width() + space_x;
                line_height = 0;
                line_length = 0;
            }

            if (apply) {
                const auto point = QPoint { current_x, current_y };
                update_items_geometry(item, point);
            }

            current_x   = next_x;
            line_height = std::max(line_height, item->sizeHint().height());
            line_length = line_length + 1;
        }
        return current_y + line_height - rect.y() + bottom;
    }
};

Flow::Flow()
    : pimpl { std::make_unique<Impl>(*this) } { }

Flow::~Flow() {
    while (auto item = Flow::takeAt(0))
        delete item;
}

auto Flow::addItem(Item* item) -> void { pimpl->items.append(item); }

auto Flow::setGeometry(const QRect& rect) -> void {
    QLayout::setGeometry(rect);
    pimpl->calculate_layout(rect, true);
}

auto Flow::takeAt(int index) -> Item* {
    auto& items = pimpl->items;
    return (index < 0 || index > items.size() - 1) ? nullptr : items.takeAt(index);
}

auto Flow::expandingDirections() const -> Qt::Orientations { return {}; }

auto Flow::hasHeightForWidth() const -> bool { return true; }

auto Flow::heightForWidth(int width) const -> int {
    return pimpl->calculate_layout({ 0, 0, width, 0 }, false);
}

auto Flow::itemAt(int index) const -> Item* { return pimpl->items.value(index); }

auto Flow::count() const -> int { return pimpl->items.size(); }

auto Flow::minimumSize() const -> QSize {
    auto result = QSize {};
    for (const auto item : std::as_const(pimpl->items))
        result = result.expandedTo(item->minimumSize());

    const auto margins = contentsMargins();
    result += QSize {
        margins.left() + margins.right(),
        margins.top() + margins.bottom(),
    };
    return result;
}

auto Flow::sizeHint() const -> QSize { return Flow::minimumSize(); }

auto Flow::set_row_spacing(int spacing) noexcept -> void { pimpl->row_spacing = spacing; }
auto Flow::row_spacing() const noexcept -> int { return pimpl->row_spacing; }

auto Flow::set_col_spacing(int spacing) noexcept -> void { pimpl->col_spacing = spacing; }
auto Flow::col_spacing() const noexcept -> int { return pimpl->col_spacing; }

auto Flow::set_row_limit(int limit) noexcept -> void { pimpl->row_limit = limit; }
auto Flow::row_limit() const noexcept -> int { return pimpl->row_limit; }
