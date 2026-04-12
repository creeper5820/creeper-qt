# CREEPER-QT 组件文档

[返回主页](../README.md) | [使用指南](./usage.md)

---

## 通用组件属性

命名空间：`creeper::widget::pro`

| 属性 | 类型 | 方法 |
| --- | --- | --- |
| `MinimumSize` | `QSize` | `setMinimumSize` |
| `MaximumSize` | `QSize` | `setMaximumSize` |
| `SizeIncrement` | `QSize` | `setSizeIncrement` |
| `BaseSize` | `QSize` | `setBaseSize` |
| `FixedSize` | `QSize` | `setFixedSize` |
| *...更多 QWidget 属性...* | | |

上述属性是将 `QWidget` 的大部分 Setter 按照本项目风格进行的封装。后续文档将省略 `METHOD` 列，属性名加 `set` 前缀即为对应方法名。

### 声明式配置（推荐）

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;

auto widget = new Widget {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
};
```

### 等价的传统写法

```cpp
auto qwidget = new QWidget {};
qwidget->setMaximumSize(200, 100);
qwidget->setMinimumSize(100, 050);
```

### 属性复用

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;

const auto props = std::tuple {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
};

auto widget_a = new Widget { props };

auto widget_b = new Widget {
    props,
    pro::WindowFlag { Qt::WindowStaysOnTopHint },
};
```

`Widget` 完全继承自 `QWidget`，只是附加了声明式构造方法，与传统调用方式完全兼容。

---

## 按钮组件

<div align="center">
<img src="https://r2.creeper5820.com/creeper-qt/blue-style-widgets.png" width="800" alt="按钮样式展示">
</div>

### 通用按钮属性

命名空间：`creeper::button::pro`

```cpp
namespace button::pro {
    using Text        = common::pro::Text<Token>;
    using TextColor   = common::pro::TextColor<Token>;
    using Radius      = common::pro::Radius<Token>;
    using BorderWidth = common::pro::BorderWidth<Token>;
    using BorderColor = common::pro::BorderColor<Token>;
    using Background  = common::pro::Background<Token>;
    using WaterColor  = common::pro::WaterColor<Token>;

    template <typename Callback, class Token>
    struct Clickable : Token;
}
```

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Text` | `QString` | 按钮文本内容 |
| `TextColor` | `QColor` | 文本颜色 |
| `Radius` | `double` | 圆角半径 |
| `BorderWidth` | `double` | 边框宽度 |
| `BorderColor` | `QColor` | 边框颜色 |
| `Background` | `QColor` | 背景颜色 |
| `WaterColor` | `QColor` | 水波纹颜色 |
| `Clickable` | `[](self){}` | 点击回调函数 |

---

### FilledButton

命名空间：`creeper::filled_button::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`、`creeper::button::pro`

```cpp
using namespace creeper;

auto button = new FilledButton {
    button::pro::Text { "提交" },
    button::pro::Radius { 8.0 },
    button::pro::Background { QColor("#2196F3") },
    button::pro::Clickable { [](auto& self) {
        qDebug() << "按钮被点击";
    }}
};
```

---

### FilledTonalButton

命名空间：`creeper::filled_tonal_button::pro`

与 `FilledButton` 相同的 API，仅配色方案不同。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`、`creeper::button::pro`

---

### OutlinedButton

命名空间：`creeper::outlined_button::pro`

与 `FilledButton` 相同的 API，仅配色方案不同。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`、`creeper::button::pro`

---

### TextButton

命名空间：`creeper::text_button::pro`

与 `FilledButton` 相同的 API，仅配色方案不同。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`、`creeper::button::pro`

---

### 属性组合复用

由于属性通过 `concept` 约束定义，以下等价关系成立：

```cpp
static_assert(std::same_as<filled_button::pro::Text, button::pro::Text>);
static_assert(std::same_as<filled_button::pro::Radius, button::pro::Radius>);
```

因此可以定义通用属性集合并应用到任意按钮类型：

```cpp
const auto properties = std::tuple {
    util::theme::pro::ThemeManager { theme_manager },
    widget::pro::FixedSize { 100, 50 },
    widget::pro::Font { "JetBrains Mono", 12 },
    button::pro::Text { "你好世界" },
    button::pro::Radius { 25 },
};

auto filled_button = FilledButton { properties };
auto outlined_button = OutlinedButton { properties };
auto text_button = TextButton { properties };
```

---

## 交互组件

### Switch 开关

<div align="center">
<img src="https://r2.creeper5820.com/creeper-qt/switch-working.gif" width="500" alt="Switch 组件动画">
</div>

命名空间：`creeper::_switch::pro`

