#pragma once
#include "creeper-qt/utility/painter/common.hh"

namespace creeper::painter {

// 核心容器结构体，现在继承自 Impl，使其满足 drawable_trait (假设 Impl 继承了所需的属性)
template <class Impl, drawable_trait... Ts>
struct Container : public Impl {
    std::tuple<std::decay_t<Ts>...> drawable;

    // 唯一构造函数：接受 Impl 实例和可变参数包
    constexpr explicit Container(const Impl& impl, Ts&&... drawable)
        : Impl { impl }
        , drawable { std::make_tuple(std::forward<Ts>(drawable)...) } { }

    auto operator()(qt::painter& painter)
        requires(std::invocable<Ts, qt::painter&> && ...)
    {
        render(painter);
    }
    auto render(qt::painter& painter) noexcept
        requires(std::invocable<Ts, qt::painter&> && ...)
    {
        constexpr auto has_unique = requires { //
            static_cast<Impl&>(*this).unique_render(painter, drawable);
        };
        if constexpr (has_unique) {
            static_cast<Impl&>(*this).unique_render(painter, drawable);
        } else {
            Impl::make_layout(drawable);

            auto f = [&](auto&... d) { (d(painter), ...); };
            std::apply(std::move(f), drawable);
        }
    }
};

// ----------------------------------------------------------------------
// 布局实现基类
// ----------------------------------------------------------------------

struct MakeLayoutFunction {
    template <drawable_trait... Ts>
    auto make_layout(this auto& self, std::tuple<Ts...>& drawable) {
        std::apply([&self](auto&... d) { ((self.make(d)), ...); }, drawable);
    }
};

// ----------------------------------------------------------------------
// SurfaceImpl (仅平移)
// ----------------------------------------------------------------------

struct SurfaceImpl : public MakeLayoutFunction, ContainerProps {
    constexpr explicit SurfaceImpl(const qt::size& size, const qt::point& origin = {})
        : ContainerProps {
            .size   = size,
            .origin = origin,
        } { }
    auto make(drawable_trait auto& drawable) {
        const auto& container_origin = origin;

        auto& drawable_origin = drawable.origin;
        drawable_origin.setX(container_origin.x() + drawable_origin.x());
        drawable_origin.setY(container_origin.y() + drawable_origin.y());
    };
};

// ----------------------------------------------------------------------
// BufferImpl
// ----------------------------------------------------------------------

struct BufferImpl : public MakeLayoutFunction, ContainerProps {
    mutable QPixmap buffer;

    constexpr explicit BufferImpl(const qt::size& size, const qt::point& origin = { 0, 0 })
        : ContainerProps {
            .size   = size,
            .origin = origin,
        } { }

    auto make(drawable_trait auto& drawable) {
        const auto& container_origin = origin;
        drawable.origin.setX(container_origin.x() + drawable.origin.x());
        drawable.origin.setY(container_origin.y() + drawable.origin.y());
    };

    template <drawable_trait... Ts>
    auto unique_render(qt::painter& main_painter, std::tuple<Ts...>& drawable) noexcept {
        make_layout(drawable);

        if (buffer.size() != size || buffer.isNull()) {
            buffer = QPixmap(size.width(), size.height());
            buffer.fill(Qt::transparent);
        }
        buffer.fill(Qt::transparent);

        auto buffer_painter = qt::painter { &buffer };
        buffer_painter.translate(-origin.x(), -origin.y());

        const auto f = [&](auto&... args) {
            (
                [&]() {
                    buffer_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                    args(buffer_painter);
                }(),
                ...);
        };
        std::apply(std::move(f), drawable);
        buffer_painter.end();

        main_painter.drawPixmap(origin, buffer);
    }
};

// ----------------------------------------------------------------------
// BoxImpl (居中对齐)
// ----------------------------------------------------------------------

struct BoxImpl : public MakeLayoutFunction, ContainerProps {

    constexpr explicit BoxImpl(
        const qt::size& size, const qt::align& align, const qt::point& origin = {})
        : ContainerProps {
            .size   = size,
            .align  = align,
            .origin = origin,
        } { }

