#pragma once

#include "utility/theme/color-scheme.hh"
#include "utility/wrapper/pimpl.hh"

namespace creeper::util::theme {

struct ThemePack {
    ColorScheme color_scheme;
};

class ThemeManager {
    CREEPER_PIMPL_DEFINTION(ThemeManager)
};

}
