#pragma once

#include "utility/single.hh"

#include <qstring.h>

namespace creeper {

class Theme : public Singleton<Theme> {
public:
    SingletonConstructor(Theme) { }
    SingletonDestructor(Theme) { }
    RemoveCopyConstructor(Theme);

    void setTheme(const QString& name) {
        this->theme_ = name;
    }
    QString theme() const {
        return this->theme_;
    }
    QString qss(const QString& name) const {
        return ":" + this->theme() + "/qss/" + name + ".qss";
    }

private:
    QString theme_ = "common-white";
};

}