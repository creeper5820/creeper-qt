#include <creeper-qt/creeper-qt.hh>

#include "bundles.hh"

using namespace creeper;
namespace mxp = mixer::pro;

static MixerMask* TestMixerMaskHandle = nullptr;

static MixerMask TestMixerMask {
    test::kWidgetProps,
    mxp::SetMixerMask { TestMixerMaskHandle },
};
