# Creeper Qt 使用指南

## 库模型

- 将本项目视为 Qt Widgets 之上的声明式薄封装，而不是 Qt 的替代品。
- 封装层应保留 Qt 行为：对象生命周期、父子所有权、事件投递、布局行为和信号/槽语义仍由 Qt 负责。
- 优先使用小型转发抽象，清晰映射到现有 Qt setter、布局 API、绘制逻辑或组件专属样式行为。
- 除非有明确且已文档化的理由，否则不要引入与 QObject 父子所有权冲突的所有权模型。

## Prop 命名空间使用

- 始终从正在构造的组件对应命名空间中取 prop。
- 对 `FilledButton`，使用 `creeper::filled_button::pro::*`，即使某个 prop 内部来自 `button::pro`、`widget::pro` 或 `theme::pro`。
- 组件的 `pro` 命名空间是有意设计的聚合入口，应暴露用户构造该组件所需的完整 prop 表面。
- 在 `.cc` 文件中，`using namespace creeper;` 很常见。当该命名空间已在作用域内时，不要给组件类型添加冗余的 `creeper::` 前缀。
- 优先使用局部命名空间别名，以提高可读性和 IDE 补全体验：

```cpp
namespace cp = creeper::card::pro;
namespace fbp = creeper::filled_button::pro;
namespace lp = creeper::linear::pro;

auto content = FilledCard {
    cp::ThemeManager { manager },
    cp::Layout<Col> {
        lp::Item<FilledButton> {
            fbp::ThemeManager { manager },
            fbp::Text { "OK" },
            fbp::FixedWidth { 120 },
        },
    },
};
```

- 避免在应用代码中依赖全局 `using namespace ...::pro`。局部别名能更清楚地表达 prop 来源组件和可用的属性集合。

## 声明式构造

- 声明式 prop 列表应聚焦于 UI 结构和配置。
- 优先通过布局/条目 prop 进行组件嵌套，这是主要构造方式。
- 当嵌套变得难以阅读时，将大型 UI 树拆分为具名组件或辅助函数。
- 按可预测的顺序组织 prop：主题/状态、尺寸/布局、内容、回调，最后是 `Apply` 等兜底式自定义。
- 优先使用专用 prop，而不是 `Apply`。仅当封装层尚未暴露所需 Qt 操作时再使用 `Apply`。
- 将复杂业务逻辑放在 prop 列表之外。先命名 lambda、状态对象和回调，再传入 UI 声明。
- 避免接触 `internal::`、`details::` 等内部命名空间下的实现，也不要直接调用 `prop.apply()`。prop 是内部机制，UI 代码应仅通过声明式嵌套使用。缺 prop 时使用 `Apply` 进行自定义。
- 保持现有模式：声明式 prop 最终应调用用户手写时也会调用的同一个 Qt setter 或组件 setter。

## Qt 生命周期和指针

- 裸指针在本代码库中是正常现象，因为 Qt Widgets 使用 QObject 父子所有权。
- 在示例和应用代码中，优先使用声明式嵌套，而不是独立的 `new` 表达式。
- 当 widget 或 layout 会立即交给 parent、layout 或 Qt 拥有的容器时，可以使用 `new` 分配，尤其是在布局/条目 prop 内部。
- 不要只是为了避免裸指针而把 Qt 拥有的对象换成智能指针。这在 Qt 代码中往往会让所有权更不正确。
- 当保存裸指针供后续使用时，确保目标对象由生命周期更长的 QObject 拥有，或在销毁时断开/清理。
- 对队列回调、存储的 lambda、主题处理器和消息总线处理器要格外小心。Qt 父子所有权不会自动保证捕获引用安全。

## 错误诊断

- 优先通过 LSP 报错定位问题，而非完整编译。模板实例化导致编译开销很大、报错数量多且难以阅读，LSP 的单文件诊断更精准友好。
- `Declarative` checker 和 fallback `static_assert` 是用户体验的一部分，不要随意移除。
- `props_trait<int &>` 这样的诊断是有价值的，因为它能指出确切的非法参数类型，包括嵌套在 tuple 内部的非法值。
- 如果改进诊断，应保留尽早报告真实错误 prop 类型的能力，不要让模板展开退化成长篇构造函数错误。
- 在可行时，优先区分这些情况：不是 prop、是另一个组件的合法 prop、prop 的 apply 目标不兼容。

## 性能预期

- Setter 风格的 prop 应保持轻薄且易于内联。其运行时成本应接近等价的手写 Qt setter 调用。
- 大多数 prop 检查成本发生在编译期。避免为普通 setter prop 添加运行时注册表或动态分发。
- 接受模板实例化、IDE 索引和诊断是该 DSL 的主要成本。
- 真正的运行时热点更可能出现在绘制、动画、布局、主题广播或 Qt 事件处理，而不是简单的 prop 转发。

