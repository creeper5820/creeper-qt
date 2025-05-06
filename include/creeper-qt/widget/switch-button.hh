#pragma once

#include "creeper-qt/widget/widget.hh"
#include <qabstractbutton.h>
#include <qtimer.h>

namespace creeper {

class AbstractSwitchButton : public Extension<QAbstractButton> {
    Q_OBJECT
public:
    AbstractSwitchButton(QWidget* parent = nullptr);

    void setSwitchStatus(bool switchStatus);
    bool switched() const;
    void reloadTheme() override;

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;

    QTimer animationTimer_;
    uint32_t lightGrey_ = 0xdddddd;
    uint32_t heavyGrey_ = 0xaaaaaa;
    uint32_t lightPrimary_ = 0x7c55bb;
    uint32_t heavyPrimary_ = 0x5d34a9;

    bool switchStatus_ = false;
    double progress_ = 0;
};

class ConvexSwitchButton : public AbstractSwitchButton {
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(ConvexSwitchButton, AbstractSwitchButton)
    Q_OBJECT
public:
    ConvexSwitchButton(QWidget* parent = nullptr);
    ConvexSwitchButton& setSwitchStatus(bool switchStatus) {
        AbstractSwitchButton::setSwitchStatus(switchStatus);
        return *this;
    }

protected:
    void paintEvent(QPaintEvent* event) override;
};

class ConcaveSwitchButton : public AbstractSwitchButton {
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(ConcaveSwitchButton, AbstractSwitchButton)
    Q_OBJECT
public:
    ConcaveSwitchButton(QWidget* parent = nullptr);
    ConcaveSwitchButton& setSwitchStatus(bool switchStatus) {
        AbstractSwitchButton::setSwitchStatus(switchStatus);
        return *this;
    }

protected:
    void paintEvent(QPaintEvent* event) override;
};

}
