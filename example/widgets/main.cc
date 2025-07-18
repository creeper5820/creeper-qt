#include "creeper-qt/creeper-qt.hh"
#include <qapplication.h>

using namespace creeper;

int main(int argc, char* argv[]) {
    namespace but = button::pro;
    namespace lin = linear::pro;
    namespace fil = filled_button::pro;
    namespace ico = icon_button::pro;
    namespace swi = _switch::pro;
    namespace tef = text_field::pro;

    new ::QApplication { argc, argv };

    const auto Font = widget::pro::Font { "WenQuanYi Zen Hei", 13 };

    auto theme_manager = ThemeManager {
        kBlueMikuThemePack,
        ColorMode::LIGHT,
    };

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
        ico::Font { material::kRoundedMediumFont },
    };
    auto icon_buttons = std::array<IconButton*, 4> {};

    const auto examples_container = new OutlinedCard {
        card::pro::ThemeManager { theme_manager },
        card::pro::Level { SurfaceLevel::HIGHEST },
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
                    swi::Clickable { [&icon_buttons](bool checked) {
                        for (const auto& icon_button : icon_buttons)
                            if (checked) icon_button->set_types(IconButton::Types::DEFAULT);
                            else icon_button->set_types(IconButton::Types::TOGGLE_UNSELECTED);
                    } },
                },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { false },
                    swi::Clickable { [&icon_buttons](bool checked) {
                        for (const auto& icon_button : icon_buttons)
                            if (!checked) icon_button->set_shape(IconButton::Shape::SQUARE);
                            else icon_button->set_shape(IconButton::Shape::DEFAULT_ROUND);
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
                    ico::FontIcon { material::kStar },
                    ico::Color { IconButton::Color::DEFAULT_FILLED },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[2] },
                    ico::FontIcon { material::kSearch },
                    ico::Color { IconButton::Color::TONAL },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[1] },
                    ico::FontIcon { material::kFolderOpen },
                    ico::Color { IconButton::Color::OUTLINED },
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    ico::Bind { icon_buttons[3] },
                    ico::FontIcon { material::kMenu },
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
                    tef::ThemeManager { theme_manager },
                    tef::ClearButton { true },
                    Font,
                },
            },
            lin::Stretch { 1 },
        },
    };
    examples_container->show();

    const auto position = QPointF { 960, 540 } - examples_container->rect().center();
    examples_container->move(position.x(), position.y());

    theme_manager.apply_theme();

    return ::QApplication::exec();
}
