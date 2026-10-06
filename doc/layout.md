# CREEPER-QT 布局系统文档

[返回主页](../README.md) | [使用指南](./usage.md) | [组件文档](./widgets.md)

---

## 通用布局属性

命名空间：由各布局组件的 `pro` 导出（如 `creeper::linear::pro`、`creeper::flow::pro`）

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `ContentsMargin` | `QMargins` | 设置布局内容边距 |
| `Spacing` | `int` | 设置子项之间的间距 |
| `Margin` | `int` | 设置统一的边距（四个方向相同） |
| `Alignment` | `Qt::Alignment` | 设置布局对齐方式 |

```cpp
namespace lnpro = creeper::linear::pro;

auto layout = new Row {
    lnpro::Spacing { 10 },
    lnpro::ContentsMargin { { 15, 15, 15, 15 } },
    lnpro::Alignment { Qt::AlignCenter },
    // ... 子项
};
```

---

## 线性布局

线性布局是最常用的布局方式，支持水平和垂直两种方向。

### Row（水平布局）

命名空间：`creeper::row::pro` 或 `creeper::linear::pro`

类型别名：`creeper::Row` 或 `creeper::HBoxLayout`

### Col（垂直布局）

命名空间：`creeper::col::pro` 或 `creeper::linear::pro`

类型别名：`creeper::Col` 或 `creeper::VBoxLayout`

### 专有属性

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `LinearItem<T>` | `T*` 或构造参数 | 添加子项（组件或布局），可由 `指针 + Placement` 得到 |
| `SpacingItem` | `int` | 添加固定大小的间距 |
| `Stretch` | `int` | 添加弹性空间（拉伸因子） |
| `SpacerItem` | `QSpacerItem*` | 添加自定义间距项 |

```cpp
using namespace creeper;
namespace lnpro = linear::pro;

// 水平布局示例：指针与 Row::Placement 相加，指定拉伸因子与对齐方式
auto row = new Row {
    lnpro::Spacing { 10 },
    new FilledButton {
      filled_button::pro::Text { "按钮1" },
    } + Row::Placement { 1, Qt::AlignLeft },
    lnpro::Stretch { 1 }, // 弹性空间
    new FilledButton {
      filled_button::pro::Text { "按钮2" },
    } + Row::Placement { 0 },
    lnpro::SpacingItem { 20 }, // 固定间距
    new FilledButton {
      filled_button::pro::Text { "按钮3" },
    } + Row::Placement { 0 },
};

// 垂直布局示例：在 Col 中使用 Col::Placement
auto col = new Col {
    lnpro::ContentsMargin { { 20, 20, 20, 20 } },
    new FilledTextField {
      manager,
      text_field::pro::LabelText { "用户名" },
    } + Col::Placement { 0 },
    new FilledTextField {
      manager,
      text_field::pro::LabelText { "密码" },
    } + Col::Placement { 0 },
    lnpro::Stretch { 1 },
    new FilledButton {
      filled_button::pro::Text { "提交" },
    } + Col::Placement { 0 },
};

// 已有指针时直接与 Placement 相加
auto createButton = [](const QString& text) {
    return new FilledButton {
        filled_button::pro::Text { text },
    };
};

auto row2 = new Row {
    createButton("按钮1") + Row::Placement { 0 },
    createButton("按钮2") + Row::Placement { 0 },
};
```

---

## 网格布局

网格布局用于创建规则的网格状布局，支持跨行跨列。

### Grid

命名空间：`creeper::grid::pro`

类型：`creeper::Grid`

### 专有属性

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `GridItem<T>` | `Grid::Placement, T*` 或构造参数 | 添加子项，需要指定行列位置 |
| `RowSpacing` | `int` | 设置行间距 |
| `ColSpacing` | `int` | 设置列间距 |

```cpp
using namespace creeper;
namespace gpro = grid::pro;

auto grid = new Grid {
    gpro::RowSpacing { 10 },
    gpro::ColSpacing { 10 },
    gpro::GridItem<FilledButton> {
      Grid::Placement { 0, 0, Qt::AlignCenter }, // row=0, col=0
      filled_button::pro::Text { "左上" },
    },
    gpro::GridItem<FilledButton> {
      Grid::Placement { 0, 1 }, // row=0, col=1
      filled_button::pro::Text { "右上" },
    },
    gpro::GridItem<FilledButton> {
      Grid::Placement { 1, 1, 0, 2, Qt::AlignCenter }, // row=1, row_span=1, col=0, col_span=2
      filled_button::pro::Text { "跨列按钮" },
    },
};
```

