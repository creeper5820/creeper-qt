#pragma once

#include "creeper-qt/utility/api/pro/clickable.hh"   // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/text-color.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/text.hh"        // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/water-color.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"    // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/shape.hh"     // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"     // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"    // IWYU pragma: keep
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qabstractbutton.h>

namespace creeper {

class FilledButton : public QAbstractButton, public DSL {
    CREEPER_PIMPL_DEFINITION(FilledButton);

public:
    explicit FilledButton(auto&&... props)
        : FilledButton { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme& pack);
    void bindThemeManager(ThemeManager& manager);

    void setRadius(double radius);
    void setBorderWidth(double border);

    void setWaterColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setTextColor(const QColor& color);
    void setBackground(const QColor& color);
    void setHoverColor(const QColor& color);

    void setWaterRippleStatus(bool enable);
    void setWaterRippleStep(double step);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;

    void enterEvent(qt::EnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;
};

}
namespace creeper::filled_button::pro {
using HoverColor        = ForwardProp<&FilledButton::setHoverColor>;
using WaterRippleStatus = ForwardProp<&FilledButton::setWaterRippleStatus>;
using WaterRippleStep   = ForwardProp<&FilledButton::setWaterRippleStep>;

using api::pro::Clickable;
using api::pro::Text;
using api::pro::TextColor;
using api::pro::WaterColor;

using namespace api::scope::common;
using namespace api::scope::shape;
using namespace api::scope::theme;
using namespace api::scope::widget;
}
