<div align="center">

# 📖 CREEPER-QT 使用指南

### 完整的安装、配置与开发文档

[← 返回主页](./README.md)

</div>

---

## 📋 目录

- [📖 CREEPER-QT 使用指南](#-creeper-qt-使用指南)
    - [完整的安装、配置与开发文档](#完整的安装配置与开发文档)
  - [📋 目录](#-目录)
  - [🔧 项目依赖](#-项目依赖)
    - [依赖安装](#依赖安装)
  - [📦 安装方式](#-安装方式)
    - [方式零: 安装预构建安装包 (推荐)](#方式零-安装预构建安装包-推荐)
      - [平台安装命令](#平台安装命令)
    - [方式一: 手动静态链接](#方式一-手动静态链接)
      - [1. 克隆仓库到项目目录](#1-克隆仓库到项目目录)
      - [2. 配置 CMakeLists.txt](#2-配置-cmakeliststxt)
    - [方式二: 手动编译安装](#方式二-手动编译安装)
  - [🖥️ 平台特定说明](#️-平台特定说明)
    - [Linux 平台](#linux-平台)
      - [编译步骤](#编译步骤)
      - [自定义安装路径](#自定义安装路径)
    - [Windows 平台](#windows-平台)
      - [安装 MSYS2 依赖](#安装-msys2-依赖)
      - [编译步骤](#编译步骤-1)
      - [⚠️ 重要提示](#️-重要提示)
  - [📚 相关文档](#-相关文档)
  - [❓ 常见问题](#-常见问题)

---

## 🔧 项目依赖

<table>
  <tr>
    <th width="200">依赖项</th>
    <th>版本要求</th>
    <th>说明</th>
  </tr>
  <tr>
    <td><strong>C++ 标准</strong></td>
    <td><code>C++23</code> 及以上</td>
    <td>需要 <code>GCC 14+</code> 或同等编译器</td>
  </tr>
  <tr>
    <td><strong>CMake</strong></td>
    <td>最新稳定版</td>
    <td>构建系统</td>
  </tr>
  <tr>
    <td><strong>Eigen</strong></td>
    <td>3.x</td>
    <td>库实现依赖（二次开发不依赖）</td>
  </tr>
  <tr>
    <td><strong>Qt</strong></td>
    <td>Qt5 或 Qt6</td>
    <td>推荐使用 Qt6</td>
  </tr>
</table>

### 依赖安装

<details>
<summary><b>🐧 Arch Linux</b></summary>

```bash
# For Qt6 (推荐)
sudo pacman -S eigen qt6-base

# For Qt5
sudo pacman -S eigen qt5-base
```

</details>

<details>
<summary><b>🐧 Ubuntu / Debian</b></summary>

```bash
# 注意：Ubuntu 默认 GCC 版本较低
# 建议使用 PPA 下载较新版本，或直接下载二进制文件

# For Qt6 (推荐)
sudo apt install libeigen3-dev qt6-base-dev

# For Qt5
sudo apt install libeigen3-dev qtbase5-dev
```

> 💡 **提示**: 如果系统 GCC 版本过低，可以通过以下方式安装新版本：
>
> ```bash
> # ubuntu24 及以上不需要手动添加源
> sudo add-apt-repository ppa:ubuntu-toolchain-r/test 
> sudo apt update
> sudo apt install gcc-14 g++-14
> ```

</details>

<details>
<summary><b>🪟 Windows (MSYS2)</b></summary>

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

## 📦 安装方式

### 方式零: 安装预构建安装包 (推荐)

<div align="center">

**[📥 前往下载页面](https://github.com/creeper5820/creeper-qt/releases/tag/nightly-library)**

默认提供 **Qt6** 版本

</div>

> 💡 **自定义 Qt 版本**: 如需 Qt5 版本，请修改 `CMakeLists.txt` 中的 `QT_VERSION` 参数，然后手动编译安装

#### 平台安装命令

<table>
  <tr>
    <th width="200">包管理器</th>
    <th>安装命令</th>
  </tr>
  <tr>
    <td><strong>APT (Debian/Ubuntu)</strong></td>
    <td><code>sudo apt install ./creeper-qt-*.deb</code></td>
  </tr>
  <tr>
    <td><strong>Pacman (Arch Linux)</strong></td>
    <td><code>sudo pacman -U creeper-qt-*.pkg.tar.zst</code></td>
  </tr>
  <tr>
    <td><strong>Windows</strong></td>
    <td>开发中...</td>
  </tr>
</table>

---

### 方式一: 手动静态链接

适合需要将库集成到项目中，不希望系统全局安装的情况。

#### 1. 克隆仓库到项目目录

```bash
cd path/to/your/project/lib/dir/
git clone https://github.com/creeper5820/creeper-qt --depth=1
```

#### 2. 配置 CMakeLists.txt

```cmake
# 启用 Qt MOC (元对象编译器)
set(CMAKE_AUTOMOC ON)

# 添加库头文件路径
include_directories(
    ${库的根路径}
)

# 添加可执行文件
add_executable(
    ${EXAMPLE_NAME}
    ${这个库所有的 .cc 文件}
    ${这个库所有的 .hh 文件}
)

# 链接 Qt 库
target_link_libraries(
    ${EXAMPLE_NAME}
    Qt6::Widgets  # 或 Qt5::Widgets
)
```

---

### 方式二: 手动编译安装

推荐需要全局使用该库的开发者使用此方式。

---

## 🖥️ 平台特定说明

### Linux 平台

#### 编译步骤

```bash
# 1. 克隆项目
git clone https://github.com/creeper5820/creeper-qt --depth=1

# 2. 进入项目根目录
cd creeper-qt

# 3. 配置构建（启用示例程序）
cmake -B build -DBUILD_EXAMPLE=ON

# 4. 编译项目
cmake --build build -j$(nproc)

# 5. 运行示例程序
./build/widgets

# 6. 安装到系统（可选）
# 默认安装到 /usr/local
sudo cmake --build build --target install
```

#### 自定义安装路径

```bash
cmake -B build \
  -DBUILD_EXAMPLE=ON \
  -DCMAKE_INSTALL_PREFIX=/your/custom/path

cmake --build build -j$(nproc)
cmake --build build --target install
```

---

### Windows 平台

> 💡 **推荐使用 MSYS2 环境** - [MSYS2 安装指南](https://www.msys2.org/docs/installer/)

<div align="center">
  <img src="https://creeper5820.com/creeper-qt/windows-neofetch.png" width="600" alt="Windows MSYS2 环境">
  <p><i>在 Windows 上使用 zsh 和 pacman 是一件令人惬意的事情 😊</i></p>
</div>

#### 安装 MSYS2 依赖

参考 [依赖安装](#依赖安装) 中的 Windows (MSYS2) 部分。

#### 编译步骤

```bash
# 1. 克隆项目
git clone https://github.com/creeper5820/creeper-qt --depth=1

# 2. 进入项目根目录
cd creeper-qt

# 3. 配置构建
# 注意：指定安装路径，避免权限问题
# 默认路径 C:/Program Files (x86)/ 需要管理员权限
cmake -G "MinGW Makefiles" \
  -B build \
  -DBUILD_EXAMPLE=ON \
  -DCMAKE_INSTALL_PREFIX="C:/your/install/path"

# 或使用 Ninja
cmake -G "Ninja" \
  -B build \
  -DBUILD_EXAMPLE=ON \
  -DCMAKE_INSTALL_PREFIX="C:/your/install/path"

# 4. 编译项目
cmake --build build -j

# 或者在 build 目录下直接使用 mingw32-make
cd build && mingw32-make -j

# 5. 安装库
cd build && mingw32-make install

# 6. 查看安装文件列表
cat install_manifest.txt

# 7. 运行示例程序
./widgets.exe
```

#### ⚠️ 重要提示

<table>
  <tr>
    <td width="60">⚠️</td>
    <td>
      <strong>DLL 依赖问题</strong><br>
      如果在 Windows 资源管理器中直接运行可执行文件，可能会提示找不到 Qt 的 DLL 文件。<br>
      这是因为通过 MSYS2 安装的 Qt 库没有暴露到 Windows 系统环境中。
    </td>
  </tr>
</table>

**解决方案:**

1. **在 MSYS2 终端中运行** (推荐)

   ```bash
   ./widgets.exe
   ```

2. **添加 MSYS2 目录到系统 PATH**
   - 将 MSYS2 的 `lib` 和 `bin` 添加到系统环境变量 PATH 中
   - 重启终端或系统后生效

---

## 📚 相关文档

- [📖 使用指南](usage.md) - 本文档
- [🧩 组件文档](widgets.md) - 组件的 API 文档
- [🎬 视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5) - 部分组件展示
- [💬 问题反馈](https://github.com/creeper5820/creeper-qt/issues) - GitHub Issues

---

## ❓ 常见问题

<details>
<summary><b>Q: 为什么需要 C++23 和 GCC 14？</b></summary>

A: CREEPER-QT 使用了 C++23 的一些新特性以提供更好的 API 设计和性能，使用了大量模板元和语法糖来增加抽象层级，加快开发速度，减少重复性代码。如果您的系统编译器版本较低，请参考上文中的编译器升级指南。对于 Windows 平台，十分建议使用 MSYS2 环境搭配包管理软件 `pacman` 来减轻依赖配置的痛苦，同时该环境默认编译器版本较高，不需要额外配置。

</details>

<details>
<summary><b>Q: 可以使用 Clang 编译吗？</b></summary>

A: 可以，但需要确保 Clang 版本支持 C++23 标准（Clang 16+）。

</details>

<details>
<summary><b>Q: Qt5 和 Qt6 有什么区别？</b></summary>

A: CREEPER-QT 同时支持 Qt5 和 Qt6，但推荐使用 Qt6 以获得更好的性能和更多的功能支持。

</details>

<details>
<summary><b>Q: 如何切换 Qt5/Qt6 版本？</b></summary>

A: 修改项目根目录 `CMakeLists.txt` 中的 `QT_VERSION` 变量，然后重新编译即可。

```cmake
set(QT_VERSION Qt6)  # 或改为 Qt5
```

</details>

---

<div align="center">

**遇到问题？** [提交 Issue](https://github.com/creeper5820/creeper-qt/issues) | [查看更多文档](.)

[⬆️ 返回顶部](#-creeper-qt-使用指南)

</div>