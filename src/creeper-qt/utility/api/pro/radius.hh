#pragma once

namespace creeper::api::pro {

/// 设置圆角（NXNY）
/// 要求组件实现：void setRadiusNxNy(double)
struct RadiusNxNy {
    double value;

    constexpr explicit RadiusNxNy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusNxNy& prop) -> void {
        widget.setRadiusNxNy(prop.value);
    }
};

/// 设置圆角（PXPY）
/// 要求组件实现：void setRadiusPxPy(double)
struct RadiusPxPy {
    double value;

    constexpr explicit RadiusPxPy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusPxPy& prop) -> void {
        widget.setRadiusPxPy(prop.value);
    }
};

/// 设置圆角（NXPY）
/// 要求组件实现：void setRadiusNxPy(double)
struct RadiusNxPy {
    double value;

    constexpr explicit RadiusNxPy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusNxPy& prop) -> void {
        widget.setRadiusNxPy(prop.value);
    }
};

/// 设置圆角（PXNY）
/// 要求组件实现：void setRadiusPxNy(double)
struct RadiusPxNy {
    double value;

    constexpr explicit RadiusPxNy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusPxNy& prop) -> void {
        widget.setRadiusPxNy(prop.value);
    }
};

/// 设置圆角（X方向）
/// 要求组件实现：void setRadiusX(double)
struct RadiusX {
    double value;

    constexpr explicit RadiusX(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusX& prop) -> void {
        widget.setRadiusX(prop.value);
    }
};

/// 设置圆角（Y方向）
/// 要求组件实现：void setRadiusY(double)
struct RadiusY {
    double value;

    constexpr explicit RadiusY(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusY& prop) -> void {
        widget.setRadiusY(prop.value);
    }
};

/// 设置通用圆角
/// 要求组件实现：void setRadius(double)
struct Radius {
    double value;

    constexpr explicit Radius(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Radius& prop) -> void {
        widget.setRadius(prop.value);
    }
};

}
