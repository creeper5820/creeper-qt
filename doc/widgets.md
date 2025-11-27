<div align="center">

# 🧩 CREEPER-QT 组件文档

### 完整的组件 API 参考与使用示例

[← 返回主页](./README.md) | [📖 使用指南](./usage.md)

</div>

---

## 🎨 通用组件属性

<div align="center">

**命名空间**: `creeper::widget::pro`

</div>

### 属性列表

<table>
  <thead>
    <tr>
      <th width="200">属性 (PROPERTY)</th>
      <th width="150">类型 (TYPE)</th>
      <th>方法 (METHOD)</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>MinimumSize</code></td>
      <td><code>QSize</code></td>
      <td><code>setMinimumSize</code></td>
    </tr>
    <tr>
      <td><code>MaximumSize</code></td>
      <td><code>QSize</code></td>
      <td><code>setMaximumSize</code></td>
    </tr>
    <tr>
      <td><code>SizeIncrement</code></td>
      <td><code>QSize</code></td>
      <td><code>setSizeIncrement</code></td>
    </tr>
    <tr>
      <td><code>BaseSize</code></td>
      <td><code>QSize</code></td>
      <td><code>setBaseSize</code></td>
    </tr>
    <tr>
      <td><code>FixedSize</code></td>
      <td><code>QSize</code></td>
      <td><code>setFixedSize</code></td>
    </tr>
    <tr>
      <td colspan="3" align="center"><i>...更多 QWidget 属性...</i></td>
    </tr>
  </tbody>
</table>

> 💡 **说明**: 上述属性是将 `QWidget` 的大部分 Setter 按照本项目风格进行的封装。后续文档将省略 `METHOD` 列，因为属性名加 `set` 前缀即为对应方法名。

### 📝 调用示例

#### 声明式配置（推荐）

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;

auto widget = new Widget {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
    // ... 其他 QWidget 的 Setter
};
```

#### 等价的传统写法

```cpp
auto qwidget = new QWidget {};
qwidget->setMaximumSize(200, 100);
qwidget->setMinimumSize(100, 050);
// ...
```

### 🔄 属性复用

声明式配置的优势在于可以方便地复用属性组合：

```cpp
namespace pro = creeper::widget::pro;
using creeper::Widget;

// 使用 tuple 打包通用属性
const auto props = std::tuple {
    pro::MaximumSize { 200, 100 },
    pro::MinimumSize { 100, 050 },
};

// 方式 1: 直接使用 tuple 构造
auto widget_a = new Widget { props };

// 方式 2: 在 tuple 基础上添加差异化配置
auto widget_b = new Widget {
    props,
    pro::WindowFlag { Qt::WindowStaysOnTopHint },
};
```

### ✨ 不可变对象

配置完成后，可以将组件声明为 `const`：

```cpp
const auto widget = new Widget { 
    pro::FixedSize { 300, 200 },
    // ... 其他配置
};
```

> 📌 **注意**: `Widget` 完全继承自 `QWidget`，只是附加了声明式构造方法，**与传统调用方式完全兼容**。

---

## 🔘 按钮组件

<div align="center">

<img src="https://r2.creeper5820.com/creeper-qt/blue-style-widgets.png" width="800" alt="按钮样式展示">

### 内置主题

| 主题类型       | 主题名称                    |
| -------------- | --------------------------- |
| 🎨 **完整主题** | `kBlueMikuThemePack`        |
| 🌙 **暗色主题** | `kBlueMikuDarkColorScheme`  |
| ☀️ **亮色主题** | `kBlueMikuLightColorScheme` |

</div>

### 通用按钮属性

<div align="center">

**命名空间**: `creeper::button::pro`

</div>

#### 属性定义

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

#### 属性表

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Text</code></td>
      <td><code>QString</code></td>
      <td>按钮文本内容</td>
    </tr>
    <tr>
      <td><code>TextColor</code></td>
      <td><code>QColor</code></td>
      <td>文本颜色</td>
    </tr>
    <tr>
      <td><code>Radius</code></td>
      <td><code>double</code></td>
      <td>圆角半径</td>
    </tr>
    <tr>
      <td><code>BorderWidth</code></td>
      <td><code>double</code></td>
      <td>边框宽度</td>
    </tr>
    <tr>
      <td><code>BorderColor</code></td>
      <td><code>QColor</code></td>
      <td>边框颜色</td>
    </tr>
    <tr>
      <td><code>Background</code></td>
      <td><code>QColor</code></td>
      <td>背景颜色</td>
    </tr>
    <tr>
      <td><code>WaterColor</code></td>
      <td><code>QColor</code></td>
      <td>水波纹颜色</td>
    </tr>
    <tr>
      <td><code>Clickable</code></td>
      <td><code>[](self){}</code></td>
      <td>点击回调函数</td>
    </tr>
  </tbody>
</table>

---

### FilledButton

<div align="center">

**命名空间**: `creeper::filled_button::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`
- ✅ `creeper::button::pro`

#### 使用示例

```cpp
using namespace creeper;

