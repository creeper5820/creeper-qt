#pragma once

#include <qfile.h>
#include <qwidget.h>

namespace creeper {

template <class Widget>
    requires std::is_convertible_v<Widget*, QWidget*>
class Extension : public Widget {
public:
    template <typename... Args>
    explicit Extension(Args... args)
        : Widget(std::forward<Args>(args)...) {
    }

    void moveCenter() {
        if (widget->parentWidget() == nullptr)
            return;
        const auto parentCenter = widget->parentWidget()->geometry().center();
        const auto frameCenter = widget->frameGeometry().center();
        widget->move(parentCenter - frameCenter);
    }

    void loadStyleFromFile(const QString& file) {
        QFile styleFile(file);
        styleFile.open(QFile::ReadOnly | QFile::Text);
        const auto style = styleFile.readAll();
        widget->setStyleSheet(style);
        widget->ensurePolished();
    }

private:
    QWidget* widget = static_cast<QWidget*>(this);
};

}