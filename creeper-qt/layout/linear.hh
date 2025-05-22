#pragma once

#include "utility/wrapper/property.hh"
#include <qlayout.h>

namespace creeper {

namespace pro::linear {
    struct Property {
        virtual void apply(QBoxLayout& _) const = 0;
    };
    template <typename T>
    concept property_concept = std::is_base_of_v<Property, T>;

    struct Widget : Property {
        std::tuple<QWidget*, int, Qt::Alignment> args;
        explicit Widget(QWidget* p0, int p1 = 0, Qt::Alignment p2 = {}) {
            args = std::tuple { p0, p1, p2 };
        }
        void apply(QBoxLayout& self) const override {
            const auto& [p0, p1, p2] = args;
            self.addWidget(p0, p1, p2);
        }
    };
    struct Layout : Property {
        std::tuple<QLayout*, int> args;
        explicit Layout(QLayout* p0, int p1 = 0) { args = std::tuple { p0, p1 }; }
        void apply(QBoxLayout& self) const override {
            const auto& [p0, p1] = args;
            self.addLayout(p0, p1);
        }
    };
}

namespace linear {
    using namespace pro::linear;
    using pro::linear::property_concept;
}

class Row : public QHBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Row, linear)
public:
    using QHBoxLayout::QHBoxLayout;
};

class Col : public QVBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Col, linear)
public:
    using QVBoxLayout::QVBoxLayout;
};

}
