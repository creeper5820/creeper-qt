# CREEPER-QT<br>轻量的QT-UI封装库

## 项目架构
- **widget**：组件是对QWidget的一层谨慎的封装
- **module**：模块通常由若干Widget组成
- **setting**：设置包含了一个全局的主题管理器
- **utility**：工具类设计一些动画计算用到的函数或者语言设计层面的类

## 效果展示
![](https://raw.githubusercontent.com/creeper5820/creeper-qt/refs/heads/main/doc/example-widgets.png)

## INSTALLATION
### For header-only
Clone this repository first:
```bash
cd path/to/your/project/lib/dir/
git clone https://github.com/creeper5820/creeper-qt
```
Edit your `CMakeLists.txt`:
```cmake
include_directories(path/to/creeper-qt/..)

```



### For Linux
```bash
# clone this repository
git clone https://github.com/creeper5820/creeper-qt
# entry the directory
cd creeper-qt
# build
mkdir build && cd build
cmake .. && make -j
# install for global env
sudo make install
```

Then you can import this library for your project:
```cpp
#include "creeper-qt/widget/line-edit.hh"
```