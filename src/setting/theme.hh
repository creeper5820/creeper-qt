#pragma once

#include "setting/color/common-white.hh"

#include <qdebug.h>
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
        const auto filePath = ":" + name + "/yaml/theme.yaml";
        if (!QFile::exists(filePath))
            return;

        auto yamlFile = QFile { filePath };
        yamlFile.open(QFile::ReadOnly | QFile::Text);

        auto yamlNode = YAML::Load(yamlFile.readAll());
        qDebug() << "yaml size: " << yamlNode.size();

        theme_ = name;
    }
    static inline const QString theme() {
        return theme_;
    }
    static inline const QString qss(const QString& name) {
        return ":" + theme() + "/qss/" + name + ".qss";
    }
    static inline constexpr auto color(const char* name) {
        if (strcmp(name, "primary050") == 0)
            return primary050;
        else if (strcmp(name, "primary100") == 0)
            return primary100;
        else if (strcmp(name, "primary200") == 0)
            return primary200;
        else if (strcmp(name, "primary300") == 0)
            return primary300;
        else if (strcmp(name, "primary400") == 0)
            return primary400;
        else if (strcmp(name, "primary500") == 0)
            return primary500;
        else if (strcmp(name, "primary600") == 0)
            return primary600;
        else if (strcmp(name, "primary700") == 0)
            return primary700;
        else if (strcmp(name, "primary800") == 0)
            return primary800;
        else if (strcmp(name, "primary900") == 0)
            return primary900;
        return 0x00000;
    }

    constexpr static inline auto CommonWhite = "common-white";
    constexpr static inline auto CommonBlack = "common-black";

private:
    static inline QString theme_ = CommonWhite;
    static inline std::atomic<bool> occupied_ = false;
    static inline std::unique_ptr<YAML::Node> themeConfig_;
};

}