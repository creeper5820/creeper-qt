#include <qapplication.h>
#include <qpointer.h>
#include <qpushbutton.h>

#include "../../creeper-qt/layout/linear.hh"
#include "../../creeper-qt/widget/widget.hh"

#include <tuple>

int main(int argc, char* argv[]) {
    new ::QApplication { argc, argv };

    QPointer<QPushButton> pushbutton = new QPushButton;
    pushbutton->setText("Hello World");
    pushbutton->show();

    {
        namespace pro = creeper::widget;
        using creeper::Widget;

        auto widget0 = new Widget {
            pro::MinimumSize { 100, 100 },
            pro::MaximumSize { 200, 200 },
            pro::Font { "JetBrains Mono", 12 },
        };
        widget0->show();

        auto properties = std::tuple {
            pro::MinimumSize { 100, 100 },
            pro::MaximumSize { 200, 200 },
            pro::Font { "JetBrains Mono", 15 },
        };

        auto widget1 = new Widget { properties };
        widget1->show();

        auto widget2 = new Widget { properties, pro::MaximumSize { 250, 250 } };
        widget2->show();

        auto widget3 = new Widget { properties, pro::MinimumSize { 250, 250 } };
        widget3->show();
    }

    return ::QApplication::exec();
}
