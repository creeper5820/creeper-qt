---
name: creeper-qt-development
description: creeper-qt 的代码与组件开发规范：组件结构、绘制约定、错误诊断、性能与 API。编写或审查 creeper-qt 库代码（新增/修改组件、DSL、绘制、主题）时使用。
---

# creeper-qt 代码与开发规范

## 错误诊断

- 优先通过 LSP 报错定位问题，而非完整编译。模板实例化导致编译开销很大、报错数量多且难以阅读，LSP 的单文件诊断更精准友好。
- `dsl_invocable` 与 `DSL::construct_with` 的 fallback `static_assert` 是用户体验的一部分，不要随意移除。
- 指向确切非法 prop 类型（包括嵌套在 tuple 内的值）的诊断是有价值的。
- 如果改进诊断，应保留尽早报告真实错误 prop 类型的能力，不要让模板展开退化成长篇构造函数错误。
- 在可行时，优先区分这些情况：不是 prop、是另一个组件的合法 prop、prop 的 `dsl_invoke` 目标不兼容。

## 性能预期

- Setter 风格的 prop 应保持轻薄且易于内联。其运行时成本应接近等价的手写 Qt setter 调用。
- 大多数 prop 检查成本发生在编译期。避免为普通 setter prop 添加运行时注册表或动态分发。
- 接受模板实例化、IDE 索引和诊断是该 DSL 的主要成本。
- 真正的运行时热点更可能出现在绘制、动画、布局、主题广播或 Qt 事件处理，而不是简单的 prop 转发。

## API 和文档

- 按每个组件自己的 `xxx::pro` 命名空间编写文档，不要要求用户去共享 scope（`api::scope::*`）命名空间中搜索。
- 添加组件时，让它的 `pro` 命名空间成为完整的用户入口，导入该组件支持的共享 prop 命名空间。
- 尽可能让 prop 名称贴近对应 Qt setter：`setFixedWidth()` 变为 `FixedWidth`，`setText()` 变为 `Text`，以此类推。
- 添加新 prop 时，确保它可以通过用户预期用于 IDE 补全的组件命名空间发现。

## 组件开发模式

- 开发新组件时，优先遵循 `text-fields.hh` / `text-fields.impl.hh` 的结构：组件类继承最贴近语义的 Qt 基类并继承 `DSL`（构造时调用 `construct_with`），组件自己的 `xxx::pro` 命名空间是声明式属性入口，绘制/事件细节放在 `.impl.hh` 或 `.cc`。
- 组件类应继承最贴近语义的 Qt 基类（实现细节可放在 `details` 命名空间），而不是重新发明行为；例如文本输入继续继承 `QLineEdit`，按钮继续基于 Qt 按钮能力，绘制和交互只补足本组件的样式差异。
- 组件类中集中保存组件运行状态，例如 hover、focus、disabled、error、checked、动画值、颜色 token、尺寸 token、缓存字体等；这些状态应最终驱动 Qt setter、绘制树或主题响应。
- 对外暴露给 prop 使用的接口应是清晰的 setter，例如 `setLabelText()`、`setMeasurements()`、`loadColorScheme()`、`bindThemeManager()`；不要让 prop 直接改内部字段。
- 实现逻辑优先放进 `.cc`（pimpl 的 `Impl` 定义也在 `.cc`），`.hh` 只声明组件形状、状态结构和 prop 表面，避免把实现逻辑塞进公开头文件的声明式入口。

典型组件骨架：

```cpp
// 组件：继承最贴近语义的 Qt 基类，并继承 DSL
namespace creeper {

class Xxx : public QWidget, public DSL {
    CREEPER_PIMPL_DEFINITION(Xxx);

public:
    explicit Xxx(auto&&... props)
        : Xxx { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme&);
    void bindThemeManager(ThemeManager&);

    void setLabelText(const QString&);

protected:
    void paintEvent(QPaintEvent*) override;
};

// 声明式属性入口：本组件新增 prop + 适用的共享 scope
namespace xxx::pro {

using LabelText = ForwardProp<&Xxx::setLabelText>;

using namespace api::scope::common;
using namespace api::scope::theme;
using namespace api::scope::widget;
}

}
```

