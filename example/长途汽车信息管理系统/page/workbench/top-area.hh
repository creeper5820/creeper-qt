#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/basic-shape.hh>

class TopLeftArea : public creeper::RoundedRectangle {
    CREEPER_WIDGET_PIMPL_DEFINTION(TopLeftArea);
    Q_OBJECT

public:
    void setFileName(const QString& name);
};

class TopCenterArea : public creeper::RoundedRectangle {
    CREEPER_WIDGET_PIMPL_DEFINTION(TopCenterArea);
    Q_OBJECT
};

class TopRightArea : public creeper::RoundedRectangle {
    CREEPER_WIDGET_PIMPL_DEFINTION(TopRightArea);
    Q_OBJECT
};