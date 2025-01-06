#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/widget.hh>

class Workbench : public creeper::Extension<QWidget> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(Workbench)
public:
};