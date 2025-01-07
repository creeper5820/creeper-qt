#pragma once

#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/image.hh>

class MapCheckView : public creeper::Image {
public:
    explicit MapCheckView() {
        setSizePolicy(QSizePolicy::Policy::Expanding, //
            QSizePolicy::Policy::Expanding);
        Image::Style {
            .pixmap = QPixmap(":/image/map.png"),
            .radius = 10,
            .borderWidth = 2,
            .borderColor = creeper::color::grey400,
            .fitness = creeper::ImageFitness::Cover,
        }(*this);
    }
};