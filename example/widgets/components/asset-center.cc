#include "asset-center.hh"
#include "example/widgets/util/download.hh"
#include "example/widgets/util/executor.hh"
#include "example/widgets/util/string.hh"

#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/outlined-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/text.hh>

#include <print>

#include <exec/static_thread_pool.hpp>
#include <qdesktopservices.h>
#include <qfiledialog.h>
#include <stdexec/execution.hpp>

using namespace creeper;

struct AssetCenter::Impl {
    ThemeManager& manager;
    exec::static_thread_pool context { 1 };

    std::atomic<bool> downloading = false;
    std::string save_location;

    MutableQString status {};

    auto update_ui(double ratio, std::string _status) {
        QMetaObject::invokeMethod(qApp, [this, ratio, _status = std::move(_status)] {
            status = QString::fromStdString(
                std::format("<code>[Status] {:3.2f}% ({})</code>", ratio, _status));
        });
    }

    auto select_location() {
        auto caption = "选择保存位置", filter = "otf Files (*.otf)";
        save_location = QFileDialog::getSaveFileName(nullptr, caption, str::kFontFileName, filter)
                            .toStdString();
        return save_location.empty() ? std::optional<std::string> { std::nullopt }
                                     : std::optional { save_location };
    }
    auto open_location() const noexcept {
        if (!save_location.empty()) {
            auto info = QFileInfo { QString::fromStdString(save_location) };
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absolutePath()));
        }
    }

    auto download_asset(std::optional<std::string> where) {
        auto task = ::util::GetUrlTask {
            .base_url  = str::kGithubBaseUrl,
            .path      = str::kFontUrlPath,
            .save_path = where,
            .progress_callback =
                [this](std::size_t now, std::size_t total) {
                    update_ui(now * 100. / total, "下载中");
                },
        };
        task.execute();
    }

    auto start_downloading() noexcept {
        if (downloading.load(std::memory_order::relaxed)) {
            std::println("一个下载任务正在进行");
            return;
        }
        auto gui_scheduler = QtScheduler { qApp };
        auto run_scheduler = context.get_scheduler();

        auto task = stdexec::schedule(gui_scheduler)
            | stdexec::then([this] { update_ui(0.0, "下载开始"); })
            | stdexec::then([this] { downloading.store(true, std::memory_order::relaxed); })
            | stdexec::then([this] { return select_location(); })
            | stdexec::continues_on(run_scheduler) //
            | stdexec::then([this](auto where) { download_asset(where); })
            | stdexec::then([this] { update_ui(100.0, "字体下载完成"); })
            | stdexec::continues_on(gui_scheduler) //
            | stdexec::upon_error([this](std::exception_ptr e) {
                  try {
                      std::rethrow_exception(e);
                  } catch (const std::runtime_error& e) {
                      update_ui(0., e.what());
                      std::println("Unexpected: {}", e.what());
                  }
              })
            | stdexec::then([this] { downloading.store(false, std::memory_order::relaxed); });
        stdexec::start_detached(task);
    }

    auto Component() noexcept -> QLayout* {
        using namespace creeper;
        update_ui(0., "下载准备就绪");

        return new Row {
            row::pro::Margin { 0 },
            row::pro::Item<OutlinedCard> {
                card::pro::ThemeManager { manager },
                card::pro::LevelLow,
                card::pro::MinimumHeight { 200 },
                card::pro::Layout<Row> {
                    row::pro::Item<Group<Col, OutlinedButton>> {
                        col::pro::Margin { 15 },
                        col::pro::Spacing { 10 },
                        col::pro::Alignment { Qt::AlignTop },
                        group::pro::Compose {
                            std::vector<std::tuple<std::string, std::function<void()>>> { {
                                { "下载字体", [this] { start_downloading(); } },
                                { "打开文件", [this] { open_location(); } },
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
                    row::pro::Item<FilledCard> {
                        card::pro::ThemeManager { manager },
                        card::pro::LevelLowest,
                        card::pro::Layout<Col> {
                            col::pro::Margin { 20 },
                            col::pro::Spacing { 10 },
                            col::pro::Item<Text> {
                                text::pro::ThemeManager { manager },
                                text::pro::TextFormat { Qt::RichText },
                                text::pro::TextInteractionFlags { Qt::TextBrowserInteraction },
                                text::pro::OpenExternalLinks { true },
                                text::pro::Text { str::kFontDescription },
                                text::pro::WordWrap { true },
                            },
                            col::pro::Item<Text> {
                                text::pro::ThemeManager { manager },
                                MutableForward { text::pro::Text {}, status },
                            },
                        },
                    },
                },
            },
        };
    }
};

AssetCenter::AssetCenter(ThemeManager& manager) noexcept
    : pimpl { std::make_unique<Impl>(manager) } {
    Widget::setLayout(pimpl->Component());
}

AssetCenter::~AssetCenter() = default;
