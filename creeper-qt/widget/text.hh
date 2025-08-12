#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/widget/widget.hh"
#include <qlabel.h>

namespace creeper::text::pro {
using Token = common::Token<QLabel>;

struct Text : Token, QString {
    using QString::QString;
    using QString::operator=;
    auto apply(auto& self) const noexcept -> void
        requires requires { self.setText(QString {}); }
    {
        self.setText(*this);
    }
};

template <class T>
concept concept_ = std::derived_from<T, Token> || widget::pro::concept_<T>;

CREEPER_DEFINE_CHECK(concept_);
using namespace widget::pro;
}

namespace creeper {
using Text = Declarative<QLabel, text::pro::checker>;
}
