#pragma once
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/buttons/filled-button.hh>
#include <creeper-qt/widget/buttons/filled-tonal-button.hh>
#include <creeper-qt/widget/buttons/outlined-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/text-fields.hh>
#include <creeper-qt/widget/text.hh>
#include <creeper-qt/widget/widget.hh>

#include <qdesktopservices.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkproxy.h>
#include <qnetworkreply.h>
#include <qstandardpaths.h>

struct NetworkContext final {
    QNetworkAccessManager accessor;
    std::atomic<bool> downloading = false;

    explicit NetworkContext() noexcept {
        // Qt5 Not Accept std::chrono literals for this function
        accessor.setTransferTimeout(5'000);
    }

    auto set_proxy(std::string const& host = "127.0.0.1", uint16_t port = 7890,
        QNetworkProxy::ProxyType type = QNetworkProxy::HttpProxy) noexcept {

        auto proxy = QNetworkProxy {};
        proxy.setType(type);
        proxy.setHostName(QString::fromStdString(host));
        proxy.setPort(port);
        accessor.setProxy(proxy);

        qDebug() << "[Network] Proxy set to:" << host.data() << ":" << port;
    }
    auto download_resource(std::string const& url, std::string const& location) noexcept {
        if (downloading.load(std::memory_order::relaxed) == true) {
            qDebug() << "[Network]] Download already in progress, returning.";
            return;
        }

        downloading.store(true, std::memory_order::relaxed);
        qDebug() << "[Network]] Url:" << url.data();

        const auto req = QNetworkRequest { { url.data() } };
        const auto res = accessor.get(req);
        QObject::connect(res, &QNetworkReply::finished, [=, this] {
            if (res->error() != QNetworkReply::NoError) {
                qDebug() << "[Network]] Download failed:" << res->error();
                downloading.store(false, std::memory_order::relaxed);
                return;
            }
            auto target = QFile { location.data() };
            if (target.open(QIODevice::WriteOnly)) {
                target.write(res->readAll());
                target.close();
                qDebug() << "[Network]] Save file to:" << QString::fromStdString(location);
            }
            res->deleteLater();
            downloading.store(false, std::memory_order::relaxed);
        });
        QObject::connect(res, &QNetworkReply::downloadProgress, [](qint64 received, qint64 total) {
            if (total > 0) {
                qDebug() << "[Network]] Download:" << (received * 100 / total) << "%"
                         << "(" << received << "/" << total << "bytes)";
            }
        });
        qDebug() << "[Network]] Download request sent, waiting for downloading";
    }
};

struct AssetCenter : creeper::Widget {
    creeper::ThemeManager& manager;
    NetworkContext network;

    // MaterialIcons Regular
    static constexpr auto font_url {
        "https://github.com/google/material-design-icons/raw/master/font/MaterialIconsRound-Regular.otf",
    };

    std::string download_location = "";
    auto download_font() noexcept {
        if (network.downloading.load(std::memory_order::relaxed)) {
            qDebug() << "[AssetCenter] Download already in progress, returning.";
            return;
        }

        const auto filename = QUrl { font_url }.fileName();
        const auto download = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        const auto default_ = download + "/" + filename;

        const auto result = QFileDialog::getSaveFileName(
            nullptr, "Save font file", default_, "TrueType Font (*.ttf)");

        if (!result.isEmpty()) {
            qDebug() << "[AssetCenter] Prepared to download:" << result;
            network.download_resource(font_url, result.toStdString());
            download_location = result.toStdString();
        }
    }
    auto open_download_location() noexcept -> void {
        if (download_location.empty()) {
            qDebug() << "[AssetCenter] Download location is empty, cannot open.";
            return;
        }

        const auto file_path = QString::fromStdString(download_location);
        const auto file_info = QFileInfo { file_path };

        const auto directory_path = file_info.absolutePath();

        if (directory_path.isEmpty()) {
            qDebug() << "[AssetCenter] Failed to get directory path from:" << file_path;
            return;
        }

        const auto directory_url = QUrl::fromLocalFile(directory_path);

        const auto success = QDesktopServices::openUrl(directory_url);

        if (success) {
            qDebug() << "[AssetCenter] Opening download directory:" << directory_path;
        } else {
            qDebug() << "[AssetCenter] Failed to open download directory:" << directory_path;
        }
    }

