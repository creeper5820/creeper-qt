#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include <qapplication.h>
#include <qcoreapplication.h>

namespace creeper::app::pro {

using Token = common::Token<QApplication>;

struct Complete : Token {

    int& argument_count;
    char** argument_array;
    int application_flags;

    explicit Complete(int& argc, char* argv[], int flags = QCoreApplication::ApplicationFlags)
        : argument_count { argc }
        , argument_array { argv }
        , application_flags { flags } { }

    void apply(auto&) const noexcept {
        new ::QApplication {
            argument_count,
            argument_array,
            application_flags,
        };
    }
};

struct Attribute : Token {
    using Enum = Qt::ApplicationAttribute;

    Qt::ApplicationAttribute attribute;
    bool on;

    explicit Attribute(Qt::ApplicationAttribute attribute, bool on = true) noexcept
        : attribute { attribute }
        , on { on } { }

    void apply(auto&) const noexcept { QApplication::setAttribute(attribute, on); }
};

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
}
namespace creeper::app {

struct Application { };
using init = Declarative<Application, pro::checker>;

inline auto exec() { return ::QApplication::exec(); }
inline auto quit() { return ::QApplication::quit(); }

inline auto focus_widget() { return ::QApplication::focusWidget(); }
inline auto focus_object() { return ::QApplication::focusObject(); }
}
