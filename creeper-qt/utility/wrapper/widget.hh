#pragma once

#include "property.hh"
#include <qwidget.h>

namespace cr {

class Widget : public QWidget {
public:
    using QWidget::QWidget;

    explicit Widget(pro::qwidget_property_concept auto... properties) {
        apply_properties(properties...);
    }

    void apply_properties(pro::qwidget_property_concept auto... properties) {
        (apply_property(std::forward<decltype(properties)>(properties)), ...);
    }

    void apply_property(const pro::qwidget_property_concept auto& _) {
        static_cast<const pro::internal::QWidgetProperty&>(_).apply(*this);
    }

private:
};

}