## API 和文档

- 按每个组件自己的 `xxx::pro` 命名空间编写文档，不要要求用户去 `widget::pro`、`theme::pro` 或基础组件命名空间中搜索。
- 添加组件时，让它的 `pro` 命名空间成为完整的用户入口，导入该组件支持的共享 prop 命名空间。
- 尽可能让 prop 名称贴近对应 Qt setter：`setFixedWidth()` 变为 `FixedWidth`，`setText()` 变为 `Text`，以此类推。
- 添加新 prop 时，确保它可以通过用户预期用于 IDE 补全的组件命名空间发现。

## 组件开发模式

- 开发新组件时，优先遵循 `text-fields.hh` / `text-fields.impl.hh` 展示的三层结构：`internal` 实现类负责 Qt 行为，组件自己的 `xxx::pro` 命名空间负责声明式属性入口，公开 `using Xxx = Declarative<...>` 负责把两者连接起来。
- `internal` 类应继承最贴近语义的 Qt 基类，而不是重新发明行为；例如文本输入继续继承 `QLineEdit`，按钮继续基于 Qt 按钮能力，绘制和交互只补足本组件的样式差异。
- `internal` 类中集中保存组件运行状态，例如 hover、focus、disabled、error、checked、动画值、颜色 token、尺寸 token、缓存字体等；这些状态应最终驱动 Qt setter、绘制树或主题响应。
- 对外暴露给 prop 使用的接口应是清晰的 setter，例如 `set_label_text()`、`set_measurements()`、`set_color_scheme()`、`load_theme_manager()`；不要让 prop 直接改内部字段。
- `.hh` 文件声明组件形状、状态结构和 prop 表面；`.impl.hh` 或 `.cc` 文件承载绘制、动画、主题映射和事件处理细节，避免把实现逻辑塞进公开头文件的声明式入口。

典型组件骨架：

```cpp
// 原始实现，不包含声明式封装
namespace creeper::xxx::internal {

class Xxx : public QWidget {
    CREEPER_PIMPL_DEFINITION(Xxx);

public:
    void set_color_scheme(const ColorScheme&);
    void load_theme_manager(ThemeManager&);
    void set_label_text(const QString&);

protected:
    void paintEvent(QPaintEvent*) override;
};

}

// 声明式属性的包装，优先复用已有的 Wrapper
namespace creeper::xxx::pro {

using Token = common::Token<internal::Xxx>;

using LabelText = common::pro::String<Token,
    [](auto& self, const auto& value) { self.set_label_text(value); }>;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);

// 在导出的命名空间中间中组合适用的属性
using namespace widget::pro;
using namespace theme::pro;
}

// 最终导出类型，组合检查器
namespace creeper {

using Xxx = Declarative<xxx::internal::Xxx,
    CheckerOr<xxx::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
```

- 组件自己的 `xxx::pro` 命名空间必须是用户入口，而不是只放本组件新增 prop；如果组件支持 `widget::pro` 或 `theme::pro`，就在本组件 `pro` 中导入它们，让 IDE 补全从一个命名空间开始。
- `CheckerOr<...>` 必须覆盖 `xxx::pro` 命名空间导入的所有 prop 来源；如果 `xxx::pro` 导入了 `widget::pro`、`theme::pro`、`button::pro`，公开别名中也应包含对应 checker。
- 简单 prop 优先用 `SetterProp` 或 `DerivedProp` 转发到 setter，保持运行时成本接近手写 setter；只有需要设置多个字段、支持多种构造来源或包含额外逻辑时，才手写继承 `Token` 的 prop 和 `apply()`。
- 从通用 prop 模板实例化本组件 prop 时，始终绑定本组件的 `Token`；不要直接要求用户混用基础组件命名空间中的 prop 名称。
- prop 的 `apply()` 只做声明式配置，不应启动复杂业务流程；复杂状态应先被封装成组件 setter 或内部方法，再由 prop 调用。

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
- `CheckerOr` 是否覆盖了 `xxx::pro` 中导入的所有 checker。
- prop 是否最终调用明确 setter，而不是直接接触内部字段。
- 主题色是否按 enabled/focused/error/disabled 等状态归档，而不是散落在绘制逻辑中。
- 绘制树是否只表达结构，复杂计算是否已提前命名。
- 绘制节点是否职责单一：容器管布局，形状管绘制，Buffer 管合成。
- 是否保留 Qt 父子所有权、事件投递、布局和信号/槽语义。
- 是否优先使用 LSP 单文件诊断确认模板错误，而不是一开始就完整编译。