    auto make(drawable_trait auto& drawable) {
        const auto container_align  = align;
        const auto container_size   = size;
        const auto container_origin = origin;

        auto& drawable_origin = drawable.origin;
        auto& drawable_size   = drawable.size;

        const auto container_w = container_size.width();
        const auto container_h = container_size.height();

        if (container_align & Qt::AlignRight) {
            drawable_origin.setX(container_origin.x() + container_w - drawable_size.width());
        } else if (container_align & Qt::AlignHCenter) {
            const auto dx = (container_w - drawable_size.width()) / 2;
            drawable_origin.setX(container_origin.x() + dx);
        } else {
            drawable_origin.setX(container_origin.x());
        }

        if (container_align & Qt::AlignBottom) {
            drawable_origin.setY(container_origin.y() + container_h - drawable_size.height());
        } else if (container_align & Qt::AlignVCenter) {
            const auto dy = (container_h - drawable_size.height()) / 2;
            drawable_origin.setY(container_origin.y() + dy);
        } else {
            drawable_origin.setY(container_origin.y());
        }
    };
};

// ----------------------------------------------------------------------
// RowImpl (横向流式布局)
// ----------------------------------------------------------------------

struct RowImpl : public MakeLayoutFunction, ContainerProps {
    // 主轴对齐 (Horizontal)
    const qt::align main_align;

    constexpr explicit RowImpl(
        const qt::size& size,
        const qt::align& main_align = Qt::AlignLeft,   // 主轴对齐：AlignLeft/AlignRight/AlignHCenter
        const qt::align& cross_align = Qt::AlignVCenter, // 非主轴对齐：AlignTop/AlignBottom/AlignVCenter
        const qt::point& origin = {})
        : ContainerProps {
              .size   = size,
              .align  = cross_align, // ContainerProps::align 存储非主轴对齐
              .origin = origin,
          }
        , main_align(main_align) // 存储主轴对齐
    { }

    mutable int current_x = 0;

    template <drawable_trait... Ts>
    auto make_layout(this auto& self, std::tuple<Ts...>& drawable) {
        // 1. 计算主轴总尺寸 (Total Width)
        int total_width = 0;
        std::apply(
            [&total_width](auto&... d) { ((total_width += d.size.width()), ...); }, drawable);

        // 2. 计算主轴偏移 (Main Axis Offset)
        int initial_x_offset      = 0;
        const int remaining_space = self.size.width() - total_width;

        if (remaining_space > 0) {
            if (self.main_align & Qt::AlignRight) {
                initial_x_offset = remaining_space;
            } else if (self.main_align & Qt::AlignHCenter) {
                initial_x_offset = remaining_space / 2;
            }
        }

        // 3. 设置起始 X 坐标
        self.current_x = self.origin.x() + initial_x_offset;

        // 4. 应用布局到每个元素
        std::apply([&self](auto&... d) { ((self.make(d)), ...); }, drawable);
    }

    auto make(drawable_trait auto& drawable) {
        const auto container_cross_align = align; // 非主轴对齐 (垂直)
        const auto container_size        = size;
        const auto container_origin      = origin;

        auto& drawable_origin = drawable.origin;
        const auto drawable_h = drawable.size.height();

        // 1. 主轴布局 (X 坐标累加)
        drawable_origin.setX(current_x);
        current_x += drawable.size.width();

        // 2. 非主轴对齐 (垂直对齐)
        if (container_cross_align & Qt::AlignBottom) {
            drawable_origin.setY(container_origin.y() + container_size.height() - drawable_h);
        } else if (container_cross_align & Qt::AlignVCenter) {
            const auto dy = (container_size.height() - drawable_h) / 2;
            drawable_origin.setY(container_origin.y() + dy);
        } else { // 默认 AlignTop
            drawable_origin.setY(container_origin.y());
        }
    };
};

// ----------------------------------------------------------------------
// ColImpl (垂直流式布局)
// ----------------------------------------------------------------------

struct ColImpl : public MakeLayoutFunction, ContainerProps {
    // 主轴对齐 (Vertical)
    const qt::align main_align;

