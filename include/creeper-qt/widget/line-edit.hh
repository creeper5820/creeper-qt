#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include <qlineedit.h>

namespace creeper {

class LineEdit : public Extension<QLineEdit> {
    Q_OBJECT
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(LineEdit, Extension<QLineEdit>)
    CREEPER_WIDGET_PIMPL_DEFINTION(LineEdit)

public:
    LineEdit& setIcon(const QIcon& icon);
    LineEdit& enableIcon();
    LineEdit& disableIcon();

    LineEdit& enableBackground();
    LineEdit& disableBackground();

    /// @todo More setter
    LineEdit& setPlaceholderText(const QString& text);

    void reloadTheme() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

}