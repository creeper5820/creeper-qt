#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>

using namespace creeper;
namespace capro = card::pro;
namespace lnpro = linear::pro;
namespace impro = image::pro;

auto ViewComponent(ThemeManager& manager) noexcept {
    return new FilledCard {
        capro::ThemeManager { manager },
        capro::Layout<Col> {
            lnpro::Alignment { Qt::AlignTop },
            lnpro::Margin { 10 },
            lnpro::SetSpacing { 10 },

            lnpro::Item<Image> {
                impro::FixedHeight { 500 },
                impro::ContentScale { ContentScale::CROP },
                impro::BorderWidth { 5 },
                impro::PainterResource {
                    "https://c-ssl.duitang.com/uploads/item/201806/23/"
                    "20180623105245_2Quxn.jpeg",
                },
                impro::Apply { [&manager](Image& self) {
                    manager.append_handler(&self, [&](const ThemeManager& manager) {
                        const auto colorscheme = manager.color_scheme();
                        const auto colorborder = colorscheme.secondary_container;
                        self.set_border_color(colorborder);
                    });
                } },
            },
        },
    };
}
