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
