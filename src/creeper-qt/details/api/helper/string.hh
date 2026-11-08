#pragma once

#include <qstring.h>

#include <concepts>
#include <string>
#include <type_traits>
#include <utility>

namespace creeper::api::helper {

/// @brief
/// 通用文本属性（可定制 setter）
/// 默认调用 widget.setText()
template <auto setter = [](auto& self, const auto& text) { self.setText(text); }>
struct String : public QString {
    using QString::QString;

    explicit String(const QString& text) noexcept
        : QString { text } { }

    explicit String(const std::string& text) noexcept
        : QString { QString::fromStdString(text) } { }

    auto operator=(const QString& text) noexcept -> String& {
        QString::operator=(text);
        return *this;
    }

    auto operator=(QString&& text) noexcept -> String& {
        QString::operator=(std::move(text));
        return *this;
    }

    friend auto dsl_invoke(auto& widget, const String& prop) -> void
        requires requires { setter(widget, static_cast<const QString&>(prop)); }
    {
        setter(widget, static_cast<const QString&>(prop));
    }
};

}
