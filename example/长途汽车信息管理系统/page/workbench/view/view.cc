#include "view.hh"
#include "about-us.hh"
#include "dashboard.hh"
#include "database.hh"
#include "map-check.hh"

#include <qstackedwidget.h>

using namespace creeper;

struct ViewArea::Impl {
    /// @note 程序退出后 QStackedWidget 的析构会把子控件一同析构，
    ///       如果 stack 声明在子控件后面，Impl 会先将子控件析构，
    ///       等 stack 析构时，由于没有做空指针处理，会导致段错误
    QStackedWidget stack;

    MapCheckView mapView;
    DashboardView dashboard;
    DataBaseView database;
    AboutUsView aboutUs;

    QGridLayout layout;
};

ViewArea::ViewArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) {
    auto& pimpl = *pimpl_;

    pimpl.stack.addWidget(&pimpl.dashboard);
    pimpl.stack.addWidget(&pimpl.database);
    pimpl.stack.addWidget(&pimpl.mapView);
    pimpl.stack.addWidget(&pimpl.aboutUs);
    pimpl.stack.setCurrentIndex(0);

    pimpl.layout.setSpacing(0);
    pimpl.layout.setMargin(0);
    pimpl.layout.addWidget(&pimpl.stack);
    setLayout(&pimpl.layout);
}

ViewArea::~ViewArea() { delete pimpl_; }

void ViewArea::changeView(std::size_t index) { //
    pimpl_->stack.setCurrentIndex(index);
}