#include "painter-resource.hh"

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qstring.h>
#include <qurl.h>

namespace creeper {

PainterResource::PainterResource(std::string_view url) noexcept
    : QPixmap { } {
    const auto qurl = QUrl(QString::fromUtf8(url.data(), static_cast<int>(url.size())));
    if (is_filesystem_url(url) || is_qt_resource_url(url)) {
        QPixmap::load(qurl.path());
    } else if (is_network_url(url)) {
        download_resource_from_network(qurl, [](auto&) { });
    } else {
        qWarning() << "[PainterResource] Failed to recognize the type of url";
    }
}

PainterResource::PainterResource(
    std::string_view url, std::function<void(PainterResource&)>&& callback) noexcept {
    const auto qurl = QUrl(QString::fromUtf8(url.data(), static_cast<int>(url.size())));
    if (is_network_url(url)) {
        download_resource_from_network(qurl, std::move(callback));
    } else {
        qWarning() << "[PainterResource] Only network url can be used with callback";
    }
}

PainterResource::~PainterResource() noexcept { *resource_exiting = false; }

auto PainterResource::is_loading() const noexcept -> bool { return is_loading_; }
auto PainterResource::is_error() const noexcept -> bool { return is_error_; }

auto PainterResource::download_resource_from_network(
    const QUrl& url, std::function<void(PainterResource&)> callback) noexcept -> void {
    is_loading_ = true;

    auto manager = new QNetworkAccessManager;
    auto reply   = manager->get(QNetworkRequest { url });

    auto resource_exiting = this->resource_exiting;
    QObject::connect(reply, &QNetworkReply::finished, [=, this] {
        if (!*resource_exiting) {
            qWarning() << "[PainterResource] Async task aborted: "
                          "Resource instance has been destroyed.";
            return;
        }

        const auto error = reply->error();
        const auto data  = reply->readAll();
        if (error != QNetworkReply::NoError) {
            is_error_ = true;
            qWarning() << "[PainterResource] Network error:" << reply->errorString();
        } else if (data.isNull()) {
            is_error_ = true;
        } else {
            is_error_ = false;
            loadFromData(data);
        }
        is_loading_ = false;
        manager->deleteLater();

        if (callback) std::invoke(callback, *this);
        if (finished_callback_) std::invoke(*finished_callback_, *this);
    });
}

}
