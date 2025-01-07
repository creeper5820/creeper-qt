#include "view.hh"

#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/image.hh>
#include <qboxlayout.h>
#include <qstackedwidget.h>

using namespace creeper;

class MapCheckView : public Image {
public:
    explicit MapCheckView() {
        setSizePolicy(QSizePolicy::Policy::Expanding, //
            QSizePolicy::Policy::Expanding);
        Image::Style {
            .pixmap = QPixmap(":/image/map.png"),
            .radius = 10,
            .borderWidth = 2,
            .borderColor = color::grey400,
            .fitness = ImageFitness::Cover,
        }(*this);
    }
};

class DashboardView : public RoundedRectangle { };

class DataBaseView : public RoundedRectangle {
public:
    explicit DataBaseView() { }

private:
};

class AboutUsView : public RoundedRectangle { };

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