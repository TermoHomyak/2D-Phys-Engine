#pragma once

#include "core/vec2.hpp"
#include "physics/body.hpp"

#include <cmath>
#include <optional>

namespace physics {

struct Contact {
    BodyId     a{};
    BodyId     b{};
    core::Vec2 normal{};      // единичный вектор, направлен от центра a к центру b
    core::Vec2 point{};       // точка контакта в мировых координатах
    float      depth{0.0f};   // насколько формы залезли друг в друга, > 0

    float normalImpulse{0.0f};
    float tangentImpulse{0.0f};
};

[[nodiscard]] inline std::optional<Contact>
collideCircles(BodyId idA, const RigidBody& a, BodyId idB, const RigidBody& b) noexcept {
    const auto* circleA = std::get_if<Circle>(&a.shape);
    if (circleA == nullptr) return std::nullopt;
    const float ra = circleA->radius;

    const auto* circleB = std::get_if<Circle>(&b.shape);
    if (circleB == nullptr) return std::nullopt;
    const float rb = circleB->radius;

    core::Vec2 d = b.position - a.position;

    const float distSq = d.lengthSq();
    const float sumR = ra + rb;
    if (distSq >= sumR * sumR) return std::nullopt;
    const float dist = std::sqrt(distSq);  
    core::Vec2 normal;
    
    if (dist < 1e-6f){
        normal = {1.0f, 0.0f};
    } else{
        normal = d * (1.0f / dist);
    }
    const float depth = sumR - dist;
    return Contact{
        idA,
        idB,
        normal,
        a.position + normal * (ra - depth * 0.5f),
        depth
    };
}

} // namespace physics
