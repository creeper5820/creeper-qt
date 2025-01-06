#include "operator.hh"

struct OperatorArea::Impl { };

OperatorArea::OperatorArea(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) { }

OperatorArea::~OperatorArea() { delete pimpl_; }

void OperatorArea::reloadTheme() { std::printf("OperatorArea reload theme\n"); }