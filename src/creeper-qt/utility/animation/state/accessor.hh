#pragma once

namespace creeper {

struct NormalAccessor {
    auto get_value(this auto const& self) { return self.value; }
    auto set_value(this auto& self, auto const& t) { self.value = t; }
    auto get_target(this auto const& self) { return self.target; }
    auto set_target(this auto& self, auto const& t) { self.target = t; }
};

}
