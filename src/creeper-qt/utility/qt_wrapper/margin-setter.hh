#pragma once

#include <QtGlobal>

namespace creeper::qt {

inline auto margin_setter = [](auto& self, const auto& margin) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    self.setContentsMargins(margin, margin, margin, margin);
#else
    self.setMargin(margin);
#endif
};

}
