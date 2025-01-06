#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/basic-shape.hh>

class TopArea : public creeper::Rectangle {
    CREEPER_WIDGET_PIMPL_DEFINTION(TopArea);
    Q_OBJECT

public:
    void setFileName(const QString& name);
};
