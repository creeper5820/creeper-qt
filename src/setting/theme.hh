#pragma once

#include <qfile.h>
#include <qstring.h>

#include <atomic>

#include <yaml-cpp/yaml.h>

namespace creeper {

class Theme {
public:
    static inline bool occupied() {
        return occupied_;
    }
    static inline void setOccupied(bool occupied) {
        occupied_ = occupied;
    }
    static inline void setTheme(const QString& name) {
        const auto filePath = ":/theme/" + name + "/theme.yaml";
        if (!QFile::exists(filePath))
            return;

        auto yamlFile = QFile(filePath);
        yamlFile.open(QFile::ReadOnly | QFile::Text);

        auto node = YAML::Load(yamlFile.readAll().toStdString());
        themeConfig_ = std::make_unique<YAML::Node>(node);

        theme_ = name;
    }
    static inline const QString theme() {
        return theme_;
    }
    static inline const QString qss(const QString& name) {
        return ":/theme/" + theme() + "/qss/" + name + ".qss";
    }
    static inline uint32_t color(const char* name) {
        if (themeConfig_ != nullptr) {
            auto& node = *themeConfig_;
            if (node["color"]["name"])
                return node["color"][name].as<uint32_t>();
        }
        if (strcmp(name, "primary050") == 0)
            return 0xece7f4;
        else if (strcmp(name, "primary100") == 0)
            return 0xd0c3e5;
        else if (strcmp(name, "primary200") == 0)
            return 0xb19cd6;
        else if (strcmp(name, "primary300") == 0)
            return 0x9373c7;
        else if (strcmp(name, "primary400") == 0)
            return 0x7c55bb;
        else if (strcmp(name, "primary500") == 0)
            return 0x6638af;
        else if (strcmp(name, "primary600") == 0)
            return 0x5d34a9;
        else if (strcmp(name, "primary700") == 0)
            return 0x502ca0;
        else if (strcmp(name, "primary800") == 0)
            return 0x442698;
        else if (strcmp(name, "primary900") == 0)
            return 0x321a89;
        else
            return 0x000000;
    }

    static inline void addReloadThemeHandler(std::function<void(void)> handler) {
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