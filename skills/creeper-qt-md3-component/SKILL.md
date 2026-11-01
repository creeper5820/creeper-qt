---
name: creeper-qt-md3-component
description: 在 creeper-qt 中按 Material Design 3 规格实现或审查组件（Qt Widgets 之上的声明式封装）。新增/修改组件、对齐 MD3 规格、编写主题/动画/绘制时使用。
---

# creeper-qt 标准（MD3）组件实现 SOP

先读基础规范再动手：

| 规范 | 何时读 |
|------|--------|
| [creeper-qt-usage](../creeper-qt-usage/SKILL.md) | 使用组件、声明式构造、prop 命名空间、生命周期 |
| [creeper-qt-development](../creeper-qt-development/SKILL.md) | 组件结构、绘制约定、诊断、性能、API |

例子: `creeper-qt/widget/checkbox.hh`

## 0. 基线与查证

- 规格唯一来源是 `tool/md3-token/config.yaml` 锁定 commit 的 **Jetpack Compose Material3** 源码（`androidx.compose.material3` 及其依赖的 ripple / animation / ui-text）；不要抄 material-web、Flutter 或 m3.material.io 的数值。
- 源码在 `build/md3-token/androidx`（`tool/md3-token/generator.py fetch` 拉取），逐条核对，不凭记忆。
- 颜色字段和取值以组件的 `XxxDefaults` / `XxxColors` 为准（如 `CheckboxDefaults.colors()`、`SnackbarDefaults`），不要从 tokens 文件里挑。
- 先判定组件是**外部受控**（`value` + 回调，父层更新）还是自驱动；本标准组件一律受控。

## 1. 骨架

- 继承最贴近语义的 Qt 基类（选择→`QAbstractButton`，展示→`QWidget`，输入→`QLineEdit`）+ `DSL`。
- `CREEPER_PIMPL_DEFINITION`；声明式构造 `explicit Xxx(auto&&... props) : Xxx{} { construct_with(...); }`。
- `pro` 命名空间是完整入口（本组件 prop + `api::scope::*`）。
- `Impl` 定义放在 `.cc`（不放 `.impl.hh`）。

## 2. 常数与度量

- 常数、`Measurements`、`Colors`、`mapColors` 全部由 `src/creeper-qt/details/token/<component>.yaml` 生成，组件不手写数值（见第 9 节）。
- 组件只引入生成的类型：
  ```cpp
  using Measurements = token::<component>::Measurements;
  using Colors       = token::<component>::Colors;
  using Defaults     = token::<component>::Defaults;
  ```
  生成结构自带泛型 `dsl_invoke`（要求组件有 `setMeasurements` / `setColors`），组件不再写。
- 尺寸是 `creeper::Dp`：使用处 `.px()` 取整像素、`.value` 取 double、`Dp / Dp` 得比例；时长是 `std::chrono::milliseconds`，easing 是 `CubicBezierSolution`。
- 纯几何比例和换算公式（如 `state_layer_size / 2`、弹簧阻尼比换算）不算 token，可以写在组件里。
- 尺寸落地：override `sizeHint()`；`set_measurements()` 里同步 `setMinimumSize` + `updateGeometry`；构造里 `setSizePolicy`。

## 3. 颜色

- `Colors` 是扁平结构，字段名是 Compose `XxxColors` 属性名的 snake_case（如 `checked_box_color`）；Compose 没有、组件需要的颜色（如 error、焦点环、状态层）追加在后面，取值仍来自 Compose 源码。
- `Colors` 字段类型是 `creeper::Color`（继承 `QColor`，支持 `{ QColor, opacity }` 构造，opacity 直接作为 alpha）。
- 状态 → 颜色的选择规则照搬 Compose（如 `CheckboxColors.boxColor(enabled, state)`），写成组件内的小函数；绘制只从 `colors` 取，不在绘制期读 `ColorScheme`。
- `Defaults::mapColors` 由生成器生成，`loadColorScheme` 里调用即可。

## 4. 状态与动画