auto button = new FilledButton {
    button::pro::Text { "提交" },
    button::pro::Radius { 8.0 },
    button::pro::Background { QColor("#2196F3") },
    button::pro::Clickable { [](auto& self) {
        qDebug() << "按钮被点击！";
    }}
};
```

---

### FilledTonalButton

<div align="center">

**命名空间**: `creeper::filled_tonal_button::pro`

> 💡 与 `FilledButton` 相同的 API，仅配色方案不同

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`
- ✅ `creeper::button::pro`

---

### OutlinedButton

<div align="center">

**命名空间**: `creeper::outlined_button::pro`

> 💡 与 `FilledButton` 相同的 API，仅配色方案不同

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`
- ✅ `creeper::button::pro`

---

### TextButton

<div align="center">

**命名空间**: `creeper::text_button::pro`

> 💡 与 `FilledButton` 相同的 API，仅配色方案不同

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`
- ✅ `creeper::button::pro`

---

### 🔄 属性组合复用

由于属性通过 `concept` 约束定义，以下等价关系成立：

```cpp
static_assert(
    std::same_as<filled_button::pro::Text, button::pro::Text>,
    "属性类型相同"
);

static_assert(
    std::same_as<filled_button::pro::Radius, button::pro::Radius>,
    "属性类型相同"
);
// ... 其他属性同理
```

因此，可以定义通用属性集合并应用到任意按钮类型：

```cpp
// 定义通用属性
const auto properties = std::tuple {
    util::theme::pro::ThemeManager { theme_manager },
    widget::pro::FixedSize { 100, 50 },
    widget::pro::Font { "JetBrains Mono", 12 },
    button::pro::Text { "你好世界" },
    button::pro::Radius { 25 },
};

// 应用到不同类型的按钮
auto filled_button = FilledButton { properties };
auto outlined_button = OutlinedButton { properties };
auto text_button = TextButton { properties };
```

---

## ⚡ 交互组件

### Switch 开关

<div align="center">

<img src="https://r2.creeper5820.com/creeper-qt/switch-working.gif" width="500" alt="Switch 组件动画">

**命名空间**: `creeper::_switch::pro`

</div>

#### 🎯 设计说明

> 实现此组件时经过多次权衡。原 Material Design 3 的 Switch 规则过于复杂，一些参数（如 Handle 的膨胀拉伸形变系数）未给出明确定义。基于曲线函数的动画在打断时表现不自然。
> 
> 经过再三斟酌，最终决定：
> - ✅ 大体复现 MD3 外观设计
> - ✅ 使用弹簧物理模拟替代曲线动画（打断效果更自然）
> - ❌ 简化按压和拉伸动画（保持核心交互体验）
> 
> 最终效果令人满意，但未来有精力时会考虑完全复现原始设计。

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

