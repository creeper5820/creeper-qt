#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/widget.hh>
#include <qscrollarea.h>

class OperatorArea : public creeper::Extension<QScrollArea> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(OperatorArea)
public:
    void reloadTheme() override;
};