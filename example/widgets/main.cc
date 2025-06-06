#include <iostream>
#include <qapplication.h>
#include <qpointer.h>
#include <qpushbutton.h>

#include "../../creeper-qt/layout/linear.hh"
#include "../../creeper-qt/widget/widget.hh"
#include "widget/button/filled-button.hh"

int main(int argc, char* argv[]) {
    new ::QApplication { argc, argv };

    {
        using namespace creeper;
        namespace wi = pro::widget;

        auto widget0 = new Widget {
            wi::MinimumSize { 100, 100 },
            wi::MaximumSize { 200, 200 },
            wi::Font { "JetBrains Mono", 12 },
        };

        const auto properties = std::tuple {
            wi::MinimumSize { 100, 100 },
            wi::MaximumSize { 200, 200 },
            wi::Font { "JetBrains Mono", 15 },
        };

        auto widget1 = new Widget {
            properties,
        };
        auto widget2 = new Widget {
            properties,
            wi::MaximumSize { 250, 250 },
        };
        auto widget3 = new Widget {
            properties,
            wi::Font { "JetBrains Mono NL", 12 },
        };

        FilledButton* button;

        namespace li = pro::linear;
        auto main_widget = new Widget {
            wi::Layout { new Row {
                li::ContentsMargin { 20, 20, 20, 20 },
                li::Spacing { 20 },
                li::Widgets { {
                    { button = new FilledButton {
                        pro::filled_button::Text { "你好世界" },
                    }, 1, Qt::AlignCenter },
                    { widget1, 1, Qt::AlignCenter },
                    { new Widget {
                        wi::MinimumSize { 100, 100 },
                        wi::MaximumSize { 200, 200 },
                        wi::Font { "JetBrains Mono", 12 },
                    } },
                } },
                li::Spacing { 20 },
            } },
        };
        main_widget->show();

        QObject::connect(
            button, &FilledButton::clicked, [] { std::println(std::cout, "Clicked This Button"); });
    }

    return ::QApplication::exec();
}
