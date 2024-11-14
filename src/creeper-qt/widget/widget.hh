#pragma once

#include <qboxlayout.h>
#include <qfile.h>
#include <qwidget.h>

#include "../setting/theme.hh"

namespace creeper {

template <class Widget>
    requires std::is_convertible_v<Widget*, QWidget*>
class Extension : public Widget {
public:
    template <typename... Args>
    explicit Extension(Args... args)
        : Widget(std::forward<Args>(args)...) {
        Theme::addReloadThemeHandler([this] { reloadTheme(); });
    }

    void moveCenter() {
        if (widget_->parentWidget() == nullptr)
            return;
        const auto parentCenter = widget_->parentWidget()->geometry().center();
        const auto frameCenter = widget_->frameGeometry().center();
        widget_->move(parentCenter - frameCenter);
    }

    void loadStyleFromFile(const QString& file) {
        QFile styleFile(file);
        styleFile.open(QFile::ReadOnly | QFile::Text);
        const auto style = styleFile.readAll();
        widget_->setStyleSheet(style);
        widget_->ensurePolished();
    }

    auto verticalWithSelf() {
        if (vertical_ == nullptr) {
            vertical_ = new QVBoxLayout;
            vertical_->setContentsMargins(0, 0, 0, 0);
            vertical_->addWidget(widget_);
        }
        return vertical_;
    }

    auto horizontalWithSelf() {
        if (horizontal_ == nullptr) {
            horizontal_ = new QHBoxLayout;
            horizontal_->setContentsMargins(0, 0, 0, 0);
            horizontal_->addWidget(widget_);
        }
        return horizontal_;
    }

    static void setRefreshIntervalMs(int ms) {
        refreshIntervalMs_ = ms;
    }

    virtual void reloadTheme() = 0;

protected:
    static inline int refreshIntervalMs_ = 10;

private:
    QWidget* widget_ = static_cast<QWidget*>(this);

    QVBoxLayout* vertical_ = nullptr;
    QHBoxLayout* horizontal_ = nullptr;
};

}