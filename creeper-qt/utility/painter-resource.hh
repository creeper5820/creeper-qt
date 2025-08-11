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
            qDebug() << "[PainterResource] Failed to recognize the type of url";
        }
    }

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

private:
    auto download_resource_from_network(
        const QUrl& url, painter_resource::finished_callback_c auto&& f) noexcept -> void {
        auto manager = new QNetworkAccessManager;
        auto replay  = manager->get(QNetworkRequest { url });
        QObject::connect(replay, &QNetworkReply::finished, [=, this] {
            loadFromData(replay->readAll());
            manager->deleteLater();

            if constexpr (std::invocable<decltype(f), PainterResource&>) std::invoke(f, *this);
            if constexpr (std::invocable<decltype(f)>) std::invoke(f);
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