> 📖 **完整属性列表**: 查看 [源代码](../creeper-qt/widget/switch.hh#L60-L106) 获取所有可用属性

主要属性包括：

- `Clickable` - 点击回调函数
- `Checked` / `Disabled` - 状态控制
- `TrackColor*` - 轨道颜色系列（Checked/Unchecked/Disabled 状态）
- `HandleColor*` - 手柄颜色系列（Checked/Unchecked/Disabled 状态）
- `OutlineColor*` - 轮廓颜色系列（Checked/Unchecked/Disabled 状态）
- `HoverColor*` - 悬停颜色系列（Checked/Unchecked 状态）

#### 使用示例

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

**命名空间**: `creeper::text_field::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>LabelText</code></td>
      <td><code>QString</code></td>
      <td>标签文本（浮动标签）</td>
    </tr>
    <tr>
      <td><code>LeadingIcon</code></td>
      <td><code>QString, QString</code></td>
      <td>前置图标（图标代码，字体名称）</td>
    </tr>
    <tr>
      <td><code>ClearButton</code></td>
      <td><code>bool</code></td>
      <td>是否显示清除按钮</td>
    </tr>
    <tr>
      <td><code>OnTextChanged</code></td>
      <td><code>[](const QString&){}</code></td>
      <td>文本改变时的回调函数</td>
    </tr>
    <tr>
      <td><code>OnEditingFinished</code></td>
      <td><code>[]{}</code></td>
      <td>编辑完成时的回调函数</td>
    </tr>
    <tr>
      <td><code>OnChanged</code></td>
      <td><code>[](const QString&){}</code></td>
      <td>文本改变时的回调函数（OnTextChanged 的别名）</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;

// 基本使用
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

<div align="center">

**命名空间**: `creeper::text_field::pro`

> 💡 与 `FilledTextField` 相同的 API，仅外观样式不同（带边框）

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 使用示例

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

<div align="center">

**命名空间**: `creeper::slider::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Progress</code></td>
      <td><code>double</code></td>
      <td>进度值（0.0 - 1.0）</td>
    </tr>
    <tr>
      <td><code>Measurements</code></td>
      <td><code>Measurements</code></td>
      <td>尺寸配置（支持 Xs, S, M, L, SL 预设）</td>
    </tr>
    <tr>
      <td><code>OnValueChange</code></td>
      <td><code>[](double){}</code></td>
      <td>值改变时的回调函数</td>
    </tr>
    <tr>
      <td><code>OnValueChangeFinished</code></td>
      <td><code>[](double){}</code></td>
      <td>值改变完成时的回调函数</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;

// 基本使用
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

<div align="center">

**命名空间**: `creeper::dropdown_menu::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>LabelText</code></td>
      <td><code>QString</code></td>
      <td>标签文本</td>
    </tr>
    <tr>
      <td><code>LeadingIcon</code></td>
      <td><code>QString, QString</code></td>
      <td>前置图标（图标代码，字体名称）</td>
    </tr>
    <tr>
      <td><code>Items</code></td>
      <td><code>QVector&lt;QString&gt;</code></td>
      <td>选项列表</td>
    </tr>
    <tr>
      <td><code>IndexChanged</code></td>
      <td><code>[](int){}</code></td>
      <td>选中项改变时的回调函数</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;

// 基本使用
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

<div align="center">

**命名空间**: `creeper::select_widget::pro`

> 💡 与 `FilledDropdownMenu` 功能类似，但外观和交互略有不同

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>LabelText</code></td>
      <td><code>QString</code></td>
      <td>标签文本</td>
    </tr>
    <tr>
      <td><code>LeadingIcon</code></td>
      <td><code>QString, QString</code></td>
      <td>前置图标（默认使用下拉箭头图标）</td>
    </tr>
    <tr>
      <td><code>Items</code></td>
      <td><code>QVector&lt;QString&gt;</code></td>
      <td>选项列表</td>
    </tr>
    <tr>
      <td><code>IndexChanged</code></td>
      <td><code>[](int){}</code></td>
      <td>选中项改变时的回调函数</td>
    </tr>
  </tbody>
</table>

#### 使用示例

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

<div align="center">

**命名空间**: `creeper::image::pro`

</div>

#### 继承属性

- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>PainterResource</code></td>
      <td><code>std::shared_ptr&lt;PainterResource&gt;</code></td>
      <td>图片资源（支持 QPixmap, QImage 等）</td>
    </tr>
    <tr>
      <td><code>Pixmap</code></td>
      <td><code>QPixmap</code></td>
      <td>设置图片（PainterResource 的别名）</td>
    </tr>
    <tr>
      <td><code>ContentScale</code></td>
      <td><code>ContentScale</code></td>
      <td>内容缩放模式</td>
    </tr>
    <tr>
      <td><code>Opacity</code></td>
      <td><code>double</code></td>
      <td>透明度（0.0 - 1.0）</td>
    </tr>
    <tr>
      <td><code>Radius</code></td>
      <td><code>double</code></td>
      <td>圆角半径</td>
    </tr>
    <tr>
      <td><code>BorderWidth</code></td>
      <td><code>double</code></td>
      <td>边框宽度</td>
    </tr>
    <tr>
      <td><code>BorderColor</code></td>
      <td><code>QColor</code></td>
      <td>边框颜色</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;

// 基本使用
auto image = new Image {
    image::pro::Pixmap { QPixmap("path/to/image.png") },
    image::pro::FixedSize { 200, 200 },
    image::pro::Radius { 10 },
    image::pro::BorderWidth { 2 },
    image::pro::BorderColor { QColor("#000000") }
};

// 使用 ContentScale
auto scaled_image = new Image {
    image::pro::Pixmap { QPixmap("path/to/image.png") },
    image::pro::ContentScale { ContentScale::Fit },
    image::pro::FixedSize { 300, 300 }
};
```

---

### Text 文本

<div align="center">

**命名空间**: `creeper::text::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Text</code></td>
      <td><code>QString</code></td>
      <td>文本内容</td>
    </tr>
    <tr>
      <td><code>Color</code></td>
      <td><code>QColor</code></td>
      <td>文本颜色</td>
    </tr>
    <tr>
      <td><code>Alignment</code></td>
      <td><code>Qt::Alignment</code></td>
      <td>文本对齐方式</td>
    </tr>
    <tr>
      <td><code>WordWrap</code></td>
      <td><code>bool</code></td>
      <td>是否自动换行</td>
    </tr>
    <tr>
      <td><code>TextInteractionFlags</code></td>
      <td><code>Qt::TextInteractionFlags</code></td>
      <td>文本交互标志（如可选择、可编辑等）</td>
    </tr>
    <tr>
      <td><code>AdjustSize</code></td>
      <td>-</td>
      <td>自动调整大小以适应内容</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;

// 基本使用
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

<div align="center">

**命名空间**: `creeper::card::pro`

</div>

卡片组件提供多种样式：`FilledCard`、`OutlinedCard`、`ElevatedCard`、`BasicCard`。

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`
- ✅ `creeper::rounded_rect::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Level</code></td>
      <td><code>CardLevel</code></td>
      <td>卡片层级（DEFAULT, HIGH, HIGHEST, LOW, LOWEST）</td>
    </tr>
    <tr>
      <td><code>LevelDefault</code></td>
      <td>-</td>
      <td>默认层级（常量）</td>
    </tr>
    <tr>
      <td><code>LevelHigh</code></td>
      <td>-</td>
      <td>高层级（常量）</td>
    </tr>
    <tr>
      <td><code>LevelHighest</code></td>
      <td>-</td>
      <td>最高层级（常量）</td>
    </tr>
    <tr>
      <td><code>LevelLow</code></td>
      <td>-</td>
      <td>低层级（常量）</td>
    </tr>
    <tr>
      <td><code>LevelLowest</code></td>
      <td>-</td>
      <td>最低层级（常量）</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;
namespace capro = card::pro;

// FilledCard - 填充卡片
auto filled_card = new FilledCard {
    capro::ThemeManager { manager },
    capro::Level { CardLevel::HIGHEST },
    capro::Radius { 12 },
    capro::FixedSize { 200, 150 },
    capro::Layout<Col> {
        // 卡片内容
    }
};

// OutlinedCard - 轮廓卡片
auto outlined_card = new OutlinedCard {
    capro::ThemeManager { manager },
    capro::LevelLowest,
    capro::Radius { 8 },
    capro::Layout<Row> {
        // 卡片内容
    }
};

// ElevatedCard - 提升卡片（带阴影）
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

<div align="center">

**命名空间**: `creeper::icon_button::pro`

</div>

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Icon</code></td>
      <td><code>QIcon</code></td>
      <td>图标（QIcon 对象）</td>
    </tr>
    <tr>
      <td><code>FontIcon</code></td>
      <td><code>QString</code></td>
      <td>字体图标（Material Icons 代码）</td>
    </tr>
    <tr>
      <td><code>Color</code></td>
      <td><code>Color</code></td>
      <td>颜色样式（FILLED, TONAL, OUTLINED, STANDARD）</td>
    </tr>
    <tr>
      <td><code>Shape</code></td>
      <td><code>Shape</code></td>
      <td>形状（ROUND, SQUARE）</td>
    </tr>
    <tr>
      <td><code>Types</code></td>
      <td><code>Types</code></td>
      <td>类型（DEFAULT, TOGGLE_SELECTED, TOGGLE_UNSELECTED）</td>
    </tr>
    <tr>
      <td><code>Width</code></td>
      <td><code>Width</code></td>
      <td>宽度（DEFAULT, NARROW, WIDE）</td>
    </tr>
    <tr>
      <td><code>Clickable</code></td>
      <td><code>[]{}</code></td>
      <td>点击回调函数</td>
    </tr>
  </tbody>
</table>

#### 预设尺寸常量

- `kExtraSmallContainerSize` - 32x32
- `kSmallContainerSize` - 40x40
- `kMediumContainerSize` - 56x56
- `kLargeContainerSize` - 96x96
- `kExtraLargeContainerSize` - 136x136

#### 使用示例

```cpp
using namespace creeper;
namespace ibpro = icon_button::pro;

// 基本使用
auto icon_button = new IconButton {
    ibpro::ThemeManager { manager },
    ibpro::FixedSize { 40, 40 },
    ibpro::Color { IconButton::Color::TONAL },
    ibpro::FontIcon { "search" },
    ibpro::Clickable { [] {
        qDebug() << "图标按钮被点击";
    }}
};

// 使用 Material Icons
auto material_button = new IconButton {
    ibpro::ThemeManager { manager },
    ibpro::FixedSize { 40, 40 },
    ibpro::Color { IconButton::Color::TONAL },
    ibpro::Font { material::kRoundSmallFont },
    ibpro::FontIcon { "change_circle" }
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

<div align="center">

**命名空间**: `creeper::main_window::pro`

</div>

#### 继承属性

- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Central</code></td>
      <td><code>Widget*</code></td>
      <td>设置中心组件</td>
    </tr>
  </tbody>
</table>

#### 使用示例

```cpp
using namespace creeper;
namespace mwpro = main_window::pro;

// 基本使用
auto main_window = new MainWindow {
    mwpro::MinimumSize { 1080, 720 },
    mwpro::Central<FilledCard> {
        card::pro::ThemeManager { manager },
        card::pro::Layout<Col> {
            // 窗口内容
        }
    }
};
main_window->show();

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

<div align="center">

**命名空间**: `creeper::wave_circle::pro`

</div>

#### 继承属性

- ✅ `creeper::widget::pro`

#### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>FlangeNumber</code></td>
      <td><code>uint8_t</code></td>
      <td>波浪数量（凸起数量）</td>
    </tr>
    <tr>
      <td><code>FlangeRadius</code></td>
      <td><code>double</code></td>
      <td>波浪半径</td>
    </tr>
    <tr>
      <td><code>OverallRadius</code></td>
      <td><code>double</code></td>
      <td>整体半径</td>
    </tr>
    <tr>
      <td><code>ProtrudingRatio</code></td>
      <td><code>double</code></td>
      <td>凸起比例（0.0 - 1.0）</td>
    </tr>
    <tr>
      <td><code>Background</code></td>
      <td><code>QColor</code></td>
      <td>背景颜色</td>
    </tr>
    <tr>
      <td><code>BorderWidth</code></td>
      <td><code>double</code></td>
      <td>边框宽度</td>
    </tr>
    <tr>
      <td><code>BorderColor</code></td>
      <td><code>QColor</code></td>
      <td>边框颜色</td>
    </tr>
  </tbody>
</table>

#### 使用示例

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

## 📚 相关文档

<div align="center">

| 文档       | 链接                                                              |
| ---------- | ----------------------------------------------------------------- |
| 📖 使用指南 | [usage.md](./usage.md)                                            |
| 📐 布局文档 | [layout.md](./layout.md)                                          |
| 🎨 主题文档 | [theme.md](./theme.md)                                            |
| 📚 API 参考 | [api.md](./api.md)                                                |
| 🏠 项目主页 | [README.md](../README.md)                                         |
| 🎬 视频演示 | [Bilibili](https://www.bilibili.com/video/BV1JbxjzZEJ5)           |
| 💬 问题反馈 | [GitHub Issues](https://github.com/creeper5820/creeper-qt/issues) |

</div>

---

## ❓ 常见问题

<details>
<summary><b>Q: 能否混合使用声明式和命令式 API？</b></summary>

完全可以！所有组件都继承自标准 Qt 组件，支持传统方法调用：

```cpp
auto button = new FilledButton {
    button::pro::Text { "初始文本" }
};

// 后续使用传统方式修改
button->setText("新文本");
button->setEnabled(false);
```

</details>

<details>
<summary><b>Q: 如何创建自定义主题？</b></summary>

参考内置主题的定义方式：

```cpp
const creeper::ThemePack kMyTheme {
    .light = creeper::ColorScheme {
        // 亮色模式配色
    },
    .dark = creeper::ColorScheme {
        // 暗色模式配色
    }
};
```

</details>

---

<div align="center">

**需要更多帮助？** 

[📖 查看使用指南](./usage.md) | [💬 提交 Issue](https://github.com/creeper5820/creeper-qt/issues) | [🌟 给项目点赞](https://github.com/creeper5820/creeper-qt)

[⬆️ 返回顶部](#-creeper-qt-组件文档)

</div>
