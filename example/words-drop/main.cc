/// @brief 南京理工大学 数据库原理及应用 面向对象程序设计
///        练习5 英文字母练习器

#include "window.hh"
#include <qapplication.h>

auto main(int argc, char** argv) -> int {
    creeper::Theme::setTheme("common-green");
    auto app = QApplication { argc, argv };
    auto window = Window {};
    window.setFixedSize(800, 400);
    window.show();
    return app.exec();
}