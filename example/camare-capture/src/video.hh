#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/widget.hh>

class Video : public creeper::Extension<QWidget> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(Video)
public:
    void reloadTheme() override;
    void connectCamera(int index);
    bool connected() const;

protected:
    void paintEvent(QPaintEvent* event) override;
};