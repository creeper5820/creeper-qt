#pragma once

#include "creeper-qt/utility/api/scope/common.hh"

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

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

using namespace common::pro;
using namespace api::scope::common;
using namespace widget::pro;
using namespace theme::pro;

using HoverColor        = ForwardProp<&FilledButton::setHoverColor>;
using WaterRippleStatus = ForwardProp<&FilledButton::setWaterRippleStatus>;
using WaterRippleStep   = ForwardProp<&FilledButton::setWaterRippleStep>;

}