- 受控：状态由外部 setter（`setChecked`/`setCheckState` 等）驱动，组件不自切换；`previous_state` = 上一个外部值，用于选择退出标记。
- `Animatable` + `TransitionValue<TweenState<T>>`（或 PID/Spring）；**单一进度**驱动绘制派生。
- 交互覆盖层各自独立：`enter/leave → hover`，`focusIn/Out → focus`，`press/release → reaction`。
- **每个事件只驱动自己那一条**，不要打包成一个 `update_overlay` 到处转发。
- 只在状态变化处安排「重放」（如先 `snap_to(0)` 再进入）；不要放进会被 hover/focus 反复触发的共享更新里。
- 动画参数照搬 Compose：`MotionSchemeKeyTokens.*` 是弹簧（`SpringState`，`k = stiffness`，`d = 2 * dampingRatio * sqrt(stiffness)`）；ripple 和状态层的时长、透明度来自 `material-ripple` 的 `RippleAnimation.kt` / `Ripple.kt` 和 `StateTokens`。进入和离开时长不同时，在各自事件里 `transition_to` 前设置 config。

## 5. 绘制

- `Graphics` 结构收拢绘制节点与 `static make_*` 辅助；用声明式 `Paint::Box` / `Paint::RoundedRectangle` + `Size`/`Fill`/`Outline`/`Radiuses`，先算值再进树。
- 自定义节点需满足 `drawable_trait`（`origin`/`size`/`operator()(QPainter&)`）；路径用相对坐标，位置交给 `Paint::Box`。
- 转发到 `Impl` 的实现不含逻辑，参数原样转发。
- focus ring 只在键盘焦点（`Tab`/`BacktabFocusReason`）显示（focus-visible）。

## 6. 常见坑

- `QColor::fromRgbF` 收到负 alpha 会返回无效色，Qt 会画成黑色；插值前 `std::clamp(t, 0, 1)`。
- `const auto paint = Paint::Box{...}` 调不动（`Container::operator()` 非 const）——用非 const 或 lambda。
- `Paint::Box` 按节点的 `size` 居中；自定义节点漏设 `size` 会被摆到盒外。
- `INDETERMINATE` 这类「snap + 重放」不要放进共享更新（会被 hover/focus 反复触发）。
- Qt 焦点：点击 NoFocus 的空白处不会自动 `focusOut`；环的去留靠 focus-visible 判定，而不是指望失焦。

## 7. 无头验证

- `QT_QPA_PLATFORM=offscreen`；用 `QImage` + `widget.render(&painter, {}, {}, QWidget::RenderFlags{})` 逐帧抓图（`RenderFlags{}` 保留背景色）。
- 对照规格动画帧（`ffmpeg` / `magick` 抽帧）**逐帧**比对，不要只看关键帧；状态矩阵做明/暗两套。
- 全量 `cmake --build build` 通过；`test/props/<name>.cc` 语法通过。

## 8. 交付

- `doc/widgets.md` 补组件章节（命名空间、属性表、示例）。
- 按主题拆 wip 提交，不混入无关改动；PR 描述最终状态。

## 9. Token 生成

工具在 `tool/md3-token/`，用法和 yaml 字段见其 `README.md`。

1. `python3 tool/md3-token/generator.py fetch`：按锁定 commit 拉取 Compose 源码到 `build/md3-token/androidx`。
2. 写 `src/creeper-qt/details/token/<component>.yaml`：只写数值在 Compose 中的位置（`tokens` / `symbols` / `extract`），不写数值。
3. `python3 tool/md3-token/generator.py generate`：生成 `details/token/<component>/{values,measurements,colors,defaults}.hh`，生成文件入库、不手改。
4. `python3 tool/md3-token/generator.py check`：生成结果与仓库一致、include 依赖合规；提交前必须通过。

约束：

- `details/` 不依赖正式组件：不 include `widget/` / `layout/`，不出现组件类名；`check` 会拦截。
- 生成代码不写与名字重复的注释（如 `ShapeKeyTokens.X` 对 `sys::shape::kX`）。
- 名字遮蔽：`IconButton::Color` 等作用域内的 `Color` 指本地类型，要用 token 颜色类型时写 `creeper::Color`。
