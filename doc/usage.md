# 使用指南

## 项目依赖

- `C++23` 及以上，对应 `GCC14` 及以上
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

## 方式零 安装预构建安装包

前往 [发布界面](https://github.com/creeper5820/creeper-qt/releases/tag/nightly-library) 下载对应的安装包，进行下载，默认提供 Qt6 的版本，如果需要 Qt5 的版本，可以修改 CMakeLists 中的 `QT_VERSION`，然后手动编译安装：

```sh
# For apt
sudo apt install creeper-qt-*.deb

# For pacman
sudo pacman -U creeper-qt-*.pkg.tar.zst

# For windows, todo (
```

## 方式一 手动静态链接

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

## 方式二 手动编译安装

### Linux 平台编译安装

```bash
# 下载这个项目
git clone https://github.com/creeper5820/creeper-qt --depth=1

# 进入项目根目录
cd creeper-qt

# build
cmake -B build -DBUILD_EXAMPLE=ON
cmake --build build -j

# 启动例程
./build/widgets

# 下载到全局环境中，理论上是 /usr/local 里面
sudo cmake --build build -j --target install
```

### Windows 平台编译安装

我推荐使用 MSYS2 环境使用这个库: [MYSY2-INSTALLATION](https://www.msys2.org/docs/installer/)

<img src="https://creeper5820.com/creeper-qt/windows-neofetch.png" title="" alt="win" data-align="center">

看呐, 我没有使用 Linux (

不得不说, 在Windows使用`zsh`和`pacman`包管理是一件令人惬意的事情

切入正题, 进入 MSYS2 终端

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

到这里就可以编译这个库了, 如果还是会有一些依赖问题, 可以 Google 一下如何在 MSYS2 中安装 Qt6

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
