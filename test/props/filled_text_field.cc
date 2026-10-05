#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace tfp = text_field::pro;

static FilledTextField TestFilledTextField {
    test::kWidgetProps,
    test::kThemeManager,
    tfp::ClearButton { true },
    tfp::LabelText { "Label" },
    tfp::Text { "text" },
    tfp::ReadOnly { false },
    tfp::LeadingIcon { QString { }, QString { } },
    tfp::OnChanged { [](const QString&) { } },
    tfp::OnEditingFinished { [] { } },
    tfp::OnPressed { [] { } },
};
