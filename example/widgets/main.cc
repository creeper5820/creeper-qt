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
        card::pro::Level { SurfaceLevel::LOWEST },
        card::pro::WindowFlag { Qt::WindowType::SplashScreen },
        card::pro::FixedSize { 50, 50 },
        card::pro::Radius { 10 },
    };
    const auto icon_button_properties = std::tuple {
        icon::ThemeManager { theme_manager },
        icon::FontIcon { "settings" },
        icon::FixedSize { IconButton::kSizeExtraSmall },
        icon::Color { IconButton::Color::DEFAULT_FILLED },
        icon::Font { font::rounded, IconButton::kIconSizeExtraSmall },
    };

    const auto card = new FilledCard {
        card::pro::ThemeManager { theme_manager },
        card::pro::Level { SurfaceLevel::HIGHEST },
        card::pro::WindowFlag { Qt::WindowType::SplashScreen },
        card::pro::FixedSize { 600, 300 },
        card::pro::Radius { 0 },
        card::pro::Layout { new Col {
            linear::SetSpacing { 20 },
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
                        button::Font { material_icon },
                        button::Text { "contrast" },
                        button::Clickable { [&theme_manager] {
                            theme_manager.toggle_color_mode();
                            theme_manager.apply_theme();
                        } },
                    } },
                    { new OutlinedButton { button_common_properties } },
                    { new TextButton {
                        button_common_properties,
                        button::Font { material_icon },
                        button::Text { "exit_to_app" },
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
                    } },
                    { new Switch {
                        switch_common_properties,
                        sw::Checked { false },
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
                        icon::Color { IconButton::Color::DEFAULT_FILLED },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Color { IconButton::Color::OUTLINED },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Color { IconButton::Color::TONAL },
                    } },
                    { new IconButton {
                        icon_button_properties,
                        icon::Color { IconButton::Color::STANDARD },
                    } },
                } },
                linear::Stretch { 1 },
            } } },
            linear::Layout { { new Row {
                linear::SetSpacing { 15 },
                linear::Stretch { 1 },
                linear::Widgets { {
                    { new ElevatedCard { card_common_properties } },
                    { new OutlinedCard { card_common_properties } },
                    { new FilledCard { card_common_properties } },
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
