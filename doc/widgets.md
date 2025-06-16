# Widgets 文档

## 通用组件属性

命名空间 : `creeper::widget::pro`

### 调用示例

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;
auto widget = new Widget {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
    ...... // 其他 QWidget 的 Setter
};
```

上面的示例将配置过程提前到构造阶段，等价于:

```cpp
auto qwidget = new QWidget {};
qwidget->setMaximumSize(200, 100);
qwidget->setMinimumSize(100, 050);
......
```

Widget 完全继承自 QWidget，只附加了一系列构造方法用于传入配置，**和传统调用手段完全兼容**，后续所有组件也使用类似封装方法

其中，属性的命名遵循 `set_xxx > Xxx`，如果是原有的接口，则为 `setXxx > Xxx`

使用上述方法，可以便捷的复用已知共有属性，例如:

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;

// 可以使用 tuple 打包属性
const auto properties = std::tuple {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
};

// 可以直接使用 tuple 进行构造
auto widget_a = new Widget { properties };

// 也可以在传入 tuple 后继续传入属性进行差异化修改（只允许一个 tuple 在参数列表开头）
auto widget_b = new Widget {
    properties,
    pro::WindowFlag { Qt::WindowStaysOnTopHint },
};
```

这样你便可以在配置完组件后将这个组件声明为 `const`（虽然我也不知道有什么必要）：

```cpp
const auto widget_a = new Widget { ... };
```

## 普通按钮组件

- 完整主题：内置 `kBlueMikuThemePack`
- 暗色主题：内置 `util::theme::kBlueMikuDarkColorScheme`
- 亮色主题：内置 `util::theme::kBlueMikuLightColorScheme`

![buttons](image/buttons.png)

### 通用属性定义

```cpp
namespace button::pro {
    using Text        = common::pro::Text<Property>;
    using TextColor   = common::pro::TextColor<Property>;
    using Radius      = common::pro::Radius<Property>;
    using BorderWidth = common::pro::BorderWidth<Property>;
    using BorderColor = common::pro::BorderColor<Property>;
    using Background  = common::pro::Background<Property>;
    using WaterColor  = common::pro::WaterColor<Property>;

    struct Clickable final : Property { ... };
}
```

注意，属性以组合的形式定义，用 concept 约束，所以有以下等价形式：

```cpp
static_assert(std::same_as<filled_button::pro::Text, button::pro::Text>, "");
static_assert(std::same_as<filled_button::pro::Radius, button::pro::Radius>, "");
// ......
// Value = true
```

于是，我们可以这样的定义属性集合配置到下面任意一种按钮中：

```cpp
const auto properties = std::tuple {
    util::theme::pro::ThemeManager { theme_manager },
    widget::pro::FixedSize { 100, 50 },
    widget::pro::Font { "JetBrains Mono", 12 },
    button::pro::Text { "你好世界" },
    button::pro::Radius { 25 },
};
```

### ElevatedButton

TODO

### FilledButton / FilledTonalButton

命名空间: `creeper::filled_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

### OutlinedButton

命名空间: `creeper::outlined_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

### TextButton

TODO
