#pragma once

#include "creeper-qt/setting/style-template.hh"
#include "creeper-qt/widget/label.hh"
#include "creeper-qt/widget/switch-button.hh"

#include <qlabel.h>
#include <qpainter.h>

namespace creeper {

class SwitchCard : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit SwitchCard(QWidget* parent = nullptr)
        : Extension(parent) {

        label_.setText(style::textForExample);

        switchButton_.setFixedSize({ 60, 30 });

        auto bottomLayout = switchButton_.horizontalWithSelf();
        bottomLayout->setAlignment(Qt::AlignRight);

        auto mainLayout = new QVBoxLayout;
        mainLayout->addWidget(&label_);
        mainLayout->addLayout(bottomLayout);

        Extension::setContentsMargins(10, 10, 10, 10);
        Extension::setLayout(mainLayout);

        reloadTheme();
    }

    void reloadTheme() override {
        background_ = Theme::color("primary050");
        label_.setWordWrap(true);
        label_.setAlignment(Qt::AlignCenter);
        label_.setStyleSheet("color: #575757;");
        label_.setFont(font());
    }

    void setText(const QString& text) { label_.setText(text); }

protected:
    void paintEvent(QPaintEvent* event) override {
        const auto width = Extension::width();
        const auto height = Extension::height();

        auto painter = QPainter(this);
        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush({ background_ });
        painter.drawRoundedRect(0, 0, width, height, 0.1 * height, 0.1 * height);
    }

private:
    ConcaveSwitchButton switchButton_ { this };
    Label label_ { this };
    uint32_t background_;
};

}