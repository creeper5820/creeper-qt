#include "creeper-qt/setting/theme.hh"

#include <qdebug.h>
#include <qfile.h>
#include <qfont.h>
#include <qstring.h>

#include <atomic>

#include <yaml-cpp/yaml.h>

using namespace creeper;

struct Theme::Impl {
    static inline auto theme_ = QString { common::purple };
    static inline auto occupied_ = std::atomic<bool> { false };
    static inline auto themeConfig_ = std::unique_ptr<YAML::Node>();

    static inline auto widgetReloadThemeHandler_ = std::map<QWidget*, std::function<void(void)>>();
};

bool Theme::occupied() { return pimpl_->occupied_; }

void Theme::setOccupied(bool occupied) { pimpl_->occupied_ = occupied; }

void Theme::setTheme(const QString& name) {
    const auto filePath = ":/theme/" + name + "/theme.yaml";
    if (!QFile::exists(filePath)) return;

    auto yaml = QFile(filePath);
    yaml.open(QFile::ReadOnly | QFile::Text);

    auto node = YAML::Load(yaml.readAll().toStdString());
    pimpl_->themeConfig_ = std::make_unique<YAML::Node>(node);

    pimpl_->theme_ = name;
}

const QString Theme::theme() { return pimpl_->theme_; }

/// @todo 补全字体管理
const QString Theme::font(const char* name) {
    if (pimpl_->themeConfig_ != nullptr) {
        auto& node = *pimpl_->themeConfig_;
        if (node["font"][name].IsScalar())
            return QString::fromStdString(node["font"][name].as<std::string>());
    }
    return "monospace";
}

const uint32_t Theme::color(const char* name) {
    if (pimpl_->themeConfig_ != nullptr) {
        auto& node = *pimpl_->themeConfig_;
        if (node["color"][name].IsScalar()) return node["color"][name].as<uint32_t>();
    }
    return 0x000000;
}

void Theme::addReloadThemeHandler(QWidget* widget, std::function<void(void)> handler) {
    pimpl_->widgetReloadThemeHandler_[widget] = handler;
}

void Theme::removeReloadThemeHandler(QWidget* widget) {
    pimpl_->widgetReloadThemeHandler_.erase(widget);
}

void Theme::reloadTheme() {
    for (auto& handler : pimpl_->widgetReloadThemeHandler_)
        handler.second();
}

const void Theme::printYamlString() {
    auto string = YAML::Dump(*pimpl_->themeConfig_);
    qDebug() << string.c_str();
}