#pragma once

#include "creeper-qt/utility/api/helper/string.hh"

namespace creeper::api::pro {

/// 标准 Text 属性
/// 要求组件实现：void setText(const QString&)
using Text = helper::String<>;

}
