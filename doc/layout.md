<div align="center">

# 📐 CREEPER-QT 布局系统文档

### 完整的布局容器 API 参考与使用示例

[← 返回主页](./README.md) | [📖 使用指南](./usage.md) | [🧩 组件文档](./widgets.md)

</div>

---

## 🎨 通用布局属性

<div align="center">

**命名空间**: `creeper::layout::pro`

</div>

### 属性列表

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>ContentsMargin</code></td>
      <td><code>QMargins</code></td>
      <td>设置布局内容边距</td>
    </tr>
    <tr>
      <td><code>Spacing</code></td>
      <td><code>int</code></td>
      <td>设置子项之间的间距</td>
    </tr>
    <tr>
      <td><code>Margin</code></td>
      <td><code>int</code></td>
      <td>设置统一的边距（四个方向相同）</td>
    </tr>
    <tr>
      <td><code>Alignment</code></td>
      <td><code>Qt::Alignment</code></td>
      <td>设置布局对齐方式</td>
    </tr>
  </tbody>
</table>

### 使用示例

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

## 📦 线性布局

线性布局是最常用的布局方式，支持水平和垂直两种方向。

### Row (水平布局)

<div align="center">

**命名空间**: `creeper::row::pro` 或 `creeper::linear::pro`

**类型别名**: `creeper::Row` 或 `creeper::HBoxLayout`

</div>

### Col (垂直布局)

<div align="center">

**命名空间**: `creeper::col::pro` 或 `creeper::linear::pro`

**类型别名**: `creeper::Col` 或 `creeper::VBoxLayout`

</div>

### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Item</code></td>
      <td><code>Widget*</code> 或 <code>Layout*</code></td>
      <td>添加子项（组件或布局）</td>
    </tr>
    <tr>
      <td><code>SpacingItem</code></td>
      <td><code>int</code></td>
      <td>添加固定大小的间距</td>
    </tr>
    <tr>
      <td><code>Stretch</code></td>
      <td><code>int</code></td>
      <td>添加弹性空间（拉伸因子）</td>
    </tr>
    <tr>
      <td><code>SpacerItem</code></td>
      <td><code>QSpacerItem*</code></td>
      <td>添加自定义间距项</td>
    </tr>
  </tbody>
</table>

### 使用示例

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

## 🔲 网格布局

网格布局用于创建规则的网格状布局，支持跨行跨列。

### Grid

<div align="center">

**命名空间**: `creeper::grid::pro`

**类型**: `creeper::Grid`

</div>

### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Item</code></td>
      <td><code>Widget*</code> 或 <code>Layout*</code></td>
      <td>添加子项，需要指定行列位置</td>
    </tr>
    <tr>
      <td><code>RowSpacing</code></td>
      <td><code>int</code></td>
      <td>设置行间距</td>
    </tr>
    <tr>
      <td><code>ColSpacing</code></td>
      <td><code>int</code></td>
      <td>设置列间距</td>
    </tr>
  </tbody>
</table>

### 使用示例

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

## 📚 堆叠布局

堆叠布局用于在同一位置显示多个组件，通过索引切换显示。

### Stacked (NavHost)

<div align="center">

**命名空间**: `creeper::stacked::pro`

**类型**: `creeper::Stacked` 或 `creeper::NavHost`

</div>

### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Item</code></td>
      <td><code>Widget*</code></td>
      <td>添加子项（仅支持 Widget）</td>
    </tr>
    <tr>
      <td><code>Index</code></td>
      <td><code>int</code></td>
      <td>设置当前显示的页面索引</td>
    </tr>
    <tr>
      <td><code>IndexChanged</code></td>
      <td><code>[](auto& self){}</code></td>
      <td>索引改变时的回调函数</td>
    </tr>
  </tbody>
</table>

### 使用示例

