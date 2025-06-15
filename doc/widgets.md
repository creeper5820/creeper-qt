# Widgets 文档

## 通用组件属性

### 命名空间:

属性命名空间 : `creeper::widget::pro`

### 调用示例:

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

Widget 完全继承自 QWidget，只附加了一系列构造方法用于传入配置，**和传统调用手段完全兼容**，后续所有组件也使用类似封装方法，其中，属性的命名遵循 `set_xxx > Xxx`，如果是原有的接口，则为 `setXxx > Xxx`

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

![buttons-dark](image/buttons-dark.png)

![buttons-light](image/buttons-light.png)

### ElevatedButton

### FilledButton

#### 命名空间

属性命名空间: `creeper::filled_button::pro`

子属性： `creeper::widget::pro`

#### 专属属性

```cpp
namespace filled_button::pro {
    // 配色方案
    using ColorScheme  = util::theme::pro::ColorScheme<...>;
    // 主题管理器
    using ThemeManager = util::theme::pro::ThemeManager<...>;

    // 按钮文字
    using Text        = common::pro::Text<...>;
    // 文字颜色
    using TextColor   = common::pro::TextColor<...>;
    // 按钮圆角半径
    using Radius      = common::pro::Radius<...>;
    // 按钮边界宽度
    using BorderWidth = common::pro::BorderWidth<...>;
    // 按钮边界颜色
    using BorderColor = common::pro::BorderColor<...>;
    // 按钮背景颜色
    using Background  = common::pro::Background<...>;
    // 水波纹颜色
    using WaterColor  = common::pro::WaterColor<...>;
    // 点击事件
    using Clickable   = button::pro::Clickable<...>;

    // 兼容 widget::pro 所有属性
    using namespace widget::pro;
}
```


### FilledTonalButton

### OutlinedButton

### TextButton
