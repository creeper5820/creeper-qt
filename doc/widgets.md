<div align="center">

# 🧩 CREEPER-QT 组件文档

### 完整的组件 API 参考与使用示例

[← 返回主页](./README.md) | [📖 使用指南](./usage.md)

</div>

---

## 📋 目录

- [🧩 CREEPER-QT 组件文档](#-creeper-qt-组件文档)
    - [完整的组件 API 参考与使用示例](#完整的组件-api-参考与使用示例)
  - [📋 目录](#-目录)
  - [🎨 通用组件属性](#-通用组件属性)
    - [属性列表](#属性列表)
    - [📝 调用示例](#-调用示例)
      - [声明式配置（推荐）](#声明式配置推荐)
      - [等价的传统写法](#等价的传统写法)
    - [🔄 属性复用](#-属性复用)
    - [✨ 不可变对象](#-不可变对象)
  - [🔘 按钮组件](#-按钮组件)
    - [内置主题](#内置主题)
    - [通用按钮属性](#通用按钮属性)
      - [属性定义](#属性定义)
      - [属性表](#属性表)
    - [FilledButton](#filledbutton)
      - [继承属性](#继承属性)
      - [使用示例](#使用示例)
    - [FilledTonalButton](#filledtonalbutton)
      - [继承属性](#继承属性-1)
    - [OutlinedButton](#outlinedbutton)
      - [继承属性](#继承属性-2)
    - [TextButton](#textbutton)
      - [继承属性](#继承属性-3)
    - [🔄 属性组合复用](#-属性组合复用)
  - [⚡ 交互组件](#-交互组件)
    - [Switch 开关](#switch-开关)
      - [🎯 设计说明](#-设计说明)
      - [继承属性](#继承属性-4)
      - [专有属性](#专有属性)
      - [使用示例](#使用示例-1)
    - [FilledTextField 文本框](#filledtextfield-文本框)
      - [继承属性](#继承属性-5)
      - [基本使用](#基本使用)
  - [📚 相关文档](#-相关文档)
  - [❓ 常见问题](#-常见问题)

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

<img src="https://creeper5820.com/creeper-qt/blue-style-widgets.png" width="800" alt="按钮样式展示">

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

<img src="https://creeper5820.com/creeper-qt/switch-working.gif" width="500" alt="Switch 组件动画">

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

<table>
  <thead>
    <tr>
      <th width="280">属性名</th>
      <th width="180">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>TrackColorUnchecked</code></td>
      <td><code>QColor</code></td>
      <td>轨道颜色 - 未选中</td>
    </tr>
    <tr>
      <td><code>TrackColorChecked</code></td>
      <td><code>QColor</code></td>
      <td>轨道颜色 - 已选中</td>
    </tr>
    <tr>
      <td><code>TrackColorUncheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>轨道颜色 - 未选中禁用</td>
    </tr>
    <tr>
      <td><code>TrackColorCheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>轨道颜色 - 已选中禁用</td>
    </tr>
    <tr>
      <td><code>HandleColorUnchecked</code></td>
      <td><code>QColor</code></td>
      <td>手柄颜色 - 未选中</td>
    </tr>
    <tr>
      <td><code>HandleColorChecked</code></td>
      <td><code>QColor</code></td>
      <td>手柄颜色 - 已选中</td>
    </tr>
    <tr>
      <td><code>HandleColorUncheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>手柄颜色 - 未选中禁用</td>
    </tr>
    <tr>
      <td><code>HandleColorCheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>手柄颜色 - 已选中禁用</td>
    </tr>
    <tr>
      <td><code>OutlineColorUnchecked</code></td>
      <td><code>QColor</code></td>
      <td>轮廓颜色 - 未选中</td>
    </tr>
    <tr>
      <td><code>OutlineColorChecked</code></td>
      <td><code>QColor</code></td>
      <td>轮廓颜色 - 已选中</td>
    </tr>
    <tr>
      <td><code>OutlineColorUncheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>轮廓颜色 - 未选中禁用</td>
    </tr>
    <tr>
      <td><code>OutlineColorCheckedDisabled</code></td>
      <td><code>QColor</code></td>
      <td>轮廓颜色 - 已选中禁用</td>
    </tr>
    <tr>
      <td><code>HoverColorUnchecked</code></td>
      <td><code>QColor</code></td>
      <td>悬停颜色 - 未选中</td>
    </tr>
    <tr>
      <td><code>HoverColorChecked</code></td>
      <td><code>QColor</code></td>
      <td>悬停颜色 - 已选中</td>
    </tr>
    <tr>
      <td><code>Clickable</code></td>
      <td><code>[](auto& self){}</code></td>
      <td>点击回调函数</td>
    </tr>
  </tbody>
</table>

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

<img src="https://creeper5820.com/creeper-qt/filled-text-field.gif" width="500" alt="FilledTextField 组件动画">

**命名空间**: `creeper::filled_text_field::pro`

</div>

> 🚧 **文档完善中** - 详细 API 文档即将补充

#### 继承属性

- ✅ `creeper::util::theme::pro`
- ✅ `creeper::widget::pro`

#### 基本使用

```cpp
using namespace creeper;

auto text_field = new FilledTextField {
    widget::pro::FixedSize { 250, 56 },
    // 更多属性配置...
};
```

---

## 📚 相关文档

<div align="center">

| 文档       | 链接                                                              |
| ---------- | ----------------------------------------------------------------- |
| 📖 使用指南 | [usage.md](./usage.md)                                            |
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