```cpp
using namespace creeper;
namespace stpro = stacked::pro;

auto stacked = new Stacked {
    stpro::Index { 0 },
    stpro::IndexChanged { [](auto& self) {
        qDebug() << "当前页面索引:" << self.currentIndex();
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

## 🌊 流式布局

流式布局用于自动换行的布局，类似于 CSS 的 flex-wrap。

### Flow

<div align="center">

**命名空间**: `creeper::flow::pro`

**类型**: `creeper::Flow`

</div>

### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>RowSpacing</code></td>
      <td><code>int</code></td>
      <td>设置行间距（MainAxisSpacing）</td>
    </tr>
    <tr>
      <td><code>ColSpacing</code></td>
      <td><code>int</code></td>
      <td>设置列间距（CrossAxisSpacing）</td>
    </tr>
    <tr>
      <td><code>RowLimit</code></td>
      <td><code>int</code></td>
      <td>设置每行最大项数（MaxItemsInEachRow）</td>
    </tr>
  </tbody>
</table>

### 使用示例

```cpp
using namespace creeper;
namespace fpro = flow::pro;

auto flow = new Flow {
    fpro::RowSpacing { 10 },
    fpro::ColSpacing { 10 },
    fpro::RowLimit { 3 },  // 每行最多 3 个
    // 使用通用 layout::pro::Widget 添加子项
    layout::pro::Widget<FilledButton> {
        button::pro::Text { "按钮1" }
    },
    layout::pro::Widget<FilledButton> {
        button::pro::Text { "按钮2" }
    },
    // ... 更多按钮
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

## 📜 滚动区域

滚动区域用于创建可滚动的容器。

### ScrollArea

<div align="center">

**命名空间**: `creeper::scroll::pro`

**类型**: `creeper::ScrollArea`

</div>

### 专有属性

<table>
  <thead>
    <tr>
      <th width="200">属性名</th>
      <th width="200">类型</th>
      <th>说明</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>Item</code></td>
      <td><code>Widget*</code> 或 <code>Layout*</code></td>
      <td>设置滚动区域的内容</td>
    </tr>
    <tr>
      <td><code>VerticalScrollBarPolicy</code></td>
      <td><code>Qt::ScrollBarPolicy</code></td>
      <td>设置垂直滚动条策略</td>
    </tr>
    <tr>
      <td><code>HorizontalScrollBarPolicy</code></td>
      <td><code>Qt::ScrollBarPolicy</code></td>
      <td>设置水平滚动条策略</td>
    </tr>
    <tr>
      <td><code>ScrollBarPolicy</code></td>
      <td><code>Qt::ScrollBarPolicy, Qt::ScrollBarPolicy</code></td>
      <td>同时设置水平和垂直滚动条策略</td>
    </tr>
  </tbody>
</table>

### 使用示例

```cpp
using namespace creeper;
namespace scroll = scroll::pro;
namespace lnpro = linear::pro;

auto scroll_area = new ScrollArea {
    scroll::ThemeManager { manager },
    scroll::HorizontalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
    scroll::VerticalScrollBarPolicy { Qt::ScrollBarAsNeeded },
    scroll::Item<Col> {
        lnpro::Item<FilledTextField> {
            text_field::pro::ThemeManager { manager },
            text_field::pro::LabelText { "内容" }
        },
        // ... 更多内容
    }
};
```

---

## 📚 相关文档

<div align="center">

| 文档       | 链接                                                              |
| ---------- | ----------------------------------------------------------------- |
| 📖 使用指南 | [usage.md](./usage.md)                                            |
| 🧩 组件文档 | [widgets.md](./widgets.md)                                        |
| 🏠 项目主页 | [README.md](../README.md)                                         |
| 🎬 视频演示 | [Bilibili](https://www.bilibili.com/video/BV1JbxjzZEJ5)           |
| 💬 问题反馈 | [GitHub Issues](https://github.com/creeper5820/creeper-qt/issues) |

</div>

---

<div align="center">

**需要更多帮助？** 

[📖 查看使用指南](./usage.md) | [💬 提交 Issue](https://github.com/creeper5820/creeper-qt/issues) | [🌟 给项目点赞](https://github.com/creeper5820/creeper-qt)

[⬆️ 返回顶部](#-creeper-qt-布局系统文档)

</div>

