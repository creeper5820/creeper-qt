#pragma once

#include "utility/wrapper/property.hh"
#include <qlayout.h>

namespace creeper {

namespace pro::linear {
    using Property = InternalProperty<QBoxLayout>;
    template <typename T>
    concept property_concept = std::is_base_of_v<Property, T>;

    struct Widget final : Property {
        struct Pack {
            QWidget* widget;
            int stretch = 0;
            Qt::Alignment alignment = Qt::Alignment();
        } widget;
        explicit Widget(const Pack& p)
            : widget(p) { }
        void apply(QBoxLayout& self) const override {
            const auto& [p0, p1, p2] = widget;
            self.addWidget(p0, p1, p2);
        }
    };
    struct Widgets final : Property {
        std::vector<Widget::Pack> widgets;
        explicit Widgets(std::vector<Widget::Pack> p)
            : widgets(p) { }
        void apply(QBoxLayout& self) const override {
            for (const auto& widget : widgets) {
                const auto& [p0, p1, p2] = widget;
                self.addWidget(p0, p1, p2);
            }
        }
    };

    struct Layout final : Property {
        struct Pack {
            QLayout* layout;
            int stretch = 0;
        } layout;
        explicit Layout(const Pack& param) { layout = param; }
        void apply(QBoxLayout& self) const override {
            const auto& [p0, p1] = layout;
            self.addLayout(p0, p1);
        }
    };
    struct Layouts final : Property {
        std::vector<Layout::Pack> layouts;
        explicit Layouts(std::vector<Layout::Pack> p) { layouts = p; }
        void apply(QBoxLayout& self) const override {
            for (const auto& layout : layouts) {
                const auto& [p0, p1] = layout;
                self.addLayout(p0, p1);
            }
        }
    };

    struct Spacing final : Property {
        int size;
        explicit Spacing(int p) { size = p; }
        void apply(QBoxLayout& self) const override { self.addSpacing(size); }
    };

    struct Stretch final : Property {
        int stretch;
        explicit Stretch(int p) { stretch = p; }
        void apply(QBoxLayout& self) const override { self.addStretch(stretch); }
    };

    struct SpacerItem final : Property {
        QSpacerItem* spacer_item;
        explicit SpacerItem(QSpacerItem* p) { spacer_item = p; }
        void apply(QBoxLayout& self) const override { self.addSpacerItem(spacer_item); }
    };

    // 属性类接口
    struct Margin final : Property {
        int margin;
        explicit Margin(int p) { margin = p; }
        void apply(QBoxLayout& self) const override { self.setMargin(margin); }
    };

    struct ContentsMargin final : public QMargins, Property {
        using QMargins::QMargins;
        explicit ContentsMargin(int left, int top, int right, int bottom)
            : QMargins(left, top, right, bottom) { }
        void apply(QBoxLayout& self) const override { self.setContentsMargins(*this); }
    };
}

class Row : public QHBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Row, pro::linear, [] { })
public:
    using QHBoxLayout::QHBoxLayout;
};

class Col : public QVBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Col, pro::linear, [] { })
public:
    using QVBoxLayout::QVBoxLayout;
};

}
