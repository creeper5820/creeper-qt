#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

#include <qevent.h>
#include <qsize.h>

namespace creeper {

class ConvexSlider : public Extension<QWidget> {
    CREEPER_WIDGET_PIMPL_DEFINTION(ConvexSlider)
    Q_OBJECT

public:
    void reloadTheme() override;

    ConvexSlider& setRange(double minimum, double maximum);
    ConvexSlider& setMinimum(double minimum);
    double minimum() const;
    ConvexSlider& setMaximum(double maximum);
    double maximum() const;
    ConvexSlider& setValue(double value);
    double value() const;

    ConvexSlider& resize(int width, int height);
    ConvexSlider& setFixedSize(int width, int height);
    ConvexSlider& setFixedSize(QSize size);
    ConvexSlider& setFixedWidth(int width);
    ConvexSlider& setFixedHeight(int height);

    ConvexSlider& setBlockThicknessRatio(double blockThicknessRatio);
    double blockThicknessRatio() const;
    ConvexSlider& setBlockLengthRatio(double ratio);
    double blockLengthRatio() const;
    ConvexSlider& setSliderSizeRatio(double ratio);
    double sliderSizeRatio() const;
    ConvexSlider& setSliderRoundRatio(double ratio);
    double sliderRoundRatio() const;

    ConvexSlider& setSliderColor(const QColor& color);
    QColor sliderColor() const;
    ConvexSlider& setRoundColor(const QColor& color);
    QColor roundColor() const;

    ConvexSlider& setBackgroundOpacity(double opacity);
    double backgroundOpacity() const;
    ConvexSlider& setBlockOpacity(double opacity);
    double blockOpacity() const;
    ConvexSlider& setBlockShadowOpacity(double opacity);
    double blockShadowOpacity() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};
}