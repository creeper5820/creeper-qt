#pragma once
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qpixmap.h>

namespace creeper {

template <typename T>
concept finished_callback_c = std::invocable<T> || std::invocable<T, QPixmap&>;

struct PainterResource : public QPixmap {

    constexpr explicit PainterResource(std::string_view url) { }

    explicit PainterResource(const PainterResource& other) noexcept
        : QPixmap(other) {
        default_constructor();
    }
    explicit PainterResource(PainterResource&& other) noexcept
        : QPixmap(std::move(other)) {
        default_constructor();
    }

private:
    auto download_resource(const QUrl& url, finished_callback_c auto&& f) noexcept -> void {
        auto manager = new QNetworkAccessManager;
        auto replay  = manager->get(QNetworkRequest { url });
        QObject::connect(replay, &QNetworkReply::finished, [=, this] {
            loadFromData(replay->readAll());
            manager->deleteLater();

            if constexpr (std::invocable<decltype(f), PainterResource&>) std::invoke(f, *this);
            if constexpr (std::invocable<decltype(f)>) std::invoke(f);
        });
    }
    auto default_constructor() noexcept -> void { }

    static auto is_network_url(const QUrl& url) noexcept -> bool {
        const auto scheme = url.scheme().toLower();
        return scheme == "http" || scheme == "https" || scheme == "ftp" //
            || scheme == "ws" || scheme == "wss";
    }
    static auto is_filesystem_url(const QUrl& url) noexcept -> bool {
        return url.isLocalFile(); //
    }
    static auto is_qt_resource_url(const QUrl& url) noexcept -> bool {
        const auto scheme = url.scheme().toLower();
        return scheme == "qrc" || (scheme.isEmpty() && url.path().startsWith("/"));
    }
};

inline auto foo() { auto s = PainterResource { "Hello World" }; }

}