- 组件自己的 `xxx::pro` 命名空间必须是用户入口，而不是只放本组件新增 prop；如果组件支持 `api::scope::widget` 或 `api::scope::theme`，就在本组件 `pro` 中导入它们，让 IDE 补全从一个命名空间开始。
- 简单 prop 优先用 `ForwardProp` 转发到 setter，保持运行时成本接近手写 setter；只有需要设置多个字段、支持多种构造来源或包含额外逻辑时，才手写带隐藏友元 `dsl_invoke` 的 prop。
- 从通用 prop 模板实例化本组件 prop 时，让 prop 的 `dsl_invoke` 作用于本组件类型；不要直接要求用户混用基础组件命名空间中的 prop 名称。
- prop 的 `dsl_invoke` 只做声明式配置，不应启动复杂业务流程；复杂状态应先被封装成组件 setter 或内部方法，再由 prop 调用。
- 转发到 `Impl` 的实现不含逻辑（不在转发层做 `update()`、lambda、参数改写）；逻辑放进 `Impl`，参数原样转发。

## 声明式绘制

绘制函数先更新组件状态，再集中计算尺寸、padding、位置、动画进度、字体和文本测量，最后用一棵对象树表达整个画面：

```cpp
// 具体接口看 `creeper-qt/details/drawable/core.hh`
auto paint_xxx(QPaintEvent*) -> void {
    using namespace drawable; ///< 一般放该 Scope 的首行

    // 提前计算好绘制过程中需要的数值
    const auto content_origin = ...;
    const auto show_ellipse   = ...;

    // 声明式描述一颗绘制的对象树
    auto painter = QPainter { &self };

    DrawHere {
        painter,
        Rect {
          .size  = self.size() ,
          .color = colors.background,
        },
        Box {
          {
            .size  = self.size(),
            .align = Qt::AlignCenter,
          },
          Surface {
            {
              .size   = measurements.content_size,
              .origin = content_origin,
            },
            Rect { ///< 不同参数组用空行划分，增加可读性
              .size  = measurements.content_size,
              .color = colors.container,

              .outline_color = colors.outline,
              .outline_width = measurements.outline_width,

              .radius_tl = measurements.radius,
              .radius_tr = measurements.radius,
              .radius_br = measurements.radius,
              .radius_bl = measurements.radius,
            },
            If {
              show_ellipse,
              Ellipse {
                .size  = measurements.overlay_size,
                .color = colors.state_layer,
              },
            },
          },
        },
    };
}
```

### 命名与构造

- 形状主体颜色使用 `color`；描边使用对称的 `outline_color / outline_width`。
- 四角圆角使用 `radius_tl / radius_tr / radius_br / radius_bl`，按顺时针方向阅读。
- 返回 UI 对象的自定义类型采用大写开头。优先继承现有节点，并通过构造函数参数完成配置，而不是编写小写工厂函数。
- UI 对象统一使用 `{}` 构造。
- `State` 等具名配置必须展开为多行，不写成单行。
- 无名构造参数只要求使用 `{}`，不强制添加具名字段或参数末尾逗号。
- 只有一个条件时不要使用提前返回；使用正向 `if` 或条件表达式，让主流程保持直观。

开发组件时的检查清单：

- 公开 prop 是否都能从组件自己的 `xxx::pro` 命名空间补全到。
- 组件 `pro` 是否导入了它支持的全部共享 scope（`api::scope::*`）。
- prop 是否最终调用明确 setter，而不是直接接触内部字段。
- 是否保留 Qt 父子所有权、事件投递、布局和信号/槽语义。
- 是否优先使用 LSP 单文件诊断确认模板错误，而不是一开始就完整编译。
