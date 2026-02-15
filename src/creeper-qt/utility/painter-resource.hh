#pragma once
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qpixmap.h>

namespace creeper {

namespace painter_resource {
    template <typename T>
    concept finished_callback_c = std::invocable<T> || std::invocable<T, QPixmap&>;
}

struct PainterResource : public QPixmap {

    constexpr explicit PainterResource(std::string_view url) noexcept
        : QPixmap {} {
        const auto qurl = QUrl(QString::fromUtf8(url.data(), static_cast<int>(url.size())));
        if (is_filesystem_url(url) || is_qt_resource_url(url)) {
            QPixmap::load(qurl.path());
        } else if (is_network_url(url)) {
            download_resource_from_network(qurl, [](auto&) { });
        } else {
            qWarning() << "[PainterResource] Failed to recognize the type of url";
        }
    }
    constexpr explicit PainterResource(std::string_view url, auto&& f) noexcept
        requires painter_resource::finished_callback_c<decltype(f)>
    {
        const auto qurl = QUrl(QString::fromUtf8(url.data(), static_cast<int>(url.size())));
        if (is_network_url(url)) {
            download_resource_from_network(qurl, f);
        } else {
            qWarning() << "[PainterResource] Only network url can be used with callback";
        }
    }

    ~PainterResource() noexcept { *resource_exiting = false; }

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

    auto is_loading() const noexcept -> bool { return is_loading_; }

    auto is_error() const noexcept -> bool { return is_error_; }

    auto add_finished_callback(std::invocable<PainterResource&> auto&& f) {
        finished_callback_ = std::forward<decltype(f)>(f);
    }

private:
    std::optional<std::function<void(PainterResource&)>> finished_callback_;

    bool is_loading_ = false;
    bool is_error_   = false;

    std::shared_ptr<bool> resource_exiting = std::make_shared<bool>(true);

    auto download_resource_from_network(const QUrl& url, auto&& f) noexcept -> void
        requires painter_resource::finished_callback_c<decltype(f)>
    {
        is_loading_ = true;

        auto manager = new QNetworkAccessManager;
        auto replay  = manager->get(QNetworkRequest { url });

        auto resource_exiting = this->resource_exiting;
        QObject::connect(replay, &QNetworkReply::finished, [=, this] {
            if (!*resource_exiting) {
                qWarning() << "[PainterResource] Async task aborted: "
                              "Resource instance has been destroyed.";
                return;
            }

            const auto error = replay->error();
            const auto data  = replay->readAll();
            if (error != QNetworkReply::NoError) {
                is_error_ = true;
                qWarning() << "[PainterResource] Network error:" << replay->errorString();
            } else if (data.isNull()) {
                is_error_ = true;
            } else {
                is_error_ = false;
                loadFromData(data);
            }
            is_loading_ = false;
            manager->deleteLater();

            using F = decltype(f);
            if constexpr (std::invocable<F, PainterResource&>) std::invoke(f, *this);
            if constexpr (std::invocable<F>) std::invoke(f);

            if (finished_callback_) std::invoke(*finished_callback_, *this);
        });
    }

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
