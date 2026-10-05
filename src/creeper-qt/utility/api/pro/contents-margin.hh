#pragma once

#include <qlayout.h>

namespace creeper::api::pro {

/**
 * @brief 设置布局内容边距
 *
 * 调用 QLayout::setContentsMargins。
 */
struct ContentsMargin : public QMargins {
    using QMargins::QMargins;
    ContentsMargin(const QMargins& margins)
        : QMargins(margins) { }

    friend auto dsl_invoke(auto& layout, const ContentsMargin& prop) -> void {
        layout.setContentsMargins(static_cast<const QMargins&>(prop));
    }
};

}
