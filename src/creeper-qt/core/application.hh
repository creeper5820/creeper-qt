#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/wrapper/dsl.hh"
#include <qapplication.h>
#include <qcoreapplication.h>

namespace creeper::app::pro {

struct Complete {

    int& argument_count;
    char** argument_array;
    int application_flags;

    explicit Complete(int& argc, char* argv[], int flags = ::QCoreApplication::ApplicationFlags)
        : argument_count { argc }
        , argument_array { argv }
        , application_flags { flags } { }

    friend auto dsl_invoke(auto&, const Complete& prop) noexcept -> void {
        new ::QApplication {
            prop.argument_count,
            prop.argument_array,
            prop.application_flags,
        };
    }
};

struct Attribute {

    ::Qt::ApplicationAttribute attribute;
    bool on;

    explicit Attribute(::Qt::ApplicationAttribute attribute, bool on = true) noexcept
        : attribute { attribute }
        , on { on } { }

    friend auto dsl_invoke(auto&, const Attribute& prop) noexcept -> void {
        ::QApplication::setAttribute(prop.attribute, prop.on);
    }
};

}

namespace creeper::app {

class Application : public DSL {
public:
    explicit Application(auto&&... props) {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

using init = Application;

inline auto exec() { return ::QApplication::exec(); }
inline auto quit() { return ::QApplication::quit(); }

inline auto focusWidget() { return ::QApplication::focusWidget(); }
inline auto focusObject() { return ::QApplication::focusObject(); }
}
