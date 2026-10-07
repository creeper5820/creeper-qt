---
name: creeper-qt-usage
description: creeper-qt 的使用规范：声明式构造、prop 命名空间、Qt 生命周期与指针。编写使用 creeper-qt 组件的应用/示例代码时使用。
---

# creeper-qt 使用规范

## 库模型

- 将本项目视为 Qt Widgets 之上的声明式薄封装，而不是 Qt 的替代品。
- 封装层应保留 Qt 行为：对象生命周期、父子所有权、事件投递、布局行为和信号/槽语义仍由 Qt 负责。
- 优先使用小型转发抽象，清晰映射到现有 Qt setter、布局 API、绘制逻辑或组件专属样式行为。
- 除非有明确且已文档化的理由，否则不要引入与 QObject 父子所有权冲突的所有权模型。

## Prop 命名空间使用

- 始终从正在构造的组件对应命名空间中取 prop。
- 对 `FilledButton`，使用 `creeper::filled_button::pro::*`，即使某个 prop 内部来自 `api::scope::widget`、`api::scope::theme` 或其它共享 scope。
- 组件的 `pro` 命名空间是有意设计的聚合入口，应暴露用户构造该组件所需的完整 prop 表面。
- 在 `.cc` 文件中，`using namespace creeper;` 很常见。当该命名空间已在作用域内时，不要给组件类型添加冗余的 `creeper::` 前缀。
- 优先使用局部命名空间别名，以提高可读性和 IDE 补全体验：

```cpp
namespace cp  = creeper::card::pro;
namespace fbp = creeper::filled_button::pro;

auto content = FilledCard {
    manager,
    cp::Layout<Col> {
      new FilledButton {
        manager,
        fbp::Text { "OK" },
        fbp::FixedWidth { 120 },
      } + Col::Placement { 0 },
    },
};
```

- 避免在应用代码中依赖全局 `using namespace ...::pro`。局部别名能更清楚地表达 prop 来源组件和可用的属性集合。

## 声明式构造

- 声明式 prop 列表应聚焦于 UI 结构和配置。
- 优先通过布局/条目 prop 进行组件嵌套，这是主要构造方式。
- 当嵌套变得难以阅读时，将大型 UI 树拆分为具名组件或辅助函数。
- 按可预测的顺序组织 prop：主题/状态、尺寸/布局、内容、回调，最后是 `With` 等兜底式自定义。
- 优先使用专用 prop，而不是 `With`。仅当封装层尚未暴露所需 Qt 操作时再用 `With` 自定义。
- 将复杂业务逻辑放在 prop 列表之外。先命名 lambda、状态对象和回调，再传入 UI 声明。
- prop 通过隐藏友元 `dsl_invoke` 作用于组件；UI 代码只做声明式嵌套，不要直接调用内部机制。缺 prop 时用 `With`。
- 保持现有模式：声明式 prop 最终应调用用户手写时也会调用的同一个 Qt setter 或组件 setter。

## Qt 生命周期和指针

- 裸指针在本代码库中是正常现象，因为 Qt Widgets 使用 QObject 父子所有权。
- 在示例和应用代码中，优先使用声明式嵌套，而不是独立的 `new` 表达式。
- 当 widget 或 layout 会立即交给 parent、layout 或 Qt 拥有的容器时，可以使用 `new` 分配，尤其是在布局/条目 prop 内部。
- 不要只是为了避免裸指针而把 Qt 拥有的对象换成智能指针。这在 Qt 代码中往往会让所有权更不正确。
- 当保存裸指针供后续使用时，确保目标对象由生命周期更长的 QObject 拥有，或在销毁时断开/清理。
- 对队列回调、存储的 lambda、主题处理器和消息总线处理器要格外小心。Qt 父子所有权不会自动保证捕获引用安全。
