#pragma once

#include "core/vec2.hpp"
#include <variant>

namespace physics{

struct Circle { float radius{0.5f}; };
struct Box    { core::Vec2 halfExtents{0.5f, 0.5f}; };

using Shape = std::variant<Circle, Box>;

[[nodiscard]] inline float momentOfInertia(const Shape& shape, float mass) noexcept {
    if (const auto* circle = std::get_if<Circle>(&shape)) {
        return 0.5f * mass * circle->radius * circle->radius;
    }
    if (const auto* box = std::get_if<Box>(&shape)) {
        return mass * (box->halfExtents.x * box->halfExtents.x +
                       box->halfExtents.y * box->halfExtents.y) / 3.0f;
    }
    return 0.0f;
}

}