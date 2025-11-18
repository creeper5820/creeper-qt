<div align="center">

# 🎨 CREEPER-QT 主题系统文档

### 完整的主题管理 API 参考与使用示例

[← 返回主页](./README.md) | [📖 使用指南](./usage.md) | [🧩 组件文档](./widgets.md)

</div>

---

## 📋 目录

- [🎨 CREEPER-QT 主题系统文档](#-creeper-qt-主题系统文档)
  - [完整的主题管理 API 参考与使用示例](#完整的主题管理-api-参考与使用示例)
  - [📋 目录](#-目录)
  - [🎯 核心概念](#-核心概念)
    - [ColorScheme (配色方案)](#colorscheme-配色方案)
    - [ThemePack (主题包)](#themepack-主题包)
    - [ThemeManager (主题管理器)](#thememanager-主题管理器)
  - [🎨 内置主题](#-内置主题)
    - [BlueMiku 主题](#bluemiku-主题)
    - [Green 主题](#green-主题)
    - [GoldenHarvest 主题](#goldenharvest-主题)
  - [🔧 使用主题](#-使用主题)
    - [基本使用](#基本使用)
    - [注册组件](#注册组件)
    - [切换主题](#切换主题)
    - [切换颜色模式](#切换颜色模式)
  - [🎨 自定义主题](#-自定义主题)
    - [创建配色方案](#创建配色方案)
    - [创建主题包](#创建主题包)
    - [完整示例](#完整示例)
  - [📚 相关文档](#-相关文档)

---

## 🎯 核心概念

### ColorScheme (配色方案)

`ColorScheme` 定义了单个颜色模式（明亮或黑暗）下的所有颜色值。

<div align="center">

**命名空间**: `creeper::ColorScheme`

</div>

主要颜色属性包括：

- `primary` - 主色调
- `onPrimary` - 主色调上的文字颜色
- `secondary` - 次要色调
- `onSecondary` - 次要色调上的文字颜色
- `background` - 背景色
- `onBackground` - 背景上的文字颜色
- `surface` - 表面色
- `onSurface` - 表面上的文字颜色
- `error` - 错误色
- `onError` - 错误色上的文字颜色
- ... 更多颜色属性

### ThemePack (主题包)

`ThemePack` 包含明亮和黑暗两种配色方案。

<div align="center">

**命名空间**: `creeper::ThemePack`

</div>

```cpp
struct ThemePack {
    ColorScheme light;  // 明亮模式配色
    ColorScheme dark;   // 黑暗模式配色
};
```

### ThemeManager (主题管理器)

`ThemeManager` 负责管理主题的切换和应用。

<div align="center">

**命名空间**: `creeper::ThemeManager`

</div>

主要功能：

- 管理当前主题包和颜色模式
- 注册组件的主题更新回调
- 统一应用主题到所有注册的组件

---

## 🎨 内置主题

CREEPER-QT 提供了三个内置主题包，可以直接使用。

### BlueMiku 主题

```cpp
using namespace creeper;

auto manager = ThemeManager { kBlueMikuThemePack };
```

<div align="center">

| 主题类型       | 主题名称                    |
| -------------- | --------------------------- |
| 🎨 **完整主题** | `kBlueMikuThemePack`        |
| 🌙 **暗色主题** | `kBlueMikuDarkColorScheme`  |
| ☀️ **亮色主题** | `kBlueMikuLightColorScheme` |

</div>

### Green 主题

```cpp
auto manager = ThemeManager { kGreenThemePack };
```

### GoldenHarvest 主题

```cpp
auto manager = ThemeManager { kGoldenHarvestThemePack };
```

---

## 🔧 使用主题

### 基本使用

```cpp
#include "creeper-qt/utility/theme/preset/blue-miku.hh"
using namespace creeper;

// 创建主题管理器
auto manager = ThemeManager { kBlueMikuThemePack };

// 创建组件并注册主题
auto button = new FilledButton {
    util::theme::pro::ThemeManager { manager },
    button::pro::Text { "按钮" }
};

// 应用主题到所有注册的组件
manager.apply_theme();
```

### 注册组件

组件可以通过两种方式注册到主题管理器：

#### 方式一：使用 ThemeManager 属性（推荐）

```cpp
auto button = new FilledButton {
    util::theme::pro::ThemeManager { manager },
    button::pro::Text { "按钮" }
};
```

#### 方式二：手动注册

```cpp
auto button = new FilledButton {
    button::pro::Text { "按钮" }
};

// 手动注册主题更新回调
manager.append_handler(button, [button](const ThemeManager& manager) {
    // 自定义主题更新逻辑
    button->set_color_scheme(manager.color_scheme());
});
```

### 切换主题

```cpp
// 切换到不同的主题包
manager.set_theme_pack(kGreenThemePack);

// 应用新主题
manager.apply_theme();
```

### 切换颜色模式

```cpp
// 切换到黑暗模式
manager.set_color_mode(ColorMode::DARK);

// 或切换模式
manager.toggle_color_mode();

// 应用主题
manager.apply_theme();
```

---

## 🎨 自定义主题

### 创建配色方案

```cpp
using namespace creeper;

ColorScheme my_light_scheme {
    .primary = QColor("#2196F3"),
    .onPrimary = QColor("#FFFFFF"),
    .secondary = QColor("#03DAC6"),
    .onSecondary = QColor("#000000"),
    .background = QColor("#FFFFFF"),
    .onBackground = QColor("#000000"),
    .surface = QColor("#FFFFFF"),
    .onSurface = QColor("#000000"),
    .error = QColor("#B00020"),
    .onError = QColor("#FFFFFF"),
    // ... 更多颜色属性
};
```

### 创建主题包

```cpp
const ThemePack kMyThemePack {
    .light = my_light_scheme,
    .dark = my_dark_scheme
};
```

### 完整示例

```cpp
#include "creeper-qt/utility/theme/theme.hh"
using namespace creeper;

// 定义明亮模式配色
const ColorScheme my_light_scheme {
    .primary = QColor("#6200EE"),
    .onPrimary = QColor("#FFFFFF"),
    .secondary = QColor("#03DAC6"),
    .onSecondary = QColor("#000000"),
    .background = QColor("#FFFFFF"),
    .onBackground = QColor("#000000"),
    .surface = QColor("#FFFFFF"),
    .onSurface = QColor("#000000"),
    .error = QColor("#B00020"),
    .onError = QColor("#FFFFFF"),
};

// 定义黑暗模式配色
const ColorScheme my_dark_scheme {
    .primary = QColor("#BB86FC"),
    .onPrimary = QColor("#000000"),
    .secondary = QColor("#03DAC6"),
    .onSecondary = QColor("#000000"),
    .background = QColor("#121212"),
    .onBackground = QColor("#FFFFFF"),
    .surface = QColor("#1E1E1E"),
    .onSurface = QColor("#FFFFFF"),
    .error = QColor("#CF6679"),
    .onError = QColor("#000000"),
};

// 创建主题包
const ThemePack kMyCustomThemePack {
    .light = my_light_scheme,
    .dark = my_dark_scheme
};

// 使用自定义主题
auto manager = ThemeManager { kMyCustomThemePack, ColorMode::LIGHT };

auto button = new FilledButton {
    util::theme::pro::ThemeManager { manager },
    button::pro::Text { "自定义主题按钮" }
};

manager.apply_theme();
```

---

## 📚 相关文档

<div align="center">

| 文档       | 链接                                                              |
| ---------- | ----------------------------------------------------------------- |
| 📖 使用指南 | [usage.md](./usage.md)                                            |
| 🧩 组件文档 | [widgets.md](./widgets.md)                                        |
| 📐 布局文档 | [layout.md](./layout.md)                                          |
| 🏠 项目主页 | [README.md](../README.md)                                         |
| 🎬 视频演示 | [Bilibili](https://www.bilibili.com/video/BV1JbxjzZEJ5)           |
| 💬 问题反馈 | [GitHub Issues](https://github.com/creeper5820/creeper-qt/issues) |

</div>

---

<div align="center">

**需要更多帮助？** 

[📖 查看使用指南](./usage.md) | [💬 提交 Issue](https://github.com/creeper5820/creeper-qt/issues) | [🌟 给项目点赞](https://github.com/creeper5820/creeper-qt)

[⬆️ 返回顶部](#-creeper-qt-主题系统文档)

</div>

