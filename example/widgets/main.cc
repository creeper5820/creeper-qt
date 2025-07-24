#include "creeper-qt/creeper-qt.hh"
#include <qapplication.h>
#include <qmainwindow.h>

using namespace creeper;

int main(int argc, char* argv[]) {
    namespace but = button::pro;
    namespace lin = linear::pro;
    namespace fil = filled_button::pro;
    namespace ico = icon_button::pro;
    namespace swi = _switch::pro;
    namespace tef = text_field::pro;

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    new ::QApplication { argc, argv };

    auto theme_manager = ThemeManager {
        kBlueMikuThemePack,
        ColorMode::LIGHT,
    };

    const auto Font = widget::pro::Font { "WenQuanYi Micro Hei", 12 };

    const auto switch_common_properties = std::tuple {
        swi::ThemeManager { theme_manager },
        swi::FixedSize { 70, 40 },
    };
    const auto button_common_properties = std::tuple {
        fil::ThemeManager { theme_manager },
        fil::FixedSize { 110, 50 },
        fil::Text { "你好世界" },
        fil::Radius { 25 },
        Font,
    };
    const auto card_common_properties = std::tuple {
        card::pro::ThemeManager { theme_manager },
        card::pro::FixedSize { 50, 50 },
        card::pro::Radius { 10 },
    };
    const auto icon_button_properties = std::tuple {
        ico::ThemeManager { theme_manager },
        ico::FontIcon { "settings" },
        ico::Shape { IconButton::Shape::SQUARE },
        ico::Width { IconButton::Width::DEFAULT },
        ico::Types { IconButton::Types::DEFAULT },
        ico::FixedSize { IconButton::kMediumContainerSize },
        ico::Font { material::kRoundMediumFont },
    };
    const auto text_field_properties = std::tuple {
        tef::ThemeManager { theme_manager },
        tef::LabelText { "Search" },
        tef::LeadingIcon { material::icon::kSearch, material::kRoundFontName },
        Font,
    };

    auto icon_buttons = std::array<IconButton*, 4> {};

    const auto examples_container = new OutlinedCard {
        card::pro::ThemeManager { theme_manager },
        card::pro::Level { SurfaceLevel::LOWEST },
        card::pro::WindowFlag { Qt::WindowType::SplashScreen },
        card::pro::Radius { 5 },
        card::pro::Layout<Col> {
            lin::SetSpacing { 20 },
            lin::Margin { 50 },
            lin::Stretch { 1 },
            lin::Item<Row> {
                lin::Stretch { 1 },
                lin::Item<FilledButton> {
                    button_common_properties,
                    but::Text { "威严满满" },
                    but::Clickable { [] { qDebug() << "抱头蹲防"; } },
                },
                lin::Item<FilledTonalButton> {
                    button_common_properties,
                    but::Text { "色彩模式" },
                    but::Clickable { [&theme_manager] {
                        theme_manager.toggle_color_mode();
                        theme_manager.apply_theme();
                    } },
                },
                lin::Item<OutlinedButton> { button_common_properties },
                lin::Item<TextButton> {
                    button_common_properties,
                    but::Text { "退出演示" },
                    but::Clickable { [] { qApp->quit(); } },
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::Stretch { 1 },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { true },
                    swi::Clickable { [&icon_buttons](Switch& self) {
                        for (const auto& icon_button : icon_buttons)
                            icon_button->set_types(self.checked() //
                                    ? IconButton::Types::DEFAULT
                                    : IconButton::Types::TOGGLE_UNSELECTED);
                    } },
                },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { false },
                    swi::Clickable { [&icon_buttons](Switch& self) {
                        for (const auto& icon_button : icon_buttons)
                            icon_button->set_shape(self.checked() //
                                    ? IconButton::Shape::DEFAULT_ROUND
                                    : IconButton::Shape::SQUARE);
                    } },
                },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { true },
                    swi::Disabled { true },
                },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { false },
                    swi::Disabled { true },
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::Stretch { 1 },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[0] },
                    ico::FontIcon { material::icon::kStar },
                    ico::Color { IconButton::Color::DEFAULT_FILLED },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[2] },
                    ico::FontIcon { material::icon::kSearch },
                    ico::Color { IconButton::Color::TONAL },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[1] },
                    ico::FontIcon { material::icon::kFolderOpen },
                    ico::Color { IconButton::Color::OUTLINED },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[3] },
                    ico::FontIcon { material::icon::kMenu },
                    ico::Color { IconButton::Color::STANDARD },
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::SetSpacing { 15 },
                lin::Stretch { 1 },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::Level { OutlinedCard::Level::LOWEST },
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::Level { OutlinedCard::Level::LOW },
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::Level { OutlinedCard::Level::DEFAULT },
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::Level { OutlinedCard::Level::HIGH },
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::Level { OutlinedCard::Level::HIGHEST },
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::Item<FilledTextField> {
                    text_field_properties,
                    tef::LabelText { "Search" },
                    tef::LeadingIcon { material::icon::kSearch, material::kRoundFontName },
                },
                lin::Item<OutlinedTextField> {
                    text_field_properties,
                    tef::LabelText { "Favorite" },
                    tef::LeadingIcon { material::icon::kFavorite, material::kRoundFontName },
                },
            },
            lin::Stretch { 1 },
        },
    };

    auto window = new QMainWindow {};
    window->setCentralWidget(examples_container);
    window->show();

    theme_manager.apply_theme();

    return ::QApplication::exec();
}
