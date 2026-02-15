<div align="center">

<img src="https://r2.creeper5820.com/creeper-qt/creeper-qt.jpg" width="300" alt="CREEPER-QT Logo">

# CREEPER-QT

### 基于 Qt 的现代化 UI 包装库

[![GitHub last commit](https://img.shields.io/github/last-commit/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=9ccbfb)](https://github.com/creeper5820/creeper-qt)
[![GitHub Repo stars](https://img.shields.io/github/stars/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=b9c8da)](https://github.com/creeper5820/creeper-qt/stargazers)
[![GitHub repo size](https://img.shields.io/github/repo-size/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=d3bfe6)](https://github.com/creeper5820/creeper-qt)

<p>
  <a href="./doc/usage.md">使用指南</a> •
  <a href="./doc/widgets.md">组件文档</a> •
  <a href="./doc/layout.md">布局文档</a> •
  <a href="./doc/theme.md">主题文档</a> •
  <a href="./doc/api.md">API 参考</a> •
  <a href="https://www.bilibili.com/video/BV1JbxjzZEJ5">视频演示</a>
</p>

</div>

---

## ✨ 项目简介

`creeper-qt` 是基于 `Qt` 开发的 **轻量级 UI 集成库**，具有以下特点：

- **现代化设计** - 组件外观遵循 `Google Material Design3` 规范
- **声明式接口** - 改造传统命令式调用，组件构造和配置一步完成
- **主题系统** - 内置主题管理，支持明亮/黑暗模式及多种配色方案
- **流畅动画** - 基于 PID 控制器和弹簧模型的迭代算法，实现丝滑动效和无缝打断
- **开箱即用** - 提供跨平台示例程序，快速上手体验

> **欢迎贡献！** 期待您的 PR 和 ISSUE
>
> QQ交流群：885246539

---
## 📦 快速体验

### 下载示例程序

这里有一份自动构建的跨平台示例程序，无需编译即可体验完整功能。

- **[📥 下载最新版本 (GitHub Releases)](https://github.com/creeper5820/creeper-qt/releases/tag/nightly-example)**

### 平台支持

<table>
  <thead>
    <tr>
      <th width="150">操作系统</th>
      <th>安装与运行方式</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align="center"><strong>🐧 Linux</strong></td>
      <td>
        • 下载 <code>AppImage</code> 文件，赋予执行权限后直接运行<br>
        • 或下载 <code>.tar.gz</code> 压缩包，解压后执行 <code>AppRun</code> 文件
      </td>
    </tr>
    <tr>
      <td align="center"><strong>🪟 Windows</strong></td>
      <td>
        • 下载 <code>.zip</code> 压缩包并解压<br>
        • 进入文件夹，双击运行 <code>widgets.exe</code>
      </td>
    </tr>
  </tbody>
</table>

---

---

## 🎨 效果展示

<div align="center">

### 应用界面

<table>
  <tr>
    <td width="50%">
      <img src="https://r2.creeper5820.com/creeper-qt/MainWindow-Screenshot-2025-09-29_21-15-30.png" alt="主窗口明亮主题">
    </td>
    <td width="50%">
      <img src="https://r2.creeper5820.com/creeper-qt/MainWindow-Screenshot-2025-09-29_21-15-38.png" alt="主窗口黑暗主题">
    </td>
  </tr>
</table>

### 组件样式

<img src="https://r2.creeper5820.com/creeper-qt/blue-style-widgets.png" width="800" alt="组件样式展示">

### 动画效果

<table>
  <tr>
    <td width="50%" align="center">
      <strong>开关组件动画</strong><br><br>
      <img src="https://r2.creeper5820.com/creeper-qt/switch-working.gif" width="400" alt="开关动画">
    </td>
    <td width="50%" align="center">
      <strong>文本框动画</strong><br><br>
      <img src="https://r2.creeper5820.com/creeper-qt/filled-text-field.gif" width="400" alt="文本框动画">
    </td>
  </tr>
</table>

</div>

---

## 🚀 快速开始

### 安装配置

详细安装指南请参考：**[使用指南](./doc/usage.md)**

### CMake 集成

```cmake
cmake_minimum_required(VERSION 3.22)

project(hello-world)

# Qt 是项目依赖的库，记得导入
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(creeper-qt REQUIRED)

# Eigen 是 Header only 的，不用 find 也可以，只要保证
# 环境中能搜寻到头文件
# 如果只是二次开发的话，就不需要该库了
# Eigen 只在实现时用到了
# find_package(Eigen3 REQUIRED)

# 在 Windows 下, 安装目录如果没有暴露在环境变量, 
# 需要手动指定一下, 项目才能找到头文件
# dll 如果找不到，也需要手动指定一下 LIB 目录
# include_directories(D:/Software/msys2/usr/include/)

# 项目只用到了 MOC，下面功能有需要再开
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

add_executable(
    ${PROJECT_NAME}
    main.cc
)
target_link_libraries(
    ${PROJECT_NAME}
    creeper-qt::widgets
    Qt6::Widgets
)
```

### 代码示例

```cpp
#include < ... >

auto main(int argc, char* argv[]) -> int {
    using namespace creeper;

    // Qt 运行时初始化
    auto application = new QApplication { argc, argv };

    // 创建主题管理器，可以传入主题包
    auto manager = ThemeManager { kBlueMikuThemePack };

    // 和正常 Qt 一致的使用方式也是 OK 的，这里用声明式的方法示例
    creeper::ShowWindow<MainWindow> {
        mwpro::MinimumSize { 1080, 720 },
        mwpro::Central<FilledCard> {
            capro::ThemeManager { manager },
            capro::Radius { 0 },
            capro::Level { CardLevel::HIGHEST },

            capro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::Spacing { 0 },

                lnpro::Item {
                    // 某些自定义组件
                    NavComponent(nav_component_state),
                },
                lnpro::Item<Col> {
                    lnpro::ContentsMargin { { 15, 15, 5, 15 } },
                    lnpro::Item { ListComponent(list_component_state) },
                },
                lnpro::Item<Col> {
                    { 255 },
                    lnpro::ContentsMargin { { 5, 15, 15, 15 } },
                    lnpro::Item<ScrollArea> {
                        scroll::pro::ThemeManager { manager },
                        scroll::pro::HorizontalScrollBarPolicy {
                            Qt::ScrollBarAlwaysOff,
                        },
                        scroll::pro::Item {
                            ViewComponent(view_component_state),
                        },
                    },
                },
            },
            // More Widgets
            // ......
        },
    };

    // 将主题应用到注册过的组件中
    manager.apply_theme();

    return application->exec();
}
```

---

## 📋 开发路线图

- [ ] **版本发布**
  - 等接口稳定后 release 一个正式版本，开始迭代版本号
  - 接口设计目前基本稳定，多数修改不会影响下游项目

- [ ] **组件完善**
  - 全组件表格展示
  - 增加更多组件（至 2025.09.18，组件数量仍需扩充）
  - 按钮的禁用效果优化

- [ ] **布局容器**
  - 增加视图容器（原生容器不可用）
  - 目前已实现无动画的 Flow 布局

- [ ] **高级组件**
  - 提供日历模组组件
  - 提供设置中心示例

- [ ] 构建系统
    - 适配 module 导出
    - 提供开发脚手架

---

## 📊 项目统计

<div align="center">

[![Star History Chart](https://api.star-history.com/svg?repos=creeper5820/creeper-qt&type=Date)](https://www.star-history.com/#creeper5820/creeper-qt&Date)

</div>

---

## 📄 许可证

本项目采用 MIT 开源许可证，详见 [LICENSE](./LICENSE) 文件。

## 🤝 贡献

欢迎提交 Pull Request 和 Issue！

- 💬 **提出问题**: [GitHub Issues](https://github.com/creeper5820/creeper-qt/issues)
- 🔀 **贡献代码**: [GitHub Pull Requests](https://github.com/creeper5820/creeper-qt/pulls)
- 📖 **阅读文档**: [使用指南](./doc/usage.md) | [组件文档](./doc/widgets.md)
