<div align=center>

<div align=center><img src="https://creeper5820.com/creeper-qt/creeper-qt.jpg" width=400></div>

<h1>CREEPER-QT</h1>

[使用指南](./doc/usage.md) | [组件文档](./doc/widgets.md) | [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)

![GitHub last commit](https://img.shields.io/github/last-commit/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=9ccbfb) ![GitHub Repo stars](https://img.shields.io/github/stars/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=b9c8da) ![GitHub repo size](https://img.shields.io/github/repo-size/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=d3bfe6)

`creeper-qt` 是基于 `Qt` 开发的 UI 集成库，轻量简洁，组件外观大体遵循 `Google Material Design3`，同时改造了老式的命令式调用，使组件构造和配置可以在同一时间完成，添加了主题管理，支持切换明亮和黑暗主题和各种配色，动效部分基于 PID 控制器和弹簧模型等迭代算法，带来更流畅的动画体验和打断效果

欢迎 PR 和 ISSUE！

</div>

## 📦 示例程序（example）使用说明

本仓库自动构建并发布了跨平台的 **示例程序**，方便快速体验项目功能。

🔗 最新版本下载地址：  
👉 [GitHub Releases 页面](https://github.com/creeper5820/creeper-qt/releases)

| 操作系统 | 安装与运行方式 |
|----------|----------------|
| **Linux** | - 下载 `AppImage` 后赋予执行权限并运行<br>- 或解压 `.tar.gz` 后执行其中的 `AppRun` 文件 |
| **Windows** | - 解压 `zip` 后进入文件夹，双击运行 `widgets.exe` 即可使用 |

## 效果展示

<img src="https://creeper5820.com/creeper-qt/blue-style-widgets.png" title="" alt="buttons" data-align="center">

<img src="https://creeper5820.com/creeper-qt/example-login.png" title="" alt="example-login" data-align="center">

<img src="https://creeper5820.com/creeper-qt/MainWindow-Screenshot-2025-09-29_21-15-30.png" title="" alt="example-widgets" data-align="center">

<img src="https://creeper5820.com/creeper-qt/MainWindow-Screenshot-2025-09-29_21-15-38.png" title="" alt="example-widgets" data-align="center">

<div align=center>
    <img src="https://creeper5820.com/creeper-qt/switch-working.gif" width=50%>
</div>

<div align=center>
    <img src="https://creeper5820.com/creeper-qt/filled-text-field.gif" width=50%>
</div>

## 调用示例

安装指南：[`usage.md`](./doc/usage.md)

使用 Cmake 导入：

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

add_executable(${PROJECT_NAME}
    main.cc
)
target_link_libraries(${PROJECT_NAME}
    creeper-qt::creeper-qt
    Qt6::Widgets
)
```

然后在项目中使用：

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

## 待做事项

- [ ] 等接口稳定后 release 一个版本然后开始迭代版本号
  - 接口设计目前基本稳定，多数修改不会影响下游
- [ ] 全组件表格展示
- [ ] 增加更多的组件
  - 至 `2025.09.18`，组件数量还是不太够，需要继续添加
- [ ] 按钮的禁止效果
- [ ] 增加视图容器，原生的不可用
  - 目前已实现无动画的 Flow 布局
- [ ] 提供一个日历模组组件
- [ ] 提供一个设置中心例子

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=creeper5820/creeper-qt&type=Date)](https://www.star-history.com/#creeper5820/creeper-qt&Date)
