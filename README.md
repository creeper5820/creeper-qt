# CREEPER-QT<br>轻量的QT-UI封装库
## 效果展示
全组件展示

![](https://raw.githubusercontent.com/creeper5820/creeper-qt/refs/heads/main/doc/example-widgets.png)

Google小时钟，是我做这个主题最原初的动力，一开始只是想着这钟真漂亮，Qt能不能搞一个出来，于是纯代码自绘了波浪，为此引入了`Eigen`，顺带做了一个UI库，可谓是为了一瓶醋包了这顿饺子

![](https://raw.githubusercontent.com/creeper5820/creeper-qt/refs/heads/main/doc/example-clock.png)

## 安装指南
### 项目依赖
- `gcc-13` 及以上，支持完整 range 等特性
- `cmake`
- `yaml-cpp`
- `qt-5`

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
    ${这个库所有的源文件}
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

然后你可以在cmake中导入这个库，并引入头文件使用它了
```cmake
cmake_minimum_required(VERSION 3.22)

project(hello-world)

# yaml-cpp 和 Qt5 是项目依赖的库，记得导入
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)
find_package(creeper-qt REQUIRED)

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
```cpp
#include <creeper-qt/widget/line-edit.hh>
```
在使用前记得设置主题：
```cpp
#include <creeper-qt/setting/theme.hh>

auto main(int argc, char* argv[]) -> int {
    using namespace creeper;
    // 直接输入字符串就可以索引到主题
    Theme::setTheme("common-green");
    // 或者用一些我提前定义好的主题常量
    Theme::setTheme(Theme::common::green);
}
```
## TODO
- [ ] 增加更多的组件
- [ ] 适配windows平台
- [ ] 增加视图容器，原生的不可用