#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace swp = _switch::pro;

static Switch TestSwitch {
    test::kWidgetProps,
    test::kThemeManager,
    swp::TrackColorUnchecked { Qt::red },
    swp::TrackColorChecked { Qt::red },
    swp::TrackColorUncheckedDisabled { Qt::red },
    swp::TrackColorCheckedDisabled { Qt::red },
    swp::HandleColorUnchecked { Qt::red },
    swp::HandleColorChecked { Qt::red },
    swp::HandleColorUncheckedDisabled { Qt::red },
    swp::HandleColorCheckedDisabled { Qt::red },
    swp::OutlineColorUnchecked { Qt::red },
    swp::OutlineColorChecked { Qt::red },
    swp::OutlineColorUncheckedDisabled { Qt::red },
    swp::OutlineColorCheckedDisabled { Qt::red },
    swp::HoverColorUnchecked { Qt::red },
    swp::HoverColorChecked { Qt::red },
    swp::Checked { true },
    swp::Disabled { false },
};
