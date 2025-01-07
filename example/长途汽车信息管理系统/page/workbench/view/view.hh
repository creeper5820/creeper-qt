#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/basic-shape.hh>

class ViewArea : public creeper::RoundedRectangle {
    CREEPER_WIDGET_PIMPL_DEFINTION(ViewArea);
    Q_OBJECT

public:
    void changeView(std::size_t index);
};