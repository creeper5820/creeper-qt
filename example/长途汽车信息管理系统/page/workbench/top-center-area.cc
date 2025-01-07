#include "top-area.hh"

struct TopCenterArea::Impl { };

TopCenterArea::TopCenterArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) { }

TopCenterArea::~TopCenterArea() { delete pimpl_; }