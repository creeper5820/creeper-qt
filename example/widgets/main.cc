#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets/QtWidgets>

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
    auto manager_pro = util::theme::pro::ThemeManager { theme_manager };

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

    auto mutable_shape = Mutable { ico::shape::DEFAULT_ROUND };
    auto mutable_types = Mutable { ico::types::DEFAULT };

    const auto icon_button_properties = std::tuple {
        ico::ThemeManager { theme_manager },
        ico::width::DEFAULT,
        ico::Font { material::kRoundMediumFont },
        ico::FontIcon { "settings" },
        ico::FixedSize { IconButton::kMediumContainerSize },
    };

    const auto text_field_properties = std::tuple {
        tef::ThemeManager { theme_manager },
        tef::LabelText { "Search" },
        tef::LeadingIcon { material::icon::kSearch, material::kRoundFontName },
        Font,
    };

    auto mutable_icon  = Mutable { tef::LeadingIcon {
        material::icon::kFavorite,
        material::round::font,
    } };
    auto mutable_label = Mutable {
        tef::LabelText { "Search" },
    };

    auto icon_buttons = std::array<IconButton*, 4> {};

    const auto examples_container = new OutlinedCard {
        card::pro::ThemeManager { theme_manager },
        card::pro::Level { CardLevel::LOWEST },
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
                    but::Clickable { [&] {
                        qDebug() << "抱头蹲防";
                        static auto flag = bool { false };

                        const auto icon = (flag = !flag) //
                            ? material::icon::kHeartBroken
                            : material::icon::kFavorite;

                        mutable_icon  = tef::LeadingIcon { icon, material::round::font };
                        mutable_label = icon;
                    } },
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
                    swi::Clickable { [&](Switch& self) {
                        mutable_types = self.checked() //
                            ? ico::types::DEFAULT
                            : ico::types::TOGGLE_UNSELECTED;
                    } },
                },
                lin::Item<Switch> {
                    switch_common_properties,
                    swi::Checked { false },
                    swi::Clickable { [&](Switch& self) {
                        mutable_shape = self.checked() //
                            ? ico::shape::SQUARE
                            : ico::shape::DEFAULT_ROUND;
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
                    mutable_shape,
                    mutable_types,
                    ico::Bind { icon_buttons[0] },
                    ico::FontIcon { material::icon::kStar },
                    ico::color::DEFAULT_FILLED,
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    mutable_shape,
                    mutable_types,
                    ico::Bind { icon_buttons[2] },
                    ico::FontIcon { material::icon::kSearch },
                    ico::color::TONAL,
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    mutable_shape,
                    mutable_types,
                    ico::Bind { icon_buttons[1] },
                    ico::FontIcon { material::icon::kFolderOpen },
                    ico::color::OUTLINED,
                },
                lin::Item<IconButton> {
                    icon_button_properties,
                    mutable_shape,
                    mutable_types,
                    ico::Bind { icon_buttons[3] },
                    ico::FontIcon { material::icon::kMenu },
                    ico::color::STANDARD,
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::SetSpacing { 15 },
                lin::Stretch { 1 },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::level::HIGHEST,
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::level::HIGH,
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::level::DEFAULT,
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::level::LOW,
                },
                lin::Item<OutlinedCard> {
                    card_common_properties,
                    card::pro::level::LOWEST,
                },
                lin::Stretch { 1 },
            },
            lin::Item<Row> {
                lin::Item<FilledTextField> {
                    text_field_properties,
                    mutable_icon,
                    mutable_label,
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
