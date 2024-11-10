#pragma once

#include "setting/color/common-white.hh"

#include <atomic>
#include <qstring.h>

namespace creeper {

class Theme {
public:
    static inline bool occupied() {
        return occupied_;
    }
    static inline void setOccupied(bool occupied) {
        occupied_ = occupied;
    }
    static inline void setTheme(const QString& name) {
        theme_ = name;
    }
    static inline const QString theme() {
        return theme_;
    }
    static inline const QString qss(const QString& name) {
        return ":" + theme() + "/qss/" + name + ".qss";
    }
    static inline auto color(const QString& name) {
        if (name == "primary050")
            return primary050;
        else if (name == "primary100")
            return primary100;
        else if (name == "primary200")
            return primary200;
        else if (name == "primary300")
            return primary300;
        else if (name == "primary400")
            return primary400;
        else if (name == "primary500")
            return primary500;
        else if (name == "primary600")
            return primary600;
        else if (name == "primary700")
            return primary700;
        else if (name == "primary800")
            return primary800;
        else if (name == "primary900")
            return primary900;
        return 0x00000;
    }

    constexpr static inline auto CommonWhite = "common-white";
    constexpr static inline auto CommonBlack = "common-black";

private:
    static inline QString theme_ = CommonWhite;
    static inline std::atomic<bool> occupied_ = false;
};

}