实现此组件时经过多次权衡。原 Material Design 3 的 Switch 规则过于复杂，一些参数（如 Handle 的膨胀拉伸形变系数）未给出明确定义。基于曲线函数的动画在打断时表现不自然。最终决定大体复现 MD3 外观设计，使用弹簧物理模拟替代曲线动画，简化了按压和拉伸动画。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

完整属性列表参考[源代码](../creeper-qt/widget/switch.hh#L60-L106)，主要属性包括：

- `Clickable` - 点击回调函数
- `Checked` / `Disabled` - 状态控制
- `TrackColor*` - 轨道颜色系列（Checked/Unchecked/Disabled 状态）
- `HandleColor*` - 手柄颜色系列（Checked/Unchecked/Disabled 状态）
- `OutlineColor*` - 轮廓颜色系列（Checked/Unchecked/Disabled 状态）
- `HoverColor*` - 悬停颜色系列（Checked/Unchecked 状态）

```cpp
using namespace creeper;

auto switch_widget = new Switch {
    _switch::pro::TrackColorChecked { QColor("#2196F3") },
    _switch::pro::HandleColorChecked { QColor("#FFFFFF") },
    _switch::pro::Clickable { [](auto& self) {
        qDebug() << "开关状态:" << self.isChecked();
    }}
};
```

---

### FilledTextField 文本框

<div align="center">
<img src="https://r2.creeper5820.com/creeper-qt/filled-text-field.gif" width="500" alt="FilledTextField 组件动画">
</div>

命名空间：`creeper::text_field::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `LabelText` | `QString` | 标签文本（浮动标签） |
| `LeadingIcon` | `QString, QString` | 前置图标（图标代码，字体名称） |
| `ClearButton` | `bool` | 是否显示清除按钮 |
| `OnTextChanged` | `[](const QString&){}` | 文本改变时的回调函数 |
| `OnEditingFinished` | `[]{}` | 编辑完成时的回调函数 |
| `OnChanged` | `[](const QString&){}` | `OnTextChanged` 的别名 |

```cpp
using namespace creeper;

auto text_field = new FilledTextField {
    text_field::pro::ThemeManager { manager },
    text_field::pro::LabelText { "用户名" },
    text_field::pro::FixedSize { 250, 56 },
    text_field::pro::OnTextChanged { [](const QString& text) {
        qDebug() << "输入内容:" << text;
    }}
};

// 带前置图标
auto search_field = new FilledTextField {
    text_field::pro::ThemeManager { manager },
    text_field::pro::LeadingIcon {
        material::icon::kSearch,
        material::round::font
    },
    text_field::pro::LabelText { "搜索" },
    text_field::pro::ClearButton { true }
};

// 与 MutableValue 绑定
auto text_value = std::make_shared<MutableValue<QString>>();
text_value->set_silent("初始值");

auto bound_field = new FilledTextField {
    text_field::pro::ThemeManager { manager },
    text_field::pro::LabelText { "输入框" },
    MutableForward {
        text_field::pro::LabelText {},
        text_value
    }
};
```

---

### OutlinedTextField 文本框

命名空间：`creeper::text_field::pro`

与 `FilledTextField` 相同的 API，仅外观样式不同（带边框）。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

```cpp
using namespace creeper;

auto outlined_field = new OutlinedTextField {
    text_field::pro::ThemeManager { manager },
    text_field::pro::LabelText { "密码" },
    text_field::pro::LeadingIcon {
        "lock",
        material::round::font
    }
};
```

---

### Slider 滑块

命名空间：`creeper::slider::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Progress` | `double` | 进度值（0.0 - 1.0） |
| `Measurements` | `Measurements` | 尺寸配置（支持 Xs, S, M, L, SL 预设） |
| `OnValueChange` | `[](double){}` | 值改变时的回调函数 |
| `OnValueChangeFinished` | `[](double){}` | 值改变完成时的回调函数 |

```cpp
using namespace creeper;

auto slider = new Slider {
    slider::pro::ThemeManager { manager },
    slider::pro::Measurements { Slider::Measurements::M() },
    slider::pro::FixedHeight { 52 },
    slider::pro::FixedWidth { 300 },
    slider::pro::Progress { 0.5 },
    slider::pro::OnValueChange { [](double progress) {
        qDebug() << "进度:" << progress;
    }}
};

// 与 MutableValue 绑定
auto progress_value = std::make_shared<MutableValue<double>>();
progress_value->set_silent(0.2);

auto bound_slider = new Slider {
    slider::pro::ThemeManager { manager },
    slider::pro::Measurements { Slider::Measurements::S() },
    MutableForward {
        slider::pro::Progress { 0. },
        progress_value
    },
    slider::pro::OnValueChange {
        [=](double progress) {
            *progress_value = progress;
        }
    }
};
```

---

### FilledDropdownMenu 下拉菜单

命名空间：`creeper::dropdown_menu::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `LabelText` | `QString` | 标签文本 |
| `LeadingIcon` | `QString, QString` | 前置图标（图标代码，字体名称） |
| `Items` | `QVector<QString>` | 选项列表 |
| `IndexChanged` | `[](int){}` | 选中项改变时的回调函数 |

```cpp
using namespace creeper;

auto dropdown = new FilledDropdownMenu {
    dropdown_menu::pro::ThemeManager { manager },
    dropdown_menu::pro::LabelText { "选择项" },
    dropdown_menu::pro::Items { QStringList { "选项1", "选项2", "选项3" } },
    dropdown_menu::pro::IndexChanged { [](int index) {
        qDebug() << "选中索引:" << index;
    }}
};

// 与 MutableValue 绑定
auto items_value = std::make_shared<MutableValue<QStringList>>();
items_value->set_silent(QStringList { "1st", "2nd", "3rd" });

auto bound_dropdown = new FilledDropdownMenu {
    dropdown_menu::pro::ThemeManager { manager },
    dropdown_menu::pro::LabelText { "Item" },
    MutableForward {
        dropdown_menu::pro::Items {},
        items_value
    }
};
```

---

### FilledSelect 选择器

命名空间：`creeper::select_widget::pro`

与 `FilledDropdownMenu` 功能类似，但外观和交互略有不同。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `LabelText` | `QString` | 标签文本 |
| `LeadingIcon` | `QString, QString` | 前置图标（默认使用下拉箭头图标） |
| `Items` | `QVector<QString>` | 选项列表 |
| `IndexChanged` | `[](int){}` | 选中项改变时的回调函数 |

```cpp
using namespace creeper;

auto select = new FilledSelect {
    select_widget::pro::ThemeManager { manager },
    select_widget::pro::LabelText { "选择" },
    select_widget::pro::Items { QStringList { "选项A", "选项B", "选项C" } }
};
```

---

### Image 图片

命名空间：`creeper::image::pro`

继承属性：`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `PainterResource` | `std::shared_ptr<PainterResource>` | 图片资源（支持 QPixmap, QImage 等） |
| `Pixmap` | `QPixmap` | 设置图片（PainterResource 的别名） |
| `ContentScale` | `ContentScale` | 内容缩放模式 |
| `Opacity` | `double` | 透明度（0.0 - 1.0） |
| `Radius` | `double` | 圆角半径 |
| `BorderWidth` | `double` | 边框宽度 |
| `BorderColor` | `QColor` | 边框颜色 |

```cpp
using namespace creeper;

auto image = new Image {
    image::pro::Pixmap { QPixmap("path/to/image.png") },
    image::pro::FixedSize { 200, 200 },
    image::pro::Radius { 10 },
    image::pro::BorderWidth { 2 },
    image::pro::BorderColor { QColor("#000000") }
};

auto scaled_image = new Image {
    image::pro::Pixmap { QPixmap("path/to/image.png") },
    image::pro::ContentScale { ContentScale::Fit },
    image::pro::FixedSize { 300, 300 }
};
```

---

### Text 文本

命名空间：`creeper::text::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Text` | `QString` | 文本内容 |
| `Color` | `QColor` | 文本颜色 |
| `Alignment` | `Qt::Alignment` | 文本对齐方式 |
| `WordWrap` | `bool` | 是否自动换行 |
| `TextInteractionFlags` | `Qt::TextInteractionFlags` | 文本交互标志 |
| `AdjustSize` | - | 自动调整大小以适应内容 |

```cpp
using namespace creeper;

auto text = new Text {
    text::pro::ThemeManager { manager },
    text::pro::Text { "Hello World" },
    text::pro::Alignment { Qt::AlignCenter }
};

// 可选择的文本
auto selectable_text = new Text {
    text::pro::ThemeManager { manager },
    text::pro::Text { "可选择的文本内容" },
    text::pro::WordWrap { true },
    text::pro::TextInteractionFlags {
        Qt::TextInteractionFlag::TextSelectableByMouse
    }
};

// 与 MutableValue 绑定
auto text_value = std::make_shared<MutableValue<QString>>();
text_value->set_silent("初始文本");

auto bound_text = new Text {
    text::pro::ThemeManager { manager },
    MutableForward {
        text::pro::Text {},
        text_value
    }
};
```

---

### Card 卡片组件

命名空间：`creeper::card::pro`

提供多种样式：`FilledCard`、`OutlinedCard`、`ElevatedCard`、`BasicCard`。

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`、`creeper::rounded_rect::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Level` | `CardLevel` | 卡片层级（DEFAULT, HIGH, HIGHEST, LOW, LOWEST） |
| `LevelDefault` | - | 默认层级（常量） |
| `LevelHigh` | - | 高层级（常量） |
| `LevelHighest` | - | 最高层级（常量） |
| `LevelLow` | - | 低层级（常量） |
| `LevelLowest` | - | 最低层级（常量） |

```cpp
using namespace creeper;
namespace capro = card::pro;

auto filled_card = new FilledCard {
    capro::ThemeManager { manager },
    capro::Level { CardLevel::HIGHEST },
    capro::Radius { 12 },
    capro::FixedSize { 200, 150 },
    capro::Layout<Col> {
        // 卡片内容
    }
};

auto outlined_card = new OutlinedCard {
    capro::ThemeManager { manager },
    capro::LevelLowest,
    capro::Radius { 8 },
    capro::Layout<Row> {
        // 卡片内容
    }
};

auto elevated_card = new ElevatedCard {
    capro::ThemeManager { manager },
    capro::LevelHigh,
    capro::Layout<Col> {
        // 卡片内容
    }
};
```

---

### IconButton 图标按钮

命名空间：`creeper::icon_button::pro`

继承属性：`creeper::util::theme::pro`、`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Icon` | `QIcon` | 图标（QIcon 对象） |
| `FontIcon` | `QString` | 字体图标（Material Icons 代码） |
| `Color` | `Color` | 颜色样式（FILLED, TONAL, OUTLINED, STANDARD） |
| `Shape` | `Shape` | 形状（ROUND, SQUARE） |
| `Types` | `Types` | 类型（DEFAULT, TOGGLE_SELECTED, TOGGLE_UNSELECTED） |
| `Width` | `Width` | 宽度（DEFAULT, NARROW, WIDE） |
| `Clickable` | `[]{}` | 点击回调函数 |

预设尺寸常量：`kExtraSmallContainerSize`（32x32）、`kSmallContainerSize`（40x40）、`kMediumContainerSize`（56x56）、`kLargeContainerSize`（96x96）、`kExtraLargeContainerSize`（136x136）

```cpp
using namespace creeper;
namespace ibpro = icon_button::pro;

auto icon_button = new IconButton {
    ibpro::ThemeManager { manager },
    ibpro::FixedSize { 40, 40 },
    ibpro::Color { IconButton::Color::TONAL },
    ibpro::FontIcon { "search" },
    ibpro::Clickable { [] {
        qDebug() << "图标按钮被点击";
    }}
};

// 切换按钮
auto toggle_button = new IconButton {
    ibpro::ThemeManager { manager },
    ibpro::Types { IconButton::Types::TOGGLE_UNSELECTED },
    ibpro::FontIcon { "favorite" }
};
```

---

### MainWindow 主窗口

命名空间：`creeper::main_window::pro`

继承属性：`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Central` | `Widget*` | 设置中心组件 |

```cpp
using namespace creeper;
namespace mwpro = main_window::pro;

// 使用 ShowWindow 语法糖（推荐）
creeper::ShowWindow<MainWindow> {
    mwpro::MinimumSize { 1080, 720 },
    mwpro::Central<FilledCard> {
        card::pro::ThemeManager { manager },
        card::pro::Layout<Col> {
            // 窗口内容
        }
    }
};
```

---

### WaveCircle 波浪圆形

命名空间：`creeper::wave_circle::pro`

继承属性：`creeper::widget::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `FlangeNumber` | `uint8_t` | 波浪数量（凸起数量） |
| `FlangeRadius` | `double` | 波浪半径 |
| `OverallRadius` | `double` | 整体半径 |
| `ProtrudingRatio` | `double` | 凸起比例（0.0 - 1.0） |
| `Background` | `QColor` | 背景颜色 |
| `BorderWidth` | `double` | 边框宽度 |
| `BorderColor` | `QColor` | 边框颜色 |

```cpp
using namespace creeper;
namespace wcpro = wave_circle::pro;

auto wave_circle = new WaveCircle {
    wcpro::FixedSize { 150, 200 },
    wcpro::FlangeNumber { 8 },
    wcpro::FlangeRadius { 20 },
    wcpro::OverallRadius { 100 },
    wcpro::ProtrudingRatio { 0.8 },
    wcpro::Background { QColor("#2196F3") },
    wcpro::BorderWidth { 2 },
    wcpro::BorderColor { QColor("#FFFFFF") }
};
```

---

## 常见问题

**Q: 能否混合使用声明式和命令式 API？**

完全可以，所有组件都继承自标准 Qt 组件，支持传统方法调用：

```cpp
auto button = new FilledButton {
    button::pro::Text { "初始文本" }
};

button->setText("新文本");
button->setEnabled(false);
```

**Q: 如何创建自定义主题？**

参考内置主题的定义方式，详见[主题文档](./theme.md)。

---

## 相关文档

- [使用指南](./usage.md)
- [布局文档](./layout.md)
- [主题文档](./theme.md)
- [项目主页](../README.md)
- [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)
- [问题反馈](https://github.com/creeper5820/creeper-qt/issues)
