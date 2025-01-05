/// @brief 南京理工大学 数据库原理及应用 面向对象程序设计
///        大作业 长途汽车信息管理系统

#include "window.hh"
#include <qapplication.h>

using namespace creeper;

auto main(int argc, char** argv) -> int {
    Theme::setTheme(Theme::common::grey);
    auto app = QApplication { argc, argv };
    auto window = Window {};
    window.show();
    return app.exec();
}