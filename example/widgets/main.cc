#include "creeper-qt/creeper-qt.hh"
#include <qapplication.h>
#include <qfontdatabase.h>
#include <qlabel.h>

using namespace creeper;

int main(int argc, char* argv[]) {
    auto application   = new QApplication { argc, argv };
    auto theme_manager = ThemeManager { kBlueMikuThemePack };

    namespace button = button::pro;
    namespace linear = linear::pro;
    namespace filled = filled_button::pro;
    namespace icon   = icon_button::pro;
    namespace sw     = _switch::pro;
    namespace font   = util::icon_font;

    auto material_icon = QFont { font::rounded, 20 };

    const auto switch_common_properties = std::tuple {
        sw::ThemeManager { theme_manager },
        sw::FixedSize { 70, 40 },
    };
    const auto button_common_properties = std::tuple {
        filled::ThemeManager { theme_manager },
        filled::FixedSize { 110, 50 },
        filled::Text { "你好世界" },
        filled::Font { "WenQuanYi Zen Hei", 13 },
        filled::Radius { 25 },
    };
    const auto card_common_properties = std::tuple {
        card::pro::ThemeManager { theme_manager },
        card::pro::FixedSize { 50, 50 },
        card::pro::Radius { 10 },
    };
    const auto icon_button_properties = std::tuple {
        icon::ThemeManager { theme_manager },
        icon::FontIcon { "settings" },
        icon::Shape { IconButton::Shape::SQUARE },
        icon::Width { IconButton::Width::DEFAULT },
        icon::Types { IconButton::Types::DEFAULT },
        icon::FixedSize { IconButton::kMediumContainerSize },
        icon::Font { font::rounded, IconButton::kMediumFontIconSize },
    };
    auto icon_buttons = std::array<IconButton*, 4> {};

    const auto card = new FilledCard {
        card::pro::ThemeManager { theme_manager },
        card::pro::Level { SurfaceLevel::HIGHEST },
        card::pro::WindowFlag { Qt::WindowType::SplashScreen },
        card::pro::Radius { 0 },
        card::pro::Layout { new Col {
            linear::SetSpacing { 20 },
            linear::Margin { 50 },
            linear::Stretch { 1 },
            linear::Layout { { new Row {
                linear::Stretch { 1 },
                linear::Widgets { {
                    { new FilledButton {
                        button_common_properties,
                        button::Text { "威严满满" },
                        button::Clickable { [] { qDebug() << "抱头蹲防"; } },
                    } },
                    { new FilledTonalButton {
                        button_common_properties,
                        button::Text { "色彩模式" },
                        button::Clickable { [&theme_manager] {
                            theme_manager.toggle_color_mode();
                            theme_manager.apply_theme();
                        } },
                    } },
                    { new OutlinedButton { button_common_properties } },
                    { new TextButton {
                        button_common_properties,
                        button::Text { "退出演示" },
                        button::Clickable { [&application] { application->exit(); } },
                    } },
                } },
                linear::Stretch { 1 },
            } } },
            linear::Layout { { new Row {
                linear::Stretch { 1 },
                linear::Widgets { {
                    { new Switch {
                        switch_common_properties,
                        sw::Checked { true },
                        sw::Clickable { [&icon_buttons](bool checked) {
                            if (checked)
                                for (const auto& icon_button : icon_buttons)
                                    icon_button->set_types(IconButton::Types::DEFAULT);
                            else
                                for (const auto& icon_button : icon_buttons)
                                    icon_button->set_types(IconButton::Types::TOGGLE_UNSELECTED);
                        } },
                    } },
                    { new Switch {
                        switch_common_properties,
                        sw::Checked { false },
                        sw::Clickable { [&icon_buttons](bool checked) {
                            for (const auto& icon_button : icon_buttons)
                                if (!checked) icon_button->set_shape(IconButton::Shape::SQUARE);
                                else icon_button->set_shape(IconButton::Shape::DEFAULT_ROUND);
                        } },
                    } },
                    { new Switch {
                        switch_common_properties,
                        sw::Checked { true },
                        sw::Disabled { true },
                    } },
                    { new Switch {
                        switch_common_properties,
                        sw::Checked { false },
                        sw::Disabled { true },
                    } },
                } },
                linear::Stretch { 1 },
            } } },
            linear::Layout { { new Row {
                linear::Stretch { 1 },
                linear::Widgets { {
                    { new IconButton {
                        icon_button_properties,
                        icon::Bind { icon_buttons[0] },
                        icon::FontIcon { util::icon_code::star },
                        icon::Color { IconButton::Color::DEFAULT_FILLED },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Bind { icon_buttons[2] },
                        icon::FontIcon { util::icon_code::search },
                        icon::Color { IconButton::Color::TONAL },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Bind { icon_buttons[1] },
                        icon::FontIcon { util::icon_code::folder_open },
                        icon::Color { IconButton::Color::OUTLINED },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Bind { icon_buttons[3] },
                        icon::FontIcon { util::icon_code::menu },
                        icon::Color { IconButton::Color::STANDARD },
                    } },
                } },
                linear::Stretch { 1 },
            } } },
            linear::Layout { { new Row {
                linear::SetSpacing { 15 },
                linear::Stretch { 1 },
                linear::Widgets { {
                    { new OutlinedCard {
                        card_common_properties,
                        card::pro::Level { OutlinedCard::Level::LOWEST },
                    } },
                    { new OutlinedCard {
                        card_common_properties,
                        card::pro::Level { OutlinedCard::Level::LOW },
                    } },
                    { new OutlinedCard {
                        card_common_properties,
                        card::pro::Level { OutlinedCard::Level::DEFAULT },
                    } },
                    { new OutlinedCard {
                        card_common_properties,
                        card::pro::Level { OutlinedCard::Level::HIGH },
                    } },
                    { new OutlinedCard {
                        card_common_properties,
                        card::pro::Level { OutlinedCard::Level::HIGHEST },
                    } },
                } },
                linear::Stretch { 1 },
            } } },
            linear::Stretch { 1 },
        } },
    };
    card->show();
    card->move(960 - card->width() / 2, 540 - card->height() / 2);

    theme_manager.set_color_mode(ColorMode::LIGHT);
    theme_manager.apply_theme();

    return application->exec();
}
