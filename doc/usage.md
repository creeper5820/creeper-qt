# CREEPER-QT 使用指南

[返回主页](../README.md)

---

## 项目依赖

| 依赖项 | 版本要求 | 说明 |
| --- | --- | --- |
| C++ 标准 | C++23 及以上 | 需要 GCC 14+ 或同等编译器 |
| CMake | 最新稳定版 | 构建系统 |
| Eigen | 3.x | 库实现依赖（二次开发不依赖） |
| Qt6 | | |

### 依赖安装

<details>
<summary>Arch Linux</summary>

```bash
sudo pacman -S eigen qt6-base
```

</details>

<details>
<summary>Ubuntu / Debian</summary>

```bash
# 注意：Ubuntu 默认 GCC 版本较低
# 建议使用 PPA 下载较新版本，或直接下载二进制文件

sudo apt install libeigen3-dev qt6-base-dev
```

如果系统 GCC 版本过低，可以通过以下方式安装新版本：

```bash
# ubuntu24 及以上不需要手动添加源
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-14 g++-14
```

</details>

<details>
<summary>Windows (MSYS2)</summary>

```bash
# 更新软件包数据库
pacman -Sy

# 安装工具链
pacman -S mingw-w64-x86_64-toolchain

# 安装 Qt6
pacman -S mingw64/mingw-w64-x86_64-qt6

# 安装 Eigen
pacman -S mingw-w64-x86_64-eigen3

# 如果找不到依赖，可以搜索对应包
pacman -Ss eigen3
```

</details>

---

## 使用方式

### 方式一：CMake FetchContent（推荐）

无需手动克隆或安装，CMake 自动拉取源码并编译。

```cmake
include(FetchContent)
FetchContent_Declare(
    creeper-qt
    GIT_REPOSITORY https://github.com/creeper5820/creeper-qt.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(creeper-qt)

# 链接单个模块
target_link_libraries(your_target PRIVATE creeper-qt::widgets)

# 或链接全部模块
target_link_libraries(your_target PRIVATE creeper-qt)
```

---

### 方式二：安装预构建安装包

[前往下载页面](https://github.com/creeper5820/creeper-qt/releases/tag/nightly-library)

| 包管理器 | 安装命令 |
| --- | --- |
| APT (Debian/Ubuntu) | `sudo apt install ./creeper-qt-*.deb` |
| Pacman (Arch Linux) | `sudo pacman -U creeper-qt-*.pkg.tar.zst` |
| Windows | 开发中... |

```cmake
find_package(creeper-qt REQUIRED)

target_link_libraries(your_target PRIVATE creeper-qt::widgets)
```

---

### 方式三：手动编译安装

<details>
<summary>Linux</summary>

```bash
git clone https://github.com/creeper5820/creeper-qt --depth=1
cd creeper-qt

cmake -B build -DBUILD_EXAMPLE=ON
cmake --build build -j$(nproc)

./build/widgets

# 安装到系统（可选，默认安装到 /usr/local）
sudo cmake --build build --target install
```

自定义安装路径：

```bash
cmake -B build \
  -DBUILD_EXAMPLE=ON \
  -DCMAKE_INSTALL_PREFIX=/your/custom/path

cmake --build build -j$(nproc)
cmake --build build --target install
```

</details>

<details>
<summary>Windows (MSYS2)</summary>

参考[依赖安装](#依赖安装)中的 Windows (MSYS2) 部分安装依赖后：

```bash
git clone https://github.com/creeper5820/creeper-qt --depth=1
cd creeper-qt

cmake -G "MinGW Makefiles" \
  -B build \
  -DBUILD_EXAMPLE=ON \
  -DCMAKE_INSTALL_PREFIX="C:/your/install/path"

cmake --build build -j
cmake --build build --target install

./build/widgets.exe
```

如果在 Windows 资源管理器中直接运行可执行文件，可能会提示找不到 Qt 的 DLL 文件。这是因为通过 MSYS2 安装的 Qt 库没有暴露到 Windows 系统环境中。建议在 MSYS2 终端中运行，或将 MSYS2 的 `lib` 和 `bin` 添加到系统 PATH。

</details>

```cmake
list(APPEND CMAKE_PREFIX_PATH "/your/custom/path")

find_package(creeper-qt REQUIRED)
target_link_libraries(your_target PRIVATE creeper-qt::widgets)
```

#### 独立构建示例程序

```bash
cd example/widgets
cmake -B build
cmake --build build -j$(nproc)
./build/widgets
```

---

## 相关文档

- [组件文档](widgets.md)
- [布局文档](layout.md)
- [主题文档](theme.md)
- [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)
- [问题反馈](https://github.com/creeper5820/creeper-qt/issues)

---

## 常见问题

**Q: 为什么需要 C++23 和 GCC 14？**

CREEPER-QT 使用了 C++23 的一些新特性以提供更好的 API 设计和性能，使用了大量模板元和语法糖来增加抽象层级，加快开发速度，减少重复性代码。如果您的系统编译器版本较低，请参考上文中的编译器升级指南。对于 Windows 平台，十分建议使用 MSYS2 环境搭配包管理软件 `pacman` 来减轻依赖配置的痛苦，同时该环境默认编译器版本较高，不需要额外配置。

**Q: 可以使用 Clang 编译吗？**

可以，但需要确保 Clang 版本支持 C++23 标准（Clang 16+）。
