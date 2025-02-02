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
    /// @todo 这里重复捕获 this 指针，有一些额外开销，日后有时间记得修复
    /// @note widget 被传入作为键值，而 function 里也会捕获一个 widget 的指针
    static void addReloadThemeHandler(QWidget* widget, std::function<void(void)> handler);
    static void removeReloadThemeHandler(QWidget* widget);

    /// @brief For Debug
    static const void printYamlString();

private:
    struct Impl;
    static inline Impl* pimpl_;
};

/// @brief 样式辅助工具
template <typename Widget> struct AbstractWidgetStyle {
    virtual void apply(Widget& widget) = 0;

    void operator()(Widget& widget) { apply(widget); }

    Widget* create() {
        auto widget = new Widget;
        apply(*widget);
        return widget;
    }
    Widget* operator()() { return create(); }
};

}