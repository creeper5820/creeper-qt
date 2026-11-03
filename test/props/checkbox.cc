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
    cbp::Colors {
      .checked_checkmark_color             = Qt::white,
      .unchecked_checkmark_color           = Qt::transparent,
      .disabled_checkmark_color            = Qt::white,
      .checked_box_color                   = Qt::blue,
      .unchecked_box_color                 = Qt::transparent,
      .disabled_checked_box_color          = Qt::darkGray,
      .disabled_unchecked_box_color        = Qt::transparent,
      .disabled_indeterminate_box_color    = Qt::darkGray,
      .checked_border_color                = Qt::blue,
      .unchecked_border_color              = Qt::gray,
      .disabled_border_color               = Qt::darkGray,
      .disabled_unchecked_border_color     = Qt::darkGray,
      .disabled_indeterminate_border_color = Qt::darkGray,
      .error_checked_checkmark_color       = Qt::white,
      .error_checked_box_color             = Qt::red,
      .error_checked_border_color          = Qt::red,
      .error_unchecked_border_color        = Qt::red,
      .focus_indicator_color               = Qt::black,
      .state_layer_color                   = Qt::blue,
    },
};

static Checkbox TestCheckboxIndeterminate {
    test::kWidgetProps,
    test::kThemeManager,
    cbp::CheckState { Checkbox::CheckState::INDETERMINATE },
};