---

## 堆叠布局

堆叠布局用于在同一位置显示多个组件，通过索引切换显示。

### Stacked（NavHost）

命名空间：`creeper::stacked::pro`

类型：`creeper::Stacked` 或 `creeper::NavHost`

### 专有属性

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `Widget*` | `Widget*` | 直接添加子 Widget |
| `CurrentIndex` | `int` | 设置当前显示的页面索引 |
| `IndexChanged` | `[](int index){}` | 索引改变时的回调函数（连接 `currentChanged` 信号） |

```cpp
using namespace creeper;
namespace stpro = stacked::pro;

auto stacked = new Stacked {
    stpro::CurrentIndex { 0 },
    stpro::IndexChanged { [](int index) { qDebug() << "当前页面索引:" << index; } },
    new Widget {
      new Col {
        // 第一页内容
      },
    },
    new Widget {
      new Col {
        // 第二页内容
      },
    },
};

// 切换页面
stacked->setCurrentIndex(1);
```

---

## 流式布局

流式布局用于自动换行的布局，类似于 CSS 的 flex-wrap。

### Flow

命名空间：`creeper::flow::pro`

类型：`creeper::Flow`

### 专有属性

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `RowSpacing` | `int` | 设置行间距（MainAxisSpacing） |
| `ColSpacing` | `int` | 设置列间距（CrossAxisSpacing） |
| `RowLimit` | `int` | 设置每行最大项数（MaxItemsInEachRow） |

```cpp
using namespace creeper;
namespace fpro = flow::pro;

auto flow = new Flow {
    fpro::RowSpacing { 10 },
    fpro::ColSpacing { 10 },
    fpro::RowLimit { 3 }, // 每行最多 3 个
    fpro::AddWidget<FilledButton> {
      filled_button::pro::Text { "按钮1" },
    },
    fpro::AddWidget<FilledButton> {
      filled_button::pro::Text { "按钮2" },
    },
};

// 或者使用 With 属性批量添加
auto flow2 = new Flow {
    fpro::RowSpacing { 10 },
    fpro::ColSpacing { 10 },
    fpro::RowLimit { 3 },
    flow::pro::With { [](Flow& self) {
        for (int i = 0; i < 10; ++i) {
            self.addWidget(new FilledButton {
              filled_button::pro::Text { QString("按钮%1").arg(i) },
            });
        }
        ,
    } },
};
```

---

## 滚动区域

### ScrollArea

命名空间：`creeper::scroll::pro`

类型：`creeper::ScrollArea`

### 专有属性

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `ScrollItem` | `Widget*` 或 `Layout*` | 设置滚动区域的内容 |
| `VerticalScrollBarPolicy` | `Qt::ScrollBarPolicy` | 设置垂直滚动条策略 |
| `HorizontalScrollBarPolicy` | `Qt::ScrollBarPolicy` | 设置水平滚动条策略 |
| `ScrollBarPolicy` | `Qt::ScrollBarPolicy, Qt::ScrollBarPolicy` | 同时设置水平和垂直滚动条策略 |

```cpp
using namespace creeper;

auto scroll_area = new ScrollArea {
    manager,
    scroll::pro::HorizontalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
    scroll::pro::VerticalScrollBarPolicy { Qt::ScrollBarAsNeeded },
    scroll::pro::ScrollItem {
      SomeContentWidget, // 传入已有的 widget 指针,
    },
};

// 或者直接构造内容组件
auto scroll_area2 = new ScrollArea {
    manager,
    scroll::pro::ScrollBarPolicy {
      Qt::ScrollBarAlwaysOff,
      Qt::ScrollBarAlwaysOff,
    },
    scroll::pro::ScrollItem { ButtonGroup }, // ButtonGroup 是已构造好的 widget*,
};
```

---

## 相关文档

- [使用指南](./usage.md)
- [组件文档](./widgets.md)
- [项目主页](../README.md)
- [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)
- [问题反馈](https://github.com/creeper5820/creeper-qt/issues)
