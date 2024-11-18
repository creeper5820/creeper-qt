#pragma once

#include <qdebug.h>
#include <qfile.h>
#include <qfont.h>
#include <qstring.h>

#include <atomic>

#include <yaml-cpp/yaml.h>

namespace creeper {

class Theme {
public:
    static inline bool occupied() { return occupied_; }
    static inline void setOccupied(bool occupied) { occupied_ = occupied; }
    static inline void setTheme(const QString& name) {
        const auto filePath = ":/theme/" + name + "/theme.yaml";
        if (!QFile::exists(filePath)) return;

        auto yamlFile = QFile(filePath);
        yamlFile.open(QFile::ReadOnly | QFile::Text);

        auto node = YAML::Load(yamlFile.readAll().toStdString());
        themeConfig_ = std::make_unique<YAML::Node>(node);

        theme_ = name;
    }
    static inline const void printYamlString() {
        auto string = YAML::Dump(*themeConfig_);
        qDebug() << string.c_str();
    }
    static inline const QString theme() { return theme_; }
    static inline const QString qss(const QString& name) {
        return ":/theme/" + theme() + "/qss/" + name + ".qss";
    }
    static inline const QFont font(const char* name) {
        // TODO
        if (strcmp(name, "text-font")) return { "monospace", 12 };
        else
            return {};
    }
    static inline uint32_t color(const char* name) {
        if (themeConfig_ != nullptr) {
            auto& node = *themeConfig_;
            if (node["color"][name].IsScalar()) {
                return node["color"][name].as<uint32_t>();
            }
        }
        return 0x000000;
    }

    static inline void addReloadThemeHandler(
        std::function<void(void)> handler) {
        widgetReloadThemeHandler_.push_back(handler);
    }

    static inline void reloadTheme() {
        for (auto& handler : widgetReloadThemeHandler_)
            handler();
    }

    constexpr static inline auto CommonWhite = "common-white";
    constexpr static inline auto CommonBlack = "common-black";

private:
    static inline auto theme_ = QString { CommonWhite };
    static inline auto occupied_ = std::atomic<bool> { false };
    static inline auto themeConfig_ = std::unique_ptr<YAML::Node>();

    static inline auto widgetReloadThemeHandler_
        = std::vector<std::function<void(void)>>();
};
}