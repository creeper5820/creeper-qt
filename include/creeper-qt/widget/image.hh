#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <qabstractbutton.h>

namespace creeper {

enum class ImageFitness { None, Fill, Contain, Cover, ScaleDown };

class Image : public QAbstractButton {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(Image)

public:
    explicit Image(const QString& path, QWidget* parent = nullptr);
    explicit Image(const QPixmap& pixmap, QWidget* parent = nullptr);

    void setPixmap(const QPixmap& pixmap);
    void setPixmap(const QString& path);

    void setRadius(double radius);
    void setBorderWidth(double width);
    void setBorderColor(QColor color);
    void setBackground(QColor color);
    void setFitness(ImageFitness fitness);

protected:
    void paintEvent(QPaintEvent* event) override;
};

}