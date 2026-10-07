#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace cbp = checkbox::pro;

static Checkbox TestCheckbox {
    test::kWidgetProps,
    test::kThemeManager,
    cbp::Checked { false },
    cbp::Disabled { false },
    cbp::CheckState { Checkbox::CheckState::UNSELECTED },
    cbp::Clickable { [](auto&) { } },
};

static Checkbox TestCheckboxColorSpecs {
    test::kWidgetProps,
    test::kThemeManager,
    cbp::ColorSpecs {
      .enabled = {
        .checked       = { Qt::white, Qt::blue, Qt::blue },
        .unchecked     = { Qt::transparent, Qt::transparent, Qt::gray },
        .indeterminate = { Qt::white, Qt::blue, Qt::blue },
      },
      .disabled = {
        .checked       = { Qt::white, Qt::darkGray, Qt::darkGray },
        .unchecked     = { Qt::transparent, Qt::transparent, Qt::darkGray },
        .indeterminate = { Qt::white, Qt::darkGray, Qt::darkGray },
      },
      .error = {
        .checked       = { Qt::white, Qt::red, Qt::red },
        .unchecked     = { Qt::transparent, Qt::transparent, Qt::red },
        .indeterminate = { Qt::white, Qt::red, Qt::red },
      },
    },
};

static Checkbox TestCheckboxIndeterminate {
    test::kWidgetProps,
    test::kThemeManager,
    cbp::CheckState { Checkbox::CheckState::INDETERMINATE },
};
