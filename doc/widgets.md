# Widgets 文档

## 通用组件属性

命名空间 : `creeper::widget::pro`

| `PROPERTY`      | `TYPE`  | `METHOD`           |
| --------------- | ------- | ------------------ |
| `MinimumSize`   | `QSize` | `setMinimumSize`   |
| `MaximumSize`   | `QSize` | `setMaximumSize`   |
| `SizeIncrement` | `QSize` | `setSizeIncrement` |
| `BaseSize`      | `QSize` | `setBaseSize`      |
| `FixedSize`     | `QSize` | `setFixedSize`     |
| `...`           | `...`   | `...`              |

上述属性只是将 QWidget 的大部分 Setter 按照本项目的风格做了一层封装，按照印象中的接口使用即可，后续章节将省略 `METHOD` 这一列，毕竟 `property` 加个 `set` 便是 `method`

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
const auto props = std::tuple {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
};

// 可以直接使用 tuple 进行构造
auto widget_a = new Widget { props };

// 也可以在传入 tuple 后继续传入属性进行差异化修改
auto widget_b = new Widget {
    props,
    pro::WindowFlag { Qt::WindowStaysOnTopHint },
};
```

这样你便可以在配置完组件后将这个组件声明为 `const`（虽然我也不知道有什么必要）：

```cpp
const auto widget_a = new Widget { ... };
```

## 普通按钮组件

- 完整主题：内置 `kBlueMikuThemePack`
- 暗色主题：内置 `kBlueMikuDarkColorScheme`
- 亮色主题：内置 `kBlueMikuLightColorScheme`

![buttons](https://creeper5820.com/creeper-qt/blue-style-widgets.png)

### 通用属性定义

```cpp
namespace button::pro {
    using Text        = common::pro::Text<Token>;
    using TextColor   = common::pro::TextColor<Token>;
    using Radius      = common::pro::Radius<Token>;
    using BorderWidth = common::pro::BorderWidth<Token>;
    using BorderColor = common::pro::BorderColor<Token>;
    using Background  = common::pro::Background<Token>;
    using WaterColor  = common::pro::WaterColor<Token>;

    template <typename Callback, class Token> struct Clickable : Token;
}
```

| `PROPERTY`    | `TYPE`       |
| ------------- | ------------ |
| `Text`        | `QString`    |
| `TextColor`   | `QColor`     |
| `Radius`      | `double`     |
| `BorderWidth` | `double`     |
| `BorderColor` | `QColor`     |
| `Background`  | `QColor`     |
| `WaterColor`  | `QColor`     |
| `Clickable`   | `[](self){}` |

### FilledButton

命名空间: `creeper::filled_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

### FilledTonalButton

`FilledButton` 改个颜色方案罢了 * 1

命名空间: `creeper::filled_tonal_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

### OutlinedButton

`FilledButton` 改个颜色方案罢了 * 2

命名空间: `creeper::outlined_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

### TextButton

`FilledButton` 改个颜色方案罢了 * 3

命名空间: `creeper::text_button::pro`

子属性：

- `creeper::util::theme::pro`
- `creeper::widget::pro`
- `creeper::button::pro`

注意，属性以组合的形式定义，用 concept 约束，所以有以下等价形式：

```cpp
static_assert(std::same_as<filled_button::pro::Text, button::pro::Text>, "");
static_assert(std::same_as<filled_button::pro::Radius, button::pro::Radius>, "");
// ......
// Value = true
```

于是，我们可以定义这样的通用属性集合配置到下面任意一种按钮中：

```cpp
const auto properties = std::tuple {
    util::theme::pro::ThemeManager { theme_manager },
    widget::pro::FixedSize { 100, 50 },
    widget::pro::Font { "JetBrains Mono", 12 },
    button::pro::Text { "你好世界" },
    button::pro::Radius { 25 },
};

auto button = XxxxButton { properties };
```

## 其他可交互组件

### Switch

<div align=center>
    <img src="https://creeper5820.com/creeper-qt/switch-working.gif" width=400>
    <br/>
    <br/>
</div>

实现这个组件时纠结了许久，原 `MD3` 的 `Switch` 规则过于复杂，一些参数例如 `Handle` 膨胀拉伸形变的系数没有给出，而且基于曲线函数推导出来的动画在打断时不自然，于是经过再三斟酌，决定大体上复现外观，动画效果上用打断效果更好的弹簧物理模拟来替代曲线取值，阉割掉比较复杂的按压和拉伸动画，事实上最后呈现出来的结果我觉得还不错，但未来有精力还是完全复现一下吧

命名空间: `creeper::_switch::pro`

子属性:

- `creeper::util::theme::pro`
- `creeper::widget::pro`

属性：

| `PROPERTIES`                    | `TYPE`             |
| ------------------------------- | ------------------ |
| `TrackColorUnchecked`           | `QColor`           |
| `TrackColorChecked`             | `QColor`           |
| `TrackColorUncheckedDisabled`   | `QColor`           |
| `TrackColorCheckedDisabled`     | `QColor`           |
| `HandleColorUnchecked`          | `QColor`           |
| `HandleColorChecked`            | `QColor`           |
| `HandleColorUncheckedDisabled`  | `QColor`           |
| `HandleColorCheckedDisabled`    | `QColor`           |
| `OutlineColorUnchecked`         | `QColor`           |
| `OutlineColorChecked`           | `QColor`           |
| `OutlineColorUncheckedDisabled` | `QColor`           |
| `OutlineColorCheckedDisabled`   | `QColor`           |
| `HoverColorUnchecked`           | `QColor`           |
| `HoverColorChecked`             | `QColor`           |
| `Clickable`                     | `[](auto& self){}` |

### FilledTextField

<div align=center>
    <img src="https://creeper5820.com/creeper-qt/filled-text-field.gif" width=400>
    <br/>
    <br/>
</div>
