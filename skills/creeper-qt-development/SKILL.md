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

绘制函数建议按固定顺序组织：

```cpp
auto paint_xxx(QPaintEvent*) -> void {
    const auto& measurements = this->measurements;
    const auto& color_tokens = get_color_tokens();

    update_component_status(...);

    using namespace painter;
    using namespace painter::common::pro;
    auto painter = qt::painter { &self };

    // 先集中计算尺寸、padding、位置、动画进度、字体和文本测量。

    Paint::Box {
        BoxImpl { self.size(), Qt::AlignCenter },
        Paint::Surface {
          SurfaceImpl { container_size },
          // 绘制节点树
        },
    }(painter);
}
```

- 绘制函数应先更新组件状态，再集中计算尺寸、padding、位置、动画进度、文本测量等缓存变量，最后用声明式 `Paint::*` 树表达绘制结构。
- 避免在 `Paint::*` 构造列表里穿插复杂计算；构造列表应尽量只呈现 UI 层级、绘制元素和必要 prop，复杂逻辑应在进入绘制树前完成并命名。
- 颜色选择优先通过 `get_color_tokens()` 之类的函数完成，让绘制树只消费当前状态下的 token，而不是在每个绘制节点里重复判断 focus/error/disabled。
- 尺寸选择优先来自 `Measurements` 或局部缓存变量；不要在多个绘制节点中重复硬编码同一个 magic number。
- 字体、文本宽度、动画位置、label 缩放等中间值应在绘制树外命名，保证声明式树读起来像结构描述，而不是计算过程。

`Paint::*` 绘制树职责划分：

- `Paint::Surface` 负责坐标偏移，适合把一组元素移动到同一个局部坐标系中。
- `Paint::Box` 负责对齐定位，适合把单个子节点放进指定大小区域内，如居中图标、定位 label 背景区域。
- `Paint::Row` 和 `Paint::Col` 负责流式布局，适合多个同级绘制节点按主轴排列。
- `Paint::Buffer` 负责离屏合成，适合需要透明混合、擦除、镂空、遮罩等效果的组合绘制。
- `Paint::RoundedRectangle`、`Paint::Rectangle`、`Paint::Text`、`Paint::Icon` 等形状节点只描述实际绘制内容，不应承担布局决策。
- `Paint::EraseRectangle` 应优先放在 `Paint::Buffer` 内使用，用于通过 `CompositionMode_DestinationOut` 擦除缓冲层内容。

从 `paint_outlined` 提炼的可复用模式：

- 轮廓组件可以先在 `Paint::Buffer` 中绘制完整轮廓，再用 `Paint::EraseRectangle` 擦出 label 缺口，最后在缓冲层之外绘制 label 文本。
- 带浮动 label 的组件应把 label 动画抽象成一个 `TransitionValue`，绘制时只读取当前位置并插值 origin、size、scale。
- 输入框、选择框等状态型组件应在 `focusInEvent()`、`focusOutEvent()`、`enterEvent()`、`leaveEvent()` 中只更新状态并触发重绘，不要直接在事件函数里做绘制计算。
- `update_component_status()` 适合集中同步 Qt 自身状态，如 `setTextMargins()`、`setFont()`、缓存 icon 字体；它应由尺寸、图标启用状态或字段类型变化触发，避免每帧重复做不必要 setter。
- 当组件有 filled/outlined/elevated 等变体时，优先复用基础状态、主题、测量和事件逻辑，只拆分最终绘制函数或少量变体专属参数。

开发组件时的检查清单：

- 公开 prop 是否都能从组件自己的 `xxx::pro` 命名空间补全到。
- 组件 `pro` 是否导入了它支持的全部共享 scope（`api::scope::*`）。
- prop 是否最终调用明确 setter，而不是直接接触内部字段。
- 主题色是否按 enabled/focused/error/disabled 等状态归档，而不是散落在绘制逻辑中。
- 绘制树是否只表达结构，复杂计算是否已提前命名。
- 绘制节点是否职责单一：容器管布局，形状管绘制，Buffer 管合成。
- 是否保留 Qt 父子所有权、事件投递、布局和信号/槽语义。
- 是否优先使用 LSP 单文件诊断确认模板错误，而不是一开始就完整编译。
