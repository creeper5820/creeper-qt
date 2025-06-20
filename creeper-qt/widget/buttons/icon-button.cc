#include "icon-button.hh"

#include <QtSvg/qsvgrenderer.h>

using namespace creeper::icon_button::internal;

struct IconButton::Impl { };

IconButton::IconButton()
    : pimpl(std::make_unique<Impl>()) { }

IconButton::~IconButton() = default;
