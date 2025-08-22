#pragma once
#include "creeper-qt/utility/wrapper/pimpl.hh"

namespace creeper::animate {

class Transition {
    CREEPER_PIMPL_DEFINITION(Transition);

public:
    struct IAnimate {
        virtual ~IAnimate()            = default;
        virtual bool update() noexcept = 0;
    };
};

}
