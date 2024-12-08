#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include "qlistwidget.h"

namespace creeper {
class CustomItemWidgetInterface : public Extension<QWidget>, public QListWidgetItem {
    CREEPER_WIDGET_PIMPL_DEFINTION(CustomItemWidgetInterface)
    Q_OBJECT
public:
    void reloadTheme() override;
    void setFirst();
    void setLast();
    void disableLast();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

class SwitchAndLabelItem : public CustomItemWidgetInterface {
    CREEPER_WIDGET_PIMPL_DEFINTION(SwitchAndLabelItem)
public:
    explicit SwitchAndLabelItem(const QString& label, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

class ListWidget : public Extension<QListWidget> {
    CREEPER_WIDGET_PIMPL_DEFINTION(ListWidget)
public:
    void reloadTheme() override;
    void addCustomItem(CustomItemWidgetInterface* item);
    void addSwitchAndLabel(const QString& label);
    void addSimpleLabel(const QString& label);

protected:
    void paintEvent(QPaintEvent* event) override;
};
}