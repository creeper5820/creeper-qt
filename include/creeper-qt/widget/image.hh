#pragma once

#include "creeper-qt/widget/widget.hh"
#include <creeper-qt/setting/theme.hh>
#include <creeper-qt/utility/pimpl.hh>
#include <qabstractbutton.h>

namespace creeper {

class Image : public QAbstractButton {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(Image)
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(Image, QAbstractButton)

public:
    enum class Fitness { None, Fill, Contain, Cover, ScaleDown };

    explicit Image(const QString& path, QWidget* parent = nullptr);
    explicit Image(const QPixmap& pixmap, QWidget* parent = nullptr);

    Image& setPixmap(const QPixmap& pixmap);
    Image& setPixmap(const QString& path);

    Image& setRadius(double radius);
    Image& setBorderWidth(double width);
    Image& setBorderColor(QColor color);
    Image& setBackground(QColor color);
    Image& setFitness(Fitness fitness);

protected:
    void paintEvent(QPaintEvent* event) override;
};

}