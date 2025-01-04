#pragma once
#include "qstring.h"

namespace creeper::style {

inline const QString textForExample {
    "你说的对，但是《原神》是由米哈游自主研发的一款全新开放世界冒险游戏"
    "游戏发生在一个被称作「提瓦特」的幻想世界，在这里，被神选中的人将被授予"
    "「神之眼」，导引元素之力。你将扮演一位名为「旅行者」的神秘角色，在自由"
    "的旅行中邂逅性格各异、能力独特的同伴们，和他们一起击败强敌，找回失散的"
    "亲人。"
};

// @param: normal::background-color
// @param: hover::background-color
// @param: pressed::background-color
// @param: disabled::color
inline const QString pushButton { R"(
    QPushButton {
        background-color: %1;
        color: #1C1B1F;
        border: none;
        border-radius: 10%;
        text-align: center;

        font-weight: bold;
        font-family: monospace;
    }
    QPushButton:hover {
        background-color: %2;
    }
    QPushButton:pressed {
        background-color: %3;
    }
    QPushButton:disabled {
        background-color: %3;
        color: %4;
        opacity: 0.38;
    })" };

// @brief: no args
inline const QString lineEdit { R"(
    QLineEdit {
        selection-background-color: #bbbbbb;
        background: transparent;
        border: none;
    })" };

// @param: normal::background-color
inline const QString mainWindow { R"(
    QMainWindow {
        background-color: %1;
    })" };

// @param: normal::background-color
// @param: normal::border-color
// @param: selected::background-color
// @param: hover::background-color
inline const QString listWidget { R"(
    QListWidget {
        background-color: %1;
        border: 2px solid %2;
        border-radius: 10%;
        padding: 10%;
        outline: none;
    }
    QListWidget::item:selected {
        background-color: %3;
        color: #ffffff;
        border-radius: 8%;
        padding: 5%;
    }
    QListWidget::item:hover::selected {
        background-color: %3;
        color: #ffffff;
        border-radius: 8%;
        padding: 5%;
    }
    QListView::item:hover {
        background-color: %4;
        color: #414141;
        border-radius: 8%;
        padding: 5%;
    })" };
}
