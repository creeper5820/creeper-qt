#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/widget/widget.hh"

#include <qscrollarea.h>
#include <qscrollbar.h>

namespace creeper {

/// NOTE: 先拿 qss 勉强用着吧，找时间完全重构
class ScrollArea : public QScrollArea, public DSL {
public:
    explicit ScrollArea(auto&&... props) {
        viewport()->setStyleSheet("background:transparent;border:none;");
        setStyleSheet("QScrollArea{background:transparent;border:none;}");

        setWidgetResizable(true);

        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme& scheme) {
        constexpr auto q = [](const QColor& c, int a = 255) {
            return QString("rgba(%1,%2,%3,%4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(a);
        };

        verticalScrollBar()->setStyleSheet(QString {
            "QScrollBar:vertical{background:transparent;width:8px;border-radius:4px;}"
            "QScrollBar::handle:vertical{background:%1;min-height:20px;border-radius:4px;}"
            "QScrollBar::handle:vertical:hover{background:%2;}"
            "QScrollBar::handle:vertical:pressed{background:%3;}"
            "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical,"
            "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{height:0px;}",
        }
                .arg(q(scheme.primary, 235))
                .arg(q(scheme.primary))
                .arg(q(scheme.primary.darker(110))));

        horizontalScrollBar()->setStyleSheet(QString {
            "QScrollBar:horizontal{background:transparent;height:8px;border-radius:4px;}"
            "QScrollBar::handle:horizontal{background:%1;min-width:20px;border-radius:4px;}"
            "QScrollBar::handle:horizontal:hover{background:%2;}"
            "QScrollBar::handle:horizontal:pressed{background:%3;}"
            "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal,"
            "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{width:0px;}",
        }
                .arg(q(scheme.primary, 235))
                .arg(q(scheme.primary))
                .arg(q(scheme.primary.darker(110))));
    }

    void bindThemeManager(ThemeManager& manager) {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

}

namespace creeper::scroll::pro {
using VerticalScrollBarPolicy   = ForwardProp<&QScrollArea::setVerticalScrollBarPolicy>;
using HorizontalScrollBarPolicy = ForwardProp<&QScrollArea::setHorizontalScrollBarPolicy>;
struct ScrollBarPolicy {
    Qt::ScrollBarPolicy v;
    Qt::ScrollBarPolicy h;

    explicit ScrollBarPolicy(Qt::ScrollBarPolicy v, Qt::ScrollBarPolicy h) noexcept
        : v { v }
        , h { h } { }

    friend auto dsl_invoke(ScrollArea& self, const ScrollBarPolicy& prop) -> void {
        self.setVerticalScrollBarPolicy(prop.v);
        self.setHorizontalScrollBarPolicy(prop.h);
    }
};
template <item_trait T>
struct ScrollItem {
    T* item_pointer = nullptr;

    explicit ScrollItem(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    explicit ScrollItem(T* pointer) noexcept
        : item_pointer { pointer } { }

    friend auto dsl_invoke(ScrollArea& self, const ScrollItem& prop) -> void {
        if constexpr (widget_trait<T>) {
            self.setWidget(prop.item_pointer);
        }
        // NOTE: 这里可能有调整的空间，直接设置 Layout，
        //       布局 Size 行为是不正确的
        else if constexpr (layout_trait<T>) {
            const auto content = new creeper::Widget { };
            content->setLayout(prop.item_pointer);
            self.setWidget(content);
        }
    }
};

using namespace api::scope::common;
using namespace api::scope::theme;
using namespace api::scope::widget;
}

namespace creeper::scrollable::details {

class Scrollable : public QWidget {
    CREEPER_PIMPL_DEFINITION(Scrollable)

protected:
    auto paintEvent(QPaintEvent*) -> void override;
};

}
