#include <creeper-qt/creeper-qt.hh>

#include <array>

#include "bundles.hh"

using namespace creeper;

static constexpr auto kMutualItems = std::array { 1, 2 };

static SelectGroup<Col, IconButton> TestMutualExclusionGroup {
    test::kLayoutProps,
    mutual_exclusion_group::pro::Compose {
      kMutualItems,
      [](int) -> IconButton* { return new IconButton { }; },
    },
    mutual_exclusion_group::pro::SignalInjection { &IconButton::clicked },
};
