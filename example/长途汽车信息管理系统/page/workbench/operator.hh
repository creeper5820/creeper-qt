#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/basic-shape.hh>

class OperatorArea : public creeper::RoundedRectangle {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(OperatorArea)

signals:
    void changeView(std::size_t index);
    void logout();
};