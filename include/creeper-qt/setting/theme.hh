#pragma once

#include <yaml-cpp/yaml.h>

#include <qfont.h>
#include <qstring.h>

namespace creeper {

class Theme {
    Q_DISABLE_COPY_MOVE(Theme)
public:
    struct common {
        constexpr static inline auto purple = "common-purple";
        constexpr static inline auto green = "common-green";
        constexpr static inline auto grey = "common-grey";
        constexpr static inline auto blue = "common-blue";
    };

    static bool occupied();
    static void setOccupied(bool occupied);
    static void setTheme(const QString& name);

    static const QString theme();

    static const QString font(const char* name);
    static const uint32_t color(const char* name);

    static void reloadTheme();
    static void addReloadThemeHandler(std::function<void(void)> handler);

    /// @brief For Debug
    static const void printYamlString();

private:
    struct Impl;
    static inline Impl* pimpl_;
};
}