# CREEPER-QT 主题系统文档

[返回主页](../README.md) | [使用指南](./usage.md) | [组件文档](./widgets.md)

---

## 核心概念

### ColorScheme（配色方案）

`ColorScheme` 定义了单个颜色模式（明亮或黑暗）下的所有颜色值。

命名空间：`creeper::ColorScheme`

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

### ThemePack（主题包）

`ThemePack` 包含明亮和黑暗两种配色方案。

命名空间：`creeper::ThemePack`

```cpp
struct ThemePack {
    ColorScheme light;  // 明亮模式配色
    ColorScheme dark;   // 黑暗模式配色
};
```

### ThemeManager（主题管理器）

`ThemeManager` 负责管理主题的切换和应用。

命名空间：`creeper::ThemeManager`

主要功能：

- 管理当前主题包和颜色模式
- 注册组件的主题更新回调
- 统一应用主题到所有注册的组件

---

## 内置主题

CREEPER-QT 提供了三个内置主题包，可以直接使用。

### BlueMiku 主题

```cpp
using namespace creeper;

auto manager = ThemeManager { kBlueMikuThemePack };
```

| 主题类型 | 主题名称 |
| --- | --- |
| 完整主题 | `kBlueMikuThemePack` |
| 暗色主题 | `kBlueMikuDarkColorScheme` |
| 亮色主题 | `kBlueMikuLightColorScheme` |

### Green 主题

```cpp
auto manager = ThemeManager { kGreenThemePack };
```

### GoldenHarvest 主题

```cpp
auto manager = ThemeManager { kGoldenHarvestThemePack };
```

---

## 使用主题

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

manager.append_handler(button, [button](const ThemeManager& manager) {
    button->set_color_scheme(manager.color_scheme());
});
```

### 切换主题

```cpp
manager.set_theme_pack(kGreenThemePack);
manager.apply_theme();
```

### 切换颜色模式

```cpp
manager.set_color_mode(ColorMode::DARK);
// 或
manager.toggle_color_mode();

manager.apply_theme();
```

---

## 自定义主题

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
};
```

### 完整示例

```cpp
#include "creeper-qt/utility/theme/theme.hh"
using namespace creeper;

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

const ThemePack kMyCustomThemePack {
    .light = my_light_scheme,
    .dark = my_dark_scheme
};

auto manager = ThemeManager { kMyCustomThemePack, ColorMode::LIGHT };

auto button = new FilledButton {
    util::theme::pro::ThemeManager { manager },
    button::pro::Text { "自定义主题按钮" }
};

manager.apply_theme();
```

---

## 相关文档

- [使用指南](./usage.md)
- [组件文档](./widgets.md)
- [布局文档](./layout.md)
- [项目主页](../README.md)
- [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)
- [问题反馈](https://github.com/creeper5820/creeper-qt/issues)
