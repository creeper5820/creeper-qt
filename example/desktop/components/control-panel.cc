#include "components/control-panel.hh"

#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/wrapper/foreach.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/outlined-button.hh>

namespace creeper {

FilledCard* ControlPanel(ThemeManager& manager) noexcept {
    namespace rp = row::pro;
    namespace cp = col::pro;

    namespace fcp = filled_card::pro;
    namespace obp = outlined_button::pro;

    struct Context {
        MutableInt8 mCurrentIndex;
    };
    auto pContext = std::make_shared<Context>();

    return new FilledCard {
        fcp::ThemeManager { manager },

        fcp::Layout<Row> {
            rp::Spacing { 5 },
            rp::Alignment { Qt::AlignVCenter | Qt::AlignLeft },

            rp::Item<Col> {
                Util::ForEach(
                    std::array {
                        "按钮一",
                        "按钮二",
                        "按钮三",
                    },
                    [&](std::size_t index, std::string_view text) {
                        return cp::Item<OutlinedButton> {
                            obp::ThemeManager { manager },
                            obp::FixedSize { 80, 30 },
                            obp::Text { text.data() },
                        };
                    }),
            },
        },
    };
}

}
