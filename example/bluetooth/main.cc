#include <creeper-qt/creeper-qt.hh>
#include <qapplication.h>

#include <iostream>

inline auto query_bluetooth_status() -> bool { return {}; }

int main(int argc, char* argv[]) {
    using namespace creeper;
    namespace card = card::pro;
    namespace line = linear::pro;
    namespace icon = icon_button::pro;
    namespace swit = _switch::pro;
    namespace butt = text_button::pro;

    new ::QApplication { argc, argv };

    std::println(std::cout, "Hello World");
    auto manager = ThemeManager { kBlueMikuThemePack };

    const auto bluetooth_status = query_bluetooth_status();

    const auto icon_properties = std::tuple {
        icon::ThemeManager { manager },
        icon::FixedSize { IconButton::kSmallContainerSize },
        icon::Color { IconButton::Color::STANDARD },
        icon::Types { IconButton::Types::DEFAULT },
        icon::Shape { IconButton::Shape::DEFAULT_ROUND },
        icon::Width { IconButton::Width::DEFAULT },
        icon::Font { material::kOutlinedSmallFont },
    };
    const auto switch_priperties = std::tuple {
        swit::ThemeManager { manager },
        swit::FixedSize { 50, 30 },
    };
    const auto text_button_properties = std::tuple {
        butt::ThemeManager { manager },
        butt::MinimumWidth { 100 },
        butt::FixedHeight { 30 },
        butt::Radius { 15 },
        butt::Font { "WenQuanYi Micro Hei", 10 },
    };

    const auto side_bar = new FilledCard {
        card::ThemeManager { manager },
        card::Level { FilledCard::Level::LOWEST },
        card::MinimumWidth { 50 },
        card::Layout<Col> {
            line::Item<IconButton> {
                icon_properties,
                icon::FontIcon { "exit_to_app" },
                icon::Clickable { [](auto&) { ::QApplication::exit(); } },
            },
            line::Stretch { 255 },
            line::Item<IconButton> {
                icon_properties,
                icon::FontIcon { bluetooth_status ? "bluetooth" : "bluetooth_disabled" },
                icon::Color { IconButton::Color::DEFAULT_FILLED },
                icon::Types { IconButton::Types::TOGGLE_UNSELECTED },
                icon::Clickable { [](IconButton& self) {
                    auto is_selected = self.types() == IconButton::Types::TOGGLE_UNSELECTED;
                    self.set_icon(is_selected ? "bluetooth_disabled" : "bluetooth");
                } },
            },
            line::Item<IconButton> {
                icon_properties,
                icon::FontIcon { "nights_stay" },
                icon::Color { IconButton::Color::DEFAULT_FILLED },
                icon::Types { IconButton::Types::TOGGLE_UNSELECTED },
                icon::Clickable { [&manager](auto&) {
                    manager.toggle_color_mode();
                    manager.apply_theme();
                } },
            },
        },
    };

    const auto new_config = [&](const QString& name, const auto& callback)
        requires std::invocable<decltype(callback), IconButton&>
    {
        return new FilledCard {
            card::ThemeManager { manager },
            card::SizePolicy { QSizePolicy::Expanding },
            card::FixedHeight { 100 },
            card::Layout<Col> {
                line::Alignment { Qt::AlignBottom },
                line::Margin { 5 },
                line::Item<Switch> {
                    { 0, Qt::AlignRight },
                    swit::ThemeManager { manager },
                    swit::FixedSize { 50, 30 },
                },
            },
        };
    };
    const auto config = new FilledCard {
        card::ThemeManager { manager },
        card::Level { OutlinedCard::Level::LOWEST },
        card::Layout<Col> {
            line::Margin { 10 },
            line::Alignment { Qt::AlignTop },
            line::Item<IconButton> {
                { 0, Qt::AlignCenter },
                icon::ThemeManager { manager },
                icon::Color { IconButton::Color::STANDARD },
                icon::Font { material::kRoundedMediumFont },
                icon::FixedSize { IconButton::kMediumContainerSize },
                icon::FontIcon { "media_output" },
            },
            line::Spacing { 15 },
            line::Item<Col> {
                { 0, Qt::AlignCenter },
                line::Alignment { Qt::AlignTop },
                line::Margin { 0 },
                line::Spacing { 5 },
                line::Item<FilledCard> { new_config("配对蓝牙", [](IconButton&) { }) },
                line::Item<FilledCard> { new_config("配对蓝牙", [](IconButton&) { }) },
            },
        },
    };

    const auto new_device_icon = [&] {
        return new IconButton {
            icon::ThemeManager { manager },
            icon::FixedSize { IconButton::kSmallContainerSize },
            icon::Font { material::kRoundedSmallFont },
            icon::FontIcon { "bluetooth" },
            icon::Color { IconButton::Color::TONAL },
            icon::Width { IconButton::Width::NARROW },
            icon::Shape { IconButton::Shape::SQUARE },
        };
    };
    const auto device_card_properties = [&] {
        return std::tuple {
            card::ThemeManager { manager },
            card::SizePolicy { QSizePolicy::Expanding },
            card::Level { FilledCard::Level::HIGHEST },
            card::MaximumHeight { 100 },
            card::Layout<Col> {
                line::Alignment { Qt::AlignTop },
                line::Margin { 5 },
                line::Item<IconButton> {
                    { 0, Qt::AlignLeft },
                    new_device_icon(),
                },
            },
        };
    };
    const auto devices = new FilledCard {
        card::ThemeManager { manager },
        card::Level { OutlinedCard::Level::HIGH },
        card::Layout<Row> {
            line::Margin { 10 },
            line::SetSpacing { 10 },
            line::Item<Col> {
                { 5 },
                line::Alignment { Qt::AlignTop },
                line::Margin { 0 },
                line::SetSpacing { 10 },
                line::Item<TextButton> {
                    { 0, Qt::AlignCenter },
                    butt::ThemeManager { manager },
                    butt::Radius { 20 },
                    butt::Font { "WenQuanYi Micro Hei", 13 },
                    butt::Text { "设备列表" },
                    butt::FixedHeight { 40 },
                    butt::MinimumWidth { 100 },
                },
                line::Item<Row> {
                    line::Alignment { Qt::AlignTop },
                    line::Margin { 0 },
                    line::Item<Col> {
                        line::Margin { 0 },
                        line::Alignment { Qt::AlignTop },
                        line::Item<FilledCard> {
                            device_card_properties(),
                        },
                        line::Item<FilledCard> {
                            device_card_properties(),
                        },
                        line::Item<FilledCard> {
                            device_card_properties(),
                        },
                    },
                    line::Item<Col> {
                        line::Margin { 0 },
                        line::Alignment { Qt::AlignTop },
                        line::Item<FilledCard> {
                            device_card_properties(),
                        },
                        line::Item<FilledCard> {
                            device_card_properties(),
                        },
                    },
                },
            },
            line::Item<FilledCard> { { 3 }, config },
        },
    };

    const auto board = new FilledCard {
        card::ThemeManager { manager },
        card::Level { FilledCard::Level::HIGHEST },
        card::Radius { 0 },
        card::WindowFlag { Qt::WindowType::Tool },
        card::MinimumWidth { 600 },
        card::MinimumHeight { 400 },
        card::Layout<Row> {
            line::SetSpacing { 10 },
            line::Margin { 5 },
            line::Item<FilledCard> { side_bar },
            line::Item<FilledCard> { devices },
        },
    };

    board->show();
    manager.apply_theme();

    return ::QApplication::exec();
}
