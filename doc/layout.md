# CREEPER-QT 布局系统文档

[返回主页](../README.md) | [使用指南](./usage.md) | [组件文档](./widgets.md)

---

## 通用布局属性

命名空间：`creeper::layout::pro`

| 属性名 | 类型 | 说明 |
| --- | --- | --- |
| `ContentsMargin` | `QMargins` | 设置布局内容边距 |
| `Spacing` | `int` | 设置子项之间的间距 |
| `Margin` | `int` | 设置统一的边距（四个方向相同） |
| `Alignment` | `Qt::Alignment` | 设置布局对齐方式 |

```cpp
namespace lnpro = creeper::layout::pro;

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
| `Item` | `Widget*` 或 `Layout*` | 添加子项（组件或布局） |
| `SpacingItem` | `int` | 添加固定大小的间距 |
| `Stretch` | `int` | 添加弹性空间（拉伸因子） |
| `SpacerItem` | `QSpacerItem*` | 添加自定义间距项 |

```cpp
using namespace creeper;
namespace lnpro = linear::pro;

// 水平布局示例
auto row = new Row {
    lnpro::Spacing { 10 },
    lnpro::Item<FilledButton> {
        { 1, Qt::AlignLeft },  // stretch=1, alignment=Left
        button::pro::Text { "按钮1" }
    },
    lnpro::Stretch { 1 },  // 弹性空间
    lnpro::Item<FilledButton> {
        button::pro::Text { "按钮2" }
    },
    lnpro::SpacingItem { 20 },  // 固定间距
    lnpro::Item<FilledButton> {
        button::pro::Text { "按钮3" }
    }
};

// 垂直布局示例
auto col = new Col {
    lnpro::ContentsMargin { { 20, 20, 20, 20 } },
    lnpro::Item<FilledTextField> {
        text_field::pro::ThemeManager { manager },
        text_field::pro::LabelText { "用户名" }
    },
    lnpro::Item<FilledTextField> {
        text_field::pro::ThemeManager { manager },
        text_field::pro::LabelText { "密码" }
    },
    lnpro::Stretch { 1 },
    lnpro::Item<FilledButton> {
        button::pro::Text { "提交" }
    }
};

// 如果组件是通过函数返回的，直接传入函数调用结果
auto createButton = [](const QString& text) {
    return new FilledButton {
        button::pro::Text { text }
    };
};

auto row2 = new Row {
    lnpro::Item { createButton("按钮1") },
    lnpro::Item { createButton("按钮2") }
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
| `Item` | `Widget*` 或 `Layout*` | 添加子项，需要指定行列位置 |
| `RowSpacing` | `int` | 设置行间距 |
| `ColSpacing` | `int` | 设置列间距 |

```cpp
using namespace creeper;
namespace gpro = grid::pro;

auto grid = new Grid {
    gpro::RowSpacing { 10 },
    gpro::ColSpacing { 10 },
    gpro::Item<FilledButton> {
        { 0, 0, Qt::AlignCenter },  // row=0, col=0
        button::pro::Text { "左上" }
    },
    gpro::Item<FilledButton> {
        { 0, 1 },  // row=0, col=1
        button::pro::Text { "右上" }
    },
    gpro::Item<FilledButton> {
        { 1, 0, 1, 2, Qt::AlignCenter },  // row=1, row_span=1, col=0, col_span=2
        button::pro::Text { "跨列按钮" }
    }
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
| `Item` | `Widget*` | 添加子项（仅支持 Widget） |
| `CurrentIndex` | `int` | 设置当前显示的页面索引 |
| `IndexChanged` | `[](int index){}` | 索引改变时的回调函数（连接 `currentChanged` 信号） |

```cpp
using namespace creeper;
namespace stpro = stacked::pro;

auto stacked = new Stacked {
    stpro::CurrentIndex { 0 },
    stpro::IndexChanged { [](int index) {
        qDebug() << "当前页面索引:" << index;
    }},
    stpro::Item<Widget> {
        widget::pro::Layout<Col> {
            // 第一页内容
        }
    },
    stpro::Item<Widget> {
        widget::pro::Layout<Col> {
            // 第二页内容
        }
    }
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
    fpro::RowLimit { 3 },  // 每行最多 3 个
    layout::pro::Widget<FilledButton> {
        button::pro::Text { "按钮1" }
    },
    layout::pro::Widget<FilledButton> {
        button::pro::Text { "按钮2" }
    },
};

// 或者使用 Apply 属性批量添加
auto flow2 = new Flow {
    fpro::RowSpacing { 10 },
    fpro::ColSpacing { 10 },
    fpro::RowLimit { 3 },
    flow::pro::Apply { [](Flow& self) {
        for (int i = 0; i < 10; ++i) {
            self.addWidget(new FilledButton {
                button::pro::Text { QString("按钮%1").arg(i) }
            });
        }
    }}
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
| `Item` | `Widget*` 或 `Layout*` | 设置滚动区域的内容 |
| `VerticalScrollBarPolicy` | `Qt::ScrollBarPolicy` | 设置垂直滚动条策略 |
| `HorizontalScrollBarPolicy` | `Qt::ScrollBarPolicy` | 设置水平滚动条策略 |
| `ScrollBarPolicy` | `Qt::ScrollBarPolicy, Qt::ScrollBarPolicy` | 同时设置水平和垂直滚动条策略 |

```cpp
using namespace creeper;

auto scroll_area = new ScrollArea {
    scroll::pro::ThemeManager { manager },
    scroll::pro::HorizontalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
    scroll::pro::VerticalScrollBarPolicy { Qt::ScrollBarAsNeeded },
    scroll::pro::Item {
        SomeContentWidget,  // 传入已有的 widget 指针
    }
};

// 或者直接构造内容组件
auto scroll_area2 = new ScrollArea {
    scroll::pro::ThemeManager { manager },
    scroll::pro::ScrollBarPolicy {
        Qt::ScrollBarAlwaysOff,
        Qt::ScrollBarAlwaysOff,
    },
    scroll::pro::Item { ButtonGroup },  // ButtonGroup 是已构造好的 widget*
};
```

---

## 相关文档

- [使用指南](./usage.md)
- [组件文档](./widgets.md)
- [项目主页](../README.md)
- [视频演示](https://www.bilibili.com/video/BV1JbxjzZEJ5)
- [问题反馈](https://github.com/creeper5820/creeper-qt/issues)
