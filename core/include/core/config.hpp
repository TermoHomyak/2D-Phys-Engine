#pragma once

namespace core {

struct Defaults {
    static constexpr float gravity     = 9.81f;
    static constexpr int   solverIters = 8;
    static constexpr bool  warmStart   = true;
    static constexpr int   maxContacts = 4096;
    static constexpr float slop        = 0.01f;
    static constexpr float sleepVel    = 0.05f;
};

static_assert(Defaults::solverIters > 0, "solver does not work without iterations");

}
