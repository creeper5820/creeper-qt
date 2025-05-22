#include <iostream>
#include <qapplication.h>
#include <qpointer.h>
#include <qpushbutton.h>

#include "../../creeper-qt/utility/wrapper/widget.hh"

#include <tuple>

int main(int argc, char* argv[]) {
    std::cout << "Hello World!!" << std::endl;

    auto app = QApplication { argc, argv };

    QPointer<QPushButton> pushbutton = new QPushButton;
    pushbutton->setText("Hello World");
    pushbutton->show();

    using namespace cr;
    const auto properties = std::tuple {
        pro::MaximumWidth { 100 },
        pro::MaximumHeight { 100 },
        pro::MinimumWidth { 50 },
        pro::MinimumHeight { 50 },
    };

    auto widget1 = std::make_from_tuple<cr::Widget>(std::tuple_cat(properties,
        std::tuple {
            pro::MinimumWidth { 80 },
            pro::MinimumHeight { 80 },
        }));
    widget1.show();

    auto widget2 = std::make_from_tuple<cr::Widget>(properties);
    widget2.show();

    return app.exec();
}
