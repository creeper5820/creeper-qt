#pragma once

#include "../widget/widget.hh"

#include <eigen3/Eigen/Eigen>

namespace creeper {

class ScallopClock : Extension<QWidget> {
    Q_OBJECT
public:
    explicit ScallopClock(QWidget* parent = nullptr)
        : Extension(parent) {
    }

private:
};

}
