# CREEPER-QT（未发布）<br>轻量的QT-UI封装库

## 效果展示

全组件展示

![example-widgets](doc/image/example-widgets.png)

![buttons](doc/image/buttons.png)

Google小时钟，是我做这个主题最原初的动力，一开始只是想着这钟真漂亮，Qt能不能搞一个出来，于是纯代码自绘了波浪，为此引入了`Eigen`，顺带做了一个UI库，可谓是为了一瓶醋包了这顿饺子

![example-clock](doc/image/example-clock.png)

数据结构大作业登陆界面，既然写了库就要好好用起来：

![example-login](doc/image/example-login.png)

## 使用文档

- [`Widgets 文档`](doc/widgets.md)：基本组件的接口罗列和使用方法
- [`Utilty 文档`](doc/utilty.md)：主题系统的使用及封装思路

## 调用示例

使用 Cmake 导入

```cmake
cmake_minimum_required(VERSION 3.22)

project(hello-world)

# yaml-cpp 和 Qt5 是项目依赖的库，记得导入
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)
find_package(creeper-qt REQUIRED)

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
    Qt5::Widgets yaml-cpp
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
        pro::ThemeManager { theme_manager }, // 与主题管理器绑定
        pro::FixedSize { 100, 50 },          // 设置固定大小
        pro::Text { "你好世界" },            // 设置文字
        pro::Clickable { [] { qDebug() << "Hello World"; } },
    };
    button.show();

    // 将主题应用到注册过的组件中
    theme_manager.apply_theme();

    return application->exec();
}
```

## 安装指南

### 项目依赖

- `gcc-13` 及以上，支持完整 range 等特性
- `cmake`
- `yaml-cpp`
- `eigen`
- `qt-5`

```zsh
# on arch linux
sudo pacman -S yaml-cpp eigen qt5-base

# on ubuntu
# ubuntu 默认 gcc 版本比较低，建议使用 ppa 下载较新的版本
# 或者直接下载二进制文件放进环境中
sudo apt install libyaml-cpp-dev libeigen3-dev qtbase5-dev
```

### 直接使用源文件

把项目拉下来吧

```bash
cd path/to/your/project/lib/dir/
git clone https://github.com/creeper5820/creeper-qt
```

Edit your `CMakeLists.txt`:

```cmake
include_directories(${库的根路径/include})
add_executable(${EXAMPLE_NAME}
    ${这个库所有的源文件(包括头文件)}
    ${这个库所有的QRC文件}
)
```

### Linux平台

```bash
# 下载这个项目
git clone https://github.com/creeper5820/creeper-qt
# 进入项目根目录
cd creeper-qt
# build
mkdir build && cd build
cmake .. && make -j
# 下载到全局环境中，理论上是/usr/local里面
sudo make install
```

### Windows平台

我推荐使用MSYS2环境使用这个库: [MYSY2-INSTALLATION](https://www.msys2.org/docs/installer/)

![win](doc/image/windows-neofetch.png)

看呐, 我没有使用Linux (

不得不说, 在Windows使用`zsh`和`pacman`包管理是一件令人惬意的事情

切入正题, 进入MSYS2终端

```bash
## 先刷新一下软件包数据
pacman -Sy

## 安装编译使用的工具链
pacman -S  mingw-w64-x86_64-toolchain

## 安装 Qt5
pacman -S mingw64/mingw-w64-x86_64-qt5

## 安装依赖
pacman -S mingw-w64-x86_64-eigen3 mingw-w64-x86_64-yaml-cpp

## 如果依赖找不到可以搜索一下对应版本的包, 找到 mingw 的版本就行
pacman -Ss eigen3
```

到这里就可以编译这个库了, 如果还是会有一些依赖问题, 可以Google一下如何在MSYS2中安装QT5

```bash
## 依然是在MSYS2环境中
## 进入项目根目录
mkdir build

## 在根目录进行项目配置
## 请务必使用"MinGW Makefiles"
## CMAKE_INSTALL_PREFIX 参数指定了安装目录, 
## 默认的下载目录一般会是 C:/Program Files (x86)/
## 会提示没有权限
cmake -G "MinGW Makefiles" -B build -DCMAKE_INSTALL_PREFIX="D:/Software/msys2/usr/"

## 编译之
## 或者在build目录下使用 mingw32-make -j
cmake --build build

## 安装库, 注意调用的是mingw的make
## 直接使用make可能会出现错误
cd build && mingw32-make install

## 可以查看所有文件的安装位置
cat install_manifest.txt

## 启动实例程序
./widgets.exe
```

需要注意的是, 如果在本机而不是MSYS2中打开编译好的可执行文件, 会报找不到Qt的dll, 因为在MSYS2下载的Qt没有暴露在Windows环境中

## 风格约定

### 绘图函数调用的参数遵从下面的顺序

1. 内容项，类似于 Text
2. 颜色配置项
3. 形状配置项

## 待做事项

- [ ] 增加更多的组件
- [ ] 按钮的禁止效果
- [ ] 增加视图容器，原生的不可用
- [ ] 给自己做一个设置中心吧
- [ ] 做一个日历模组