    constexpr explicit ColImpl(
        const qt::size& size,
        const qt::align& main_align = Qt::AlignTop,     // 主轴对齐：AlignTop/AlignBottom/AlignVCenter
        const qt::align& cross_align = Qt::AlignHCenter, // 非主轴对齐：AlignLeft/AlignRight/AlignHCenter
        const qt::point& origin = {})
        : ContainerProps {
              .size   = size,
              .align  = cross_align, // ContainerProps::align 存储非主轴对齐
              .origin = origin,
          }
        , main_align(main_align) // 存储主轴对齐
    { }

    mutable int current_y = 0;

    template <drawable_trait... Ts>
    auto make_layout(this auto& self, std::tuple<Ts...>& drawable) {
        // 1. 计算主轴总尺寸 (Total Height)
        int total_height = 0;
        std::apply(
            [&total_height](auto&... d) { ((total_height += d.size.height()), ...); }, drawable);

        // 2. 计算主轴偏移 (Main Axis Offset)
        int initial_y_offset      = 0;
        const int remaining_space = self.size.height() - total_height;

        if (remaining_space > 0) {
            if (self.main_align & Qt::AlignBottom) {
                initial_y_offset = remaining_space;
            } else if (self.main_align & Qt::AlignVCenter) {
                initial_y_offset = remaining_space / 2;
            }
        }

        // 3. 设置起始 Y 坐标
        self.current_y = self.origin.y() + initial_y_offset;

        // 4. 应用布局到每个元素
        std::apply([&self](auto&... d) { ((self.make(d)), ...); }, drawable);
    }

    auto make(drawable_trait auto& drawable) {
        const auto container_cross_align = align; // 非主轴对齐 (水平)
        const auto container_size        = size;
        const auto container_origin      = origin;

        auto& drawable_origin = drawable.origin;
        const auto drawable_w = drawable.size.width();

        // 1. 主轴布局 (Y 坐标累加)
        drawable_origin.setY(current_y);
        current_y += drawable.size.height();

        // 2. 非主轴对齐 (水平对齐)
        if (container_cross_align & Qt::AlignRight) {
            drawable_origin.setX(container_origin.x() + container_size.width() - drawable_w);
        } else if (container_cross_align & Qt::AlignHCenter) {
            const auto dx = (container_size.width() - drawable_w) / 2;
            drawable_origin.setX(container_origin.x() + dx);
        } else { // 默认 AlignLeft
            drawable_origin.setX(container_origin.x());
        }
    };
};

// ----------------------------------------------------------------------
// 通用 Container 推导指引 (用于简化用户代码)
// ----------------------------------------------------------------------

template <typename... Ts>
Container(const SurfaceImpl& impl, Ts&&... args) -> Container<SurfaceImpl, Ts...>;

template <typename... Ts>
Container(const BufferImpl& impl, Ts&&... args) -> Container<BufferImpl, Ts...>;

template <typename... Ts>
Container(const BoxImpl& impl, Ts&&... args) -> Container<BoxImpl, Ts...>;

template <typename... Ts>
Container(const RowImpl& impl, Ts&&... args) -> Container<RowImpl, Ts...>;

template <typename... Ts>
Container(const ColImpl& impl, Ts&&... args) -> Container<ColImpl, Ts...>;

// ----------------------------------------------------------------------
// Paint 类型导出
// ----------------------------------------------------------------------

namespace Paint {

    template <drawable_trait... Ts>
    using Surface = Container<SurfaceImpl, Ts...>;

    template <drawable_trait... Ts>
    using Buffer = Container<BufferImpl, Ts...>;

    template <drawable_trait... Ts>
    using Box = Container<BoxImpl, Ts...>;

    template <drawable_trait... Ts>
    using Row = Container<RowImpl, Ts...>;

    template <drawable_trait... Ts>
    using Col = Container<ColImpl, Ts...>;
}

} // namespace creeper::painter
