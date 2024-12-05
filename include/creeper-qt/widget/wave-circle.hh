#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {

class WaveCircle : public Extension<QWidget> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(WaveCircle);

public:
    WaveCircle& setFlange(uint flange);
    WaveCircle& setRadius(double radius);
    WaveCircle& setFlangeRatio(double ratio);
    WaveCircle& setFlangeRadius(double radius);
    WaveCircle& setLineColor(uint32_t color);
    WaveCircle& setBackground(uint32_t color);
    WaveCircle& setLineWidth(double width);
    void reloadTheme() override;

protected:
    void paintEvent(QPaintEvent* event) override;
};
}