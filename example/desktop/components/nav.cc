#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/mutual-exclusion-group.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>

#include "component.hh"

using namespace creeper;
namespace fc = filled_card::pro;
namespace sg = select_group::pro;
namespace ln = linear::pro;
namespace im = image::pro;
namespace ic = icon_button::pro;

auto NavComponent(NavComponentState& state) noexcept -> raw_pointer<QWidget> {

    const auto AvatarComponent = new Image {
        im::FixedSize { 60, 60 },
        im::Radius { -1 },
        im::ContentScale { ContentScale::CROP },
        im::BorderWidth { 3 },
        im::PainterResource {
          "https://r2.creeper5820.com/creeper-qt/ohtoai.png",
          [] { qDebug() << "[main] Image loading completed"; },
        },
    };
    state.manager.appendHandler(AvatarComponent, [AvatarComponent](const ThemeManager& manager) {
        const auto colorscheme = manager.colorScheme();
        const auto colorborder = colorscheme.secondary_container;
        AvatarComponent->setBorderColor(colorborder);
    });

    const auto navigation_icons_config = std::tuple {
        ic::BindTheme { state.manager },
        ic::ColorStandard,
        ic::ShapeRound,
        ic::TypesToggleUnselected,
        ic::WidthDefault,
        ic::Font(material::round::font_1),
        ic::FixedSize(IconButton::kSmallContainerSize),
    };

    return new FilledCard {
        state.manager,
        fc::Radius { 0 },
        fc::Level { CardLevel::HIGHEST },

        new Col {
          ln::Spacing { 10 },
          ln::Margin { 15 },

          AvatarComponent + Col::Placement { 0, Qt::AlignHCenter },
          ln::SpacingItem { 20 },
          new SelectGroup<Col, IconButton> {
            ln::Margin { 0 },
            ln::SpacingItem { 10 },
            sg::Compose {
              state.buttons_context | std::views::enumerate,
              [&](int index, const auto& context) {
                  const auto& [name, icon] = context;

                  const auto status = (index == 0) //
                      ? ic::TypesToggleSelected
                      : ic::TypesToggleUnselected;

                  return new IconButton {
                      navigation_icons_config,
                      status,
                      ic::ColorFilled,
                      ic::FontIcon { QString::fromUtf8(icon.data(), icon.size()) },
                      ic::Clickable { [=] { state.switch_callback(index, name); } },
                  };
              },
              Qt::AlignHCenter,
            },
            sg::SignalInjection { &IconButton::clicked },
          } + Col::Placement { 0, Qt::AlignHCenter },
          ln::SpacingItem { 40 },
          ln::Stretch { 255 },
          new IconButton {
            navigation_icons_config,
            ic::TypesDefault,
            ic::FontIcon { "tab" },
            ic::Clickable { state.next_tab },
          } + Col::Placement { 0, Qt::AlignHCenter },
          new IconButton {
            navigation_icons_config,
            ic::TypesDefault,
            ic::FontIcon { material::icon::kLogout },
            ic::Clickable { &app::quit },
          } + Col::Placement { 0, Qt::AlignHCenter },
          new IconButton {
            navigation_icons_config,
            ic::ColorFilled,
            ic::FontIcon { material::icon::kDarkMode },
            ic::Clickable { [&](IconButton& self) {
                std::ignore = self.selected();
                state.manager.toggleColorMode();
                state.manager.applyTheme();
            } },
          } + Col::Placement { 0, Qt::AlignHCenter },
        },
    };
}
