#include <creeper-qt/creeper-qt.hh>

#include <array>

#include "bundles.hh"

using namespace creeper;

static constexpr auto kGroupItems = std::array { 1, 2, 3 };

static SelectGroup<Col, IconButton> TestSelectGroup {
    test::kLayoutProps,
    group::pro::Compose {
      kGroupItems,
      [](int) -> IconButton* { return new IconButton { }; },
      Qt::AlignLeft,
    },
    group::pro::Foreach { [](IconButton&) { } },
};
