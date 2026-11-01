#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;

static Snackbar TestSnackbar {
    test::kWidgetProps,
    test::kThemeManager,
    snackbar::pro::Text { "Text" },
    snackbar::pro::ActionLabel { "Action" },
    snackbar::pro::WithDismissAction { true },
    snackbar::pro::ActionOnNewLine { false },
    snackbar::pro::Measurements { },
    snackbar::pro::Colors { },
    snackbar::pro::OnActionPerformed { [] { } },
    snackbar::pro::OnDismissed { [] { } },
};
