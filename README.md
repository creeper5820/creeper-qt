<div align=center>

<div align=center><img src="https://creeper5820.com/creeper-qt/creeper-qt.jpg" width=400></div>

<h1>CREEPER-QT</h1>

[组件文档](./doc/widgets.md) | [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)

![GitHub last commit](https://img.shields.io/github/last-commit/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=9ccbfb) ![GitHub Repo stars](https://img.shields.io/github/stars/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=b9c8da) ![GitHub repo size](https://img.shields.io/github/repo-size/creeper5820/creeper-qt?style=for-the-badge&labelColor=101418&color=d3bfe6)

`creeper-qt` 是基于 `Qt` 开发的 UI 集成库，轻量简洁，组件外观大体遵循 `Google Material Design3`，同时改造了老式的命令式调用，使组件构造和配置可以在同一时间完成，添加了主题管理，支持切换明亮和黑暗主题和各种配色，动效部分基于 PID 控制器和弹簧模型等迭代算法，带来更流畅的动画体验和打断效果

欢迎 PR 和 ISSUE！

</div>

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


## 📦 示例程序（example）使用说明

本仓库自动构建并发布了跨平台的 **示例程序**，方便快速体验项目功能。

🔗 最新版本下载地址：  
👉 [GitHub Releases 页面](https://github.com/creeper5820/creeper-qt/releases)

| 操作系统 | 安装与运行方式 |
|----------|----------------|
| **Linux** | - 下载 `AppImage` 后赋予执行权限并运行<br>- 或解压 `.tar.gz` 后执行其中的 `AppRun` 文件 |
| **Windows** | - 解压 `zip` 后进入文件夹，双击运行 `widget.exe` 即可使用 |

## 安装指南

### 项目依赖

- `C++23` 及以上
- `cmake`
- `eigen` (库实现依赖，二次开发不依赖)
- `qt-5 / qt-6`

```zsh
# For Qt6
# on arch linux
sudo pacman -S eigen qt6-base

# on ubuntu
# ubuntu 默认 gcc 版本比较低，建议使用 ppa 下载较新的版本
# 或者直接下载二进制文件放进环境中
sudo apt install libeigen3-dev qt6-base-dev

# For Qt5
# on arch linux
sudo pacman -S eigen qt5-base

# on ubuntu
sudo apt install libeigen3-dev qtbase5-dev
```
### 方式零 安装预构建安装包

前往 [发布界面](https://github.com/creeper5820/creeper-qt/releases) 下载对应的安装包，进行下载：

```sh
# For apt
sudo apt install creeper-qt-*.deb
# For pacman
sudo pacman -U creeper-qt-*.pkg.tar.zst
```

### 方式一 直接使用源文件

把项目拉下来吧

```bash
cd path/to/your/project/lib/dir/
git clone https://github.com/creeper5820/creeper-qt --depth=1
```

Edit your `CMakeLists.txt`:

```cmake
set(CMAKE_AUTOMOC ON)

include_directories(
    ${库的根路径}
)
add_executable(
    ${EXAMPLE_NAME}
    ${这个库所有的 .cc 文件}
    ${这个库所有的 .hh 文件}
)
target_link_libraries(
    ${EXAMPLE_NAME}
    Qt6::Widgets
)
```

### 方式二 Linux 平台编译安装

```bash
# 下载这个项目
git clone https://github.com/creeper5820/creeper-qt --depth=1

# 进入项目根目录
cd creeper-qt

# build
cmake -B build -DBUILD_EXAMPLE=ON
cmake --build build

# 启动例程
./build/widgets

# 下载到全局环境中，理论上是 /usr/local 里面
sudo cmake --build build -j --target install
```

### 方式三 Windows 平台编译安装

我推荐使用MSYS2环境使用这个库: [MYSY2-INSTALLATION](https://www.msys2.org/docs/installer/)

<img src="https://creeper5820.com/creeper-qt/windows-neofetch.png" title="" alt="win" data-align="center">

看呐, 我没有使用Linux (

不得不说, 在Windows使用`zsh`和`pacman`包管理是一件令人惬意的事情

切入正题, 进入MSYS2终端

```sh
## 先刷新一下软件包数据
pacman -Sy

## 安装编译使用的工具链
pacman -S mingw-w64-x86_64-toolchain

## 安装 Qt6
pacman -S mingw64/mingw-w64-x86_64-qt6

## 安装依赖
pacman -S mingw-w64-x86_64-eigen3

## 如果依赖找不到可以搜索一下对应版本的包, 找到 mingw 的版本就行
pacman -Ss eigen3
```

到这里就可以编译这个库了, 如果还是会有一些依赖问题, 可以Google一下如何在MSYS2中安装Qt6

```sh
git clone https://github.com/creeper5820/creeper-qt --depth=1

## 依然是在MSYS2环境中
## 进入项目根目录
mkdir build

## 在根目录进行项目配置
## 使用"MinGW Makefiles"或者“Ninja”
## CMAKE_INSTALL_PREFIX 参数指定了安装目录, 
## 默认的下载目录一般会是 C:/Program Files (x86)/
## 会提示没有权限
cmake -G "MinGW Makefiles" -B build -DBUILD_EXAMPLE=ON -DCMAKE_INSTALL_PREFIX="C:/xxx/xxx/"

## 编译之
## 或者在build目录下使用 mingw32-make -j
cmake --build build -j

## 安装库, 注意调用的是mingw的make
## 直接使用make可能会出现错误
cd build && mingw32-make install

## 可以查看所有文件的安装位置
cat install_manifest.txt

## 启动实例程序
./widgets.exe
```

需要注意的是, 如果在本机而不是 MSYS2 中打开编译好的可执行文件, 会报找不到 Qt 的 dll, 因为在 MSYS2 下载的 Qt 没有暴露在 Windows 环境中

## 调用示例

使用 Cmake 导入

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
find_package(Eigen3 REQUIRED)

# 在 Windows 下, 安装目录如果没有暴露在环境变量, 
# 需要手动指定一下, 项目才能找到头文件
# dll 文件在我这能找到, 没有在其他电脑上测过, 可能需要注意一下
# include_directories(D:/Software/msys2/usr/include/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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

int main(int argc, char* argv[]) {
    using namespace creeper;

    // Qt 运行时初始化
    auto application = new QApplication { argc, argv };

    // 创建主题管理器，可以传入主题包
    auto theme_manager = ThemeManager { kBlueMikuThemePack };

    namespace pro = filled_button::pro;
    auto button   = FilledButton {
        pro::ThemeManager { theme_manager },    // 与主题管理器绑定
        pro::FixedSize { 100, 50 },             // 设置固定大小
        pro::Text { "你好世界" },               // 设置文字
        pro::Clickable { [] { qDebug() << "Hello World"; } },
    };
    button.show();

    // 将主题应用到注册过的组件中
    theme_manager.apply_theme();

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
