#pragma once

#include <qboxlayout.h>
#include <qlayoutitem.h>
#include <qobject.h>
#include <qwidget.h>
#include <type_traits>

namespace creeper {

template <typename Layout, typename Child>
    requires std::is_base_of_v<QBoxLayout, Layout>
class Container : public Layout {
public:
    /// @brief 创建一个新的 Container，并返回引用，你问我为什么返回引用，我只是不想让链式调用
    /// 的第一个调用用箭头，而其他的用点罢了
    static Child& create() { return *(new Child); }

    /// @brief 显然，这个函数只是返回该容器的指针，但你不觉得先 create 后
    /// 再调用 build 很酷吗
    /// @note 记得和 create 组合使用哦，要是生命周期不对，出了作用域这容器就被析构了
    [[nodiscard]] Child* build() { return static_cast<Child*>(this); }

public:
    Container& add(QWidget* widget, int stretch = 0, Qt::Alignment alignment = {}) {
        Layout::addWidget(widget, stretch, alignment);
        return *this;
    }
    Container& add(QLayout* layout, int stretch = 0) {
        Layout::addLayout(layout, stretch);
        return *this;
    }
    Container& add(QLayoutItem* item) {
        Layout::addItem(item);
        return *this;
    }
    Container& add(QSpacerItem* spacerItem) {
        Layout::addSpacerItem(spacerItem);
        return *this;
    }
    Container& addSpacing(int size) {
        Layout::addSpacing(size);
        return *this;
    }
    Container& addStretch(int stretch) {
        Layout::addStretch(stretch);
        return *this;
    }
    Container& addStrut(int size) {
        Layout::addStrut(size);
        return *this;
    }

public:
    Container& setAlignment(Qt::Alignment alignment) {
        Layout::setAlignment(alignment);
        return *this;
    }
    Container& setMargin(int margin) {
        Layout::setMargin(margin);
        return *this;
    }
    Container& setSpacing(int spacing) {
        Layout::setSpacing(spacing);
        return *this;
    }
};

class Column : public Container<QVBoxLayout, Column> { };

class Row : public Container<QHBoxLayout, Row> { };

}
