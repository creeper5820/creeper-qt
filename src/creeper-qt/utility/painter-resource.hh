#pragma once
#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <utility>

#include <qpixmap.h>
#include <qurl.h>

namespace creeper {

namespace painter_resource {
    template <typename T>
    concept finished_callback_c = std::invocable<T> || std::invocable<T, QPixmap&>;
}

struct PainterResource : public QPixmap {

    explicit PainterResource(std::string_view url) noexcept;

    template <typename F>
    explicit PainterResource(std::string_view url, F&& f) noexcept
        requires painter_resource::finished_callback_c<F>
        : PainterResource { url, normalize_callback(std::forward<F>(f)) } { }

    ~PainterResource() noexcept;

    template <typename T>
    explicit PainterResource(T&& other) noexcept
        requires std::convertible_to<T, QPixmap>
        : QPixmap(std::forward<T>(other)) { }

    template <typename T>
    auto operator=(T&& other) noexcept -> PainterResource&
        requires std::convertible_to<T, QPixmap>
    {
        QPixmap::operator=(std::forward<T>(other));
        return *this;
    }

    auto is_loading() const noexcept -> bool;
    auto is_error() const noexcept -> bool;

    auto add_finished_callback(std::invocable<PainterResource&> auto&& f) {
        finished_callback_ = std::forward<decltype(f)>(f);
    }

private:
    std::optional<std::function<void(PainterResource&)>> finished_callback_;

    bool is_loading_ = false;
    bool is_error_   = false;

    std::shared_ptr<bool> resource_exiting = std::make_shared<bool>(true);

    template <typename F>
    static auto normalize_callback(F&& f) -> std::function<void(PainterResource&)> {
        return [f = std::forward<F>(f)](PainterResource& self) mutable {
            if constexpr (std::invocable<F, PainterResource&>) std::invoke(f, self);
            if constexpr (std::invocable<F>) std::invoke(f);
        };
    }

    explicit PainterResource(
        std::string_view url, std::function<void(PainterResource&)>&& callback) noexcept;

    auto download_resource_from_network(
        const QUrl& url, std::function<void(PainterResource&)>) noexcept -> void;

    static constexpr auto starts_with(std::string_view s, std::string_view prefix) -> bool {
        return s.substr(0, prefix.size()) == prefix;
    }
    static constexpr auto is_filesystem_url(std::string_view url) -> bool {
        return !starts_with(url, "http://") && !starts_with(url, "https://")
            && !starts_with(url, "qrc:/") && !starts_with(url, ":/");
    }
    static constexpr auto is_qt_resource_url(std::string_view url) -> bool {
        return starts_with(url, "qrc:/") || starts_with(url, ":/");
    }
    static constexpr auto is_network_url(std::string_view url) -> bool {
        return starts_with(url, "http://") || starts_with(url, "https://");
    }
};

}
