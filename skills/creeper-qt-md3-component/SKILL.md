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

- 规格基线是 **Jetpack Compose Material3**（`androidx.compose.material3`）；不要默认抄 material-web / Flutter 的实现。
- 数值 token 查 material-web `tokens/versions/*/_md-comp-*.scss`；动画/状态查 Compose 源（如 `Checkbox.kt`）。
- 用 `webfetch` 逐条核对源码，不凭记忆。
- 先判定组件是**外部受控**（`value` + 回调，父层更新）还是自驱动；本标准组件一律受控。

## 1. 骨架

- 继承最贴近语义的 Qt 基类（选择→`QAbstractButton`，展示→`QWidget`，输入→`QLineEdit`）+ `DSL`。
- `CREEPER_PIMPL_DEFINITION`；声明式构造 `explicit Xxx(auto&&... props) : Xxx{} { construct_with(...); }`。
- `pro` 命名空间是完整入口（本组件 prop + `api::scope::*`）。
- `Impl` 定义放在 `.cc`（不放 `.impl.hh`）。

## 2. 常数与度量

- 常数收敛到 `Defaults`：尺寸用 `int`，时长用 `std::chrono::milliseconds`，easing 用 `CubicBezierSolution`；同一份 token 只留一处（如共享的 `focus_ring`）。
- `Measurements` 字段默认值引用 `Defaults::k*`。
- 尺寸落地：override `sizeHint()`；`set_measurements()` 里同步 `setMinimumSize` + `updateGeometry`；构造里 `setSizePolicy`。

## 3. 颜色

- `Colors` 按状态分组：外层状态（`enabled`/`disabled`/`error`）× 内层态（`checked`/`unchecked`/`indeterminate`）× token（`checkmark`/`box`/`border`/`state_layer`）；跨状态相同的 token 提到顶层。
- `Defaults::mapColors(const ColorScheme&, Colors&)` 填满；加 `friend dsl_invoke` 使 `Colors` 成为合法 prop。
- 绘制只从 `Colors` 取（`tokens()` / `group()`），不在绘制期读 `ColorScheme`、不做动态分发。

## 4. 状态与动画

- 受控：状态由外部 setter（`setChecked`/`setCheckState` 等）驱动，组件不自切换；`previous_state` = 上一个外部值，用于选择退出标记。
- `Animatable` + `TransitionValue<TweenState<T>>`（或 PID/Spring）；**单一进度**驱动绘制派生。
- 交互覆盖层各自独立：`enter/leave → hover`，`focusIn/Out → focus`，`press/release → reaction`。
- **每个事件只驱动自己那一条**，不要打包成一个 `update_overlay` 到处转发。
- 只在状态变化处安排「重放」（如先 `snap_to(0)` 再进入）；不要放进会被 hover/focus 反复触发的共享更新里。

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
