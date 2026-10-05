#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;

static int TestArgc    = 0;
static char** TestArgv = nullptr;

static app::Application TestApplication {
    app::pro::Attribute { Qt::AA_DontShowIconsInMenus, true },
    app::pro::Complete { TestArgc, TestArgv },
};
