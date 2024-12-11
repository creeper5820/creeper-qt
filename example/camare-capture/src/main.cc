/// @brief 南京理工大学 嵌入式大作业 基于STM32F10x的智能分拣系统设计
///        Qt上位机设计

#include "window.hh"

auto main(int argc, char* argv[]) -> int {
    creeper::Theme::setTheme("common-green");
    auto app = QApplication(argc, argv);
    auto window = Workbench();
    window.show();
    return app.exec();
}
