#pragma once
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/widget.hh"
#include <qscrollarea.h>
#include <qscrollbar.h>

namespace creeper::scroll::internal {

/// NOTE: 先拿 qss 勉强用着吧，找时间完全重构
class ScrollArea : public QScrollArea {
public:
    explicit ScrollArea() noexcept {
        viewport()->setStyleSheet("background:transparent;border:none;");
        setStyleSheet("QScrollArea{background:transparent;border:none;}");

        setWidgetResizable(true);
    }

    void set_color_scheme(const ColorScheme& scheme) {
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

    void load_theme_manager(ThemeManager& manager) {
        manager.append_handler(this,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }
};

}
namespace creeper::scroll::pro {

using Token = common::Token<internal::ScrollArea>;

struct VerticalScrollBarPolicy : Token {
    Qt::ScrollBarPolicy policy;

    explicit VerticalScrollBarPolicy(Qt::ScrollBarPolicy policy) noexcept
        : policy { policy } { }

    auto apply(auto& self) const noexcept -> void { //
        self.setVerticalScrollBarPolicy(policy);
    }
};
struct HorizontalScrollBarPolicy : Token {
    Qt::ScrollBarPolicy policy;

    explicit HorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy) noexcept
        : policy { policy } { }

    auto apply(auto& self) const noexcept -> void { //
        self.setHorizontalScrollBarPolicy(policy);
    }
};
struct ScrollBarPolicy : Token {
    Qt::ScrollBarPolicy v;
    Qt::ScrollBarPolicy h;

    explicit ScrollBarPolicy(Qt::ScrollBarPolicy v, Qt::ScrollBarPolicy h) noexcept
        : v { v }
        , h { h } { }

    auto apply(auto& self) const noexcept -> void {
        self.setVerticalScrollBarPolicy(v);
        self.setHorizontalScrollBarPolicy(h);
    }
};

template <item_trait T>
struct Item : Token {
    T* item_pointer = nullptr;

    explicit Item(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    explicit Item(T* pointer) noexcept
        : item_pointer { pointer } { }

    auto apply(area_trait auto& layout) const noexcept -> void {
        if constexpr (widget_trait<T>) {
            layout.setWidget(item_pointer);
        }
        // NOTE: 这里可能有调整的空间，直接设置 Layout，
        //       布局 Size 行为是不正确的
        else if constexpr (layout_trait<T>) {
            const auto content = new Widget {
                widget::pro::Layout { item_pointer },
            };
            layout.setWidget(content);
        }
    }
};

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace widget::pro;
using namespace theme::pro;
}
namespace creeper {

using ScrollArea = Declarative<scroll::internal::ScrollArea,
    CheckerOr<scroll::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
