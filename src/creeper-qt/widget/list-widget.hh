#pragma once

#include <qboxlayout.h>
#include <qfile.h>
#include <qicon.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qstyle.h>

#include "../setting/style-template.hh"
#include "../setting/theme.hh"
#include "../widget/switch-button.hh"
#include "../widget/widget.hh"

namespace creeper {
class CustomItemWidgetInterface : public Extension<QWidget>, public QListWidgetItem { };

class SwitchAndLabelItem : public CustomItemWidgetInterface {
public:
    explicit SwitchAndLabelItem(const QString& label) {
        button_ = new SwitchButton;
        button_->setFixedSize({ 50, 30 });

        auto labelWidget_ = new QLabel;
        labelWidget_->setText(label);
        labelWidget_->setFont(QFont("monospace", 12, QFont::Normal));
        labelWidget_->setStyleSheet("color: #575757;");

        horizonLayout_ = new QHBoxLayout;
        horizonLayout_->setAlignment(Qt::AlignLeft);
        horizonLayout_->addLayout(button_->verticalWithSelf());
        horizonLayout_->addWidget(labelWidget_);

        Extension::setLayout(horizonLayout_);
        QListWidgetItem::setSizeHint(QSize(1, 80));
    }

    void reloadTheme() override { }

private:
    QHBoxLayout* horizonLayout_;
    QLabel* labelWidget_;
    SwitchButton* button_;
};

class ListWidget : public Extension<QListWidget> {
public:
    explicit ListWidget(QWidget* parent = nullptr)
        : Extension(parent) {
        reloadTheme();
    }

    void addCustomItem(CustomItemWidgetInterface* item) {
        QListWidget::addItem(item);
        QListWidget::setItemWidget(item, item);
    }

    void addSwitchAndLabel(const QString& label) {
        auto customItem = new SwitchAndLabelItem(label);
        addCustomItem(static_cast<CustomItemWidgetInterface*>(customItem));
    }

    void addSimpleLabel(const QString& label) {
        auto item = new QListWidgetItem;
        item->setText(label);
        item->setFont(QFont("monospace", 12, QFont::Normal));
        QListWidget::addItem(item);
    }

    void reloadTheme() override {
        Extension::setStyleSheet(QString(style::ListWidget)
                .arg("#f7f7f7", "#ece7f4", "#cfc2e7", "#ece7f4"));
    }
};

}