    std::string proxy_string = "";
    auto apply_proxy_configuration() noexcept {
        if (proxy_string.empty()) {
            qDebug() << "[AssetCenter] Proxy string is empty, skipping proxy configuration";
            return;
        }

        const auto proxy_qstr = QString::fromStdString(proxy_string);
        const auto parts      = proxy_qstr.split(':');
        if (parts.size() != 2) {
            qDebug() << "[AssetCenter] Error: Invalid proxy format, expected 'host:port'";
            return;
        }

        const auto host_qstr = parts[0].trimmed();
        const auto port_qstr = parts[1].trimmed();

        bool ok   = false;
        auto port = port_qstr.toInt(&ok);
        if (!ok || port < 1 || port > 65535) {
            qDebug() << "[AssetCenter] Error: Invalid port number:" << parts[1];
            return;
        }

        network.set_proxy(host_qstr.toStdString(), port);
        qDebug() << "[AssetCenter] Proxy configured:" << host_qstr << ":" << port;
    }

    auto component() noexcept {
        using namespace creeper;
        return new Row {
            row::pro::Margin { 0 },
            row::pro::Item<OutlinedCard> {
                card::pro::ThemeManager { manager },
                card::pro::LevelLowest,
                card::pro::MinimumHeight { 200 },
                card::pro::Layout<Row> {
                    row::pro::Item<Group<Col, OutlinedButton>> {
                        col::pro::Margin { 15 },
                        col::pro::Spacing { 10 },
                        col::pro::Alignment { Qt::AlignTop },
                        group::pro::Compose {
                            std::vector<std::tuple<std::string, std::function<void()>>> { {
                                { "下载字体", [this] { download_font(); } },
                                { "应用代理", [this] { apply_proxy_configuration(); } },
                                { "打开文件", [this] { open_download_location(); } },
                            } },
                            [this](const std::string& text, const auto& function) {
                                return new OutlinedButton {
                                    outlined_button::pro::ThemeManager { manager },
                                    outlined_button::pro::FixedSize { 70, 40 },
                                    outlined_button::pro::Text { text },
                                    outlined_button::pro::Clickable { function },
                                };
                            },
                        },
                    },
                    row::pro::Item<Col> {
                        col::pro::Margin { 20 },
                        col::pro::Spacing { 10 },
                        col::pro::Item<Text> {
                            text::pro::ThemeManager { manager },
                            text::pro::Text {
                                QString {
                                    "示例中的所有 ICON 均为 Google Material Icons 字体提供，"
                                    "需要下载并安装相应的字体文件才能正常显示\n"
                                    "这里贴心做了下载按钮，点击即送，如果无法下载，"
                                    "可以设置代理进行下载，字体链接：%1\n",
                                }
                                    .arg(font_url),
                            },
                            text::pro::WordWrap { true },
                            text::pro::TextInteractionFlags {
                                Qt::TextInteractionFlag::TextSelectableByMouse,
                            },
                        },
                        col::pro::Item<FilledTextField> {
                            text_field::pro::ThemeManager { manager },
                            text_field::pro::LeadingIcon {
                                "public",
                                material::round::font,
                            },
                            text_field::pro::LabelText {
                                "设置代理，像这样：localhost:5820",
                            },
                            text_field::pro::OnChanged {
                                [this](const auto& s) { proxy_string = s.toStdString(); },
                            },
                        },
                    },
                },
            },
        };
    }

    explicit AssetCenter(creeper::ThemeManager& manager) noexcept
        : manager { manager } {
        creeper::Widget::setLayout(component());
    }
